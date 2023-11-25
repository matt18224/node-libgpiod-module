//
// Created by Matt on 6/24/2023.
//

#include "linesettings.hpp"
#include "utils.hh"
#include "enumhelpers.hpp"

Napi::FunctionReference LineSettings::constructor;

Napi::Object LineSettings::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "LineSettings", {
      InstanceMethod("getDirection", &LineSettings::GetDirection),
      InstanceMethod("setDirection", &LineSettings::SetDirection),
      InstanceMethod("reset", &LineSettings::Reset),
      InstanceMethod("setOutputValue", &LineSettings::SetOutputValue),
      InstanceMethod("getOutputValue", &LineSettings::GetOutputValue),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  env.SetInstanceData<Napi::FunctionReference>(&constructor);

  exports.Set("LineSettings", func);
  return exports;
}

LineSettings::LineSettings(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<LineSettings>(info)
{
  lineSettingsInstance = std::make_shared<gpiod::line_settings>();
}

LineSettings::~LineSettings() = default;

// RV: string | undefined
Napi::Value LineSettings::GetDirection(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  std::string lineDirection = directionToName(info, lineSettingsInstance->direction());
  return Napi::String::New(env, lineDirection);
}

Napi::Value LineSettings::SetDirection(const Napi::CallbackInfo &info)
{
  std::string direction = info[0].As<Napi::String>().Utf8Value();
  gpiod::line::direction lineDirection = nameToDirection(info, direction);
  lineSettingsInstance->set_direction(lineDirection);
  return info.This();
}

Napi::Value LineSettings::Reset(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  checkArgTypes(info, std::vector<Validator>());
  lineSettingsInstance->reset();
  return env.Undefined();
}

Napi::Value LineSettings::SetOutputValue(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  //info[0] is a number in a valid range, info[1] is a string
  bool valueStr = info[0].As<Napi::Number>().ToBoolean();
  gpiod::line::value value = boolToValue(info, valueStr);
  lineSettingsInstance->set_output_value(value);
  return info.This();
}

Napi::Value LineSettings::GetOutputValue(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  int rv = valueToBool(info, lineSettingsInstance->output_value());
  return Napi::Number::New(env, rv);
}