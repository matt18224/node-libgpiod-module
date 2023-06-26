//
// Created by Matt on 6/23/2023.
//

#include "linerequest.hpp"
#include "enumhelpers.hpp"

Napi::FunctionReference LineRequest::constructor;

LineRequest::LineRequest(const Napi::CallbackInfo &info) : ObjectWrap(info)
{
  auto nativeRequest = info[0].As<Napi::External<gpiod::line_request>>().Data();
  lineRequestInstance = std::unique_ptr<gpiod::line_request, LineRequestDestructor>(
      nativeRequest);
}

Napi::Object LineRequest::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "LineSettings", {
      InstanceMethod("getValue", &LineRequest::GetValue),
      InstanceMethod("setValue", &LineRequest::SetValue),
      InstanceMethod("release", &LineRequest::Release)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("LineRequest", func);
  return exports;
}

Napi::Value LineRequest::GetValue(const Napi::CallbackInfo &info)
{
  //Returns 0 or 1
  unsigned int offset = info[0].As<Napi::Number>();
  int rv = valueToBool(info, lineRequestInstance->get_value(offset));
  return Napi::Number::New(info.Env(), rv);
}

Napi::Value LineRequest::SetValue(const Napi::CallbackInfo &info)
{
  unsigned int offset = info[0].As<Napi::Number>();
  int intValue = info[1].As<Napi::Number>();
  auto value = boolToValue(info, intValue);
  lineRequestInstance->set_value(offset, value);
  return info.Env().Undefined();
}

Napi::Value LineRequest::Release(const Napi::CallbackInfo &info)
{
  lineRequestInstance->release();
  return info.Env().Undefined();
}