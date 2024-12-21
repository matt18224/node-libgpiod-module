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

LineConfig::LineConfig(const Napi::CallbackInfo &info,
                       const gpiod::line_config &config) : ObjectWrap(info)
{
  lineConfigInstance = std::make_unique(config);
}

Napi::Object LineConfig::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "LineConfig", {
      InstanceMethod("reset", &LineConfig::Reset),
      InstanceMethod("addLineSetting", &LineConfig::AddLineSetting),
      StaticMethod("createFromLiteral", &LineConfig::CreateFromLiteral)
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
  privateAddLineSetting(offset, lineSettings);
  return info.This();
}

Napi::Value LineConfig::CreateFromLiteral(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1 || !info[0].IsObject())
  {
    throw Napi::TypeError::New(env, "Expected single object mapping int offsets to "
                                    "LineSettings instance");
  }

  auto obj = info[0].As<Napi::Object>();
  Napi::Array propertyNames = obj.GetPropertyNames();

  Napi::Object instance = constructor.New({});
  LineConfig *config = Napi::ObjectWrap<LineConfig>::Unwrap(instance);

  // Iterate through all properties
  for (uint32_t i = 0; i < propertyNames.Length(); ++i)
  {
    Napi::String key = propertyNames.Get(i).ToString();
    Napi::Value value = obj.Get(key);

    // Ensure key is a number and value is an object
    if (!value.IsObject())
    {
      continue; // Or handle the error
    }

    uint32_t offset = key.ToNumber().Uint32Value();
    LineSettings *valueString = Napi::ObjectWrap<LineSettings>::Unwrap(
        value.As<Napi::Object>());

    config->privateAddLineSetting(offset, valueString);
  }
  return instance;
}

void LineConfig::privateAddLineSetting(gpiod::line::offset offset,
                                       const LineSettings *lineSettings)
{
  lineConfigInstance->add_line_settings(offset, *(lineSettings->lineSettingsInstance));
}

