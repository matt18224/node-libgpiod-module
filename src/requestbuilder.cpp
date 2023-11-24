//
// Created by Matt on 6/25/2023.
//

#include "requestbuilder.hpp"
#include "chip.hh"
#include "linerequest.hpp"
#include "napi.h"
#include "linesettings.hpp"

Napi::FunctionReference RequestBuilder::constructor;

Napi::Object RequestBuilder::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "LineSettings", {
      InstanceMethod("addLineSettings", &RequestBuilder::AddLineSettings),
      InstanceMethod("doRequest", &RequestBuilder::DoRequest),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("LineSettings", func);
  return exports;
}

RequestBuilder::RequestBuilder(const Napi::CallbackInfo &info) : ObjectWrap(info)
{
  if (info.Length() < 1 || !info[0].IsExternal())
  {
    Napi::TypeError::New(info.Env(), "Expected an External<request_builder>")
        .ThrowAsJavaScriptException();
  }
  gpiod::request_builder *arg = info[0].As<Napi::External<gpiod::request_builder>>()
                                       .Data();
  //Take ownership of pointer
  requestBuilderInstance = std::unique_ptr<gpiod::request_builder>(arg);
}

RequestBuilder::~RequestBuilder() = default;

Napi::Value RequestBuilder::AddLineSettings(const Napi::CallbackInfo &info)
{
  //Args required: number, LineSettings
  const unsigned int offset = info[0].As<Napi::Number>();
  const auto lineSettings = Napi::ObjectWrap<LineSettings>::Unwrap(info[1]
                                                                       .As<Napi::Object>());
  //Safe to pass reference. LineSettings are copied, a reference isn't held
  requestBuilderInstance->add_line_settings(
      offset,
      *(lineSettings->lineSettingsInstance));
  return info.This();
}

Napi::Value RequestBuilder::DoRequest(const Napi::CallbackInfo &info)
{
  auto copied = new gpiod::line_request(requestBuilderInstance->do_request());
  Napi::Value ext = Napi::External<gpiod::line_request>::New(
      info.Env(),
      copied
  );
  return LineRequest::constructor.New({ext});
}


