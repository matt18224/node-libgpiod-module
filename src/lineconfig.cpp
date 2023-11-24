//
// Created by Matt on 6/25/2023.
//

#include "lineconfig.hpp"
#include "linesettings.hpp"

Napi::FunctionReference LineConfig::constructor;

LineConfig::LineConfig(const Napi::CallbackInfo &info) : ObjectWrap(info)
{
  lineConfigInstance = std::make_unique<gpiod::line_config>();
}

Napi::Object LineConfig::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "LineConfig", {
      InstanceMethod("reset", &LineConfig::Reset),
      InstanceMethod("addLineSetting", &LineConfig::AddLineSetting)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("LineConfig", func);
  return exports;
}

LineConfig::~LineConfig() = default;

Napi::Value LineConfig::Reset(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  lineConfigInstance->reset();
  return env.Undefined();
}

Napi::Value LineConfig::AddLineSetting(const Napi::CallbackInfo &info)
{
  //Args required: number, LineSettings
  const unsigned int offset = info[0].As<Napi::Number>();
  const auto lineSettings = Napi::ObjectWrap<LineSettings>::Unwrap(info[1]
                                                                       .As<Napi::Object>());
  lineConfigInstance->add_line_settings(offset, *(lineSettings->lineSettingsInstance));
  return info.This();
}