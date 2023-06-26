#include "chip.hh"
#include "requestbuilder.hpp"
#include <filesystem>

Napi::Object Chip::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Chip", {
      InstanceMethod("getNumberOfLines", &Chip::getNumberOfLines),
      InstanceMethod("getChipName", &Chip::getChipName),
      InstanceMethod("getChipLabel", &Chip::getChipLabel),
      InstanceMethod("createRequest", &Chip::createRequest)
  });

  auto *constructor = new Napi::FunctionReference();

  // Create a persistent reference to the class constructor. This will allow
  // a function called on a class prototype and a function
  // called on instance of a class to be distinguished from each other.
  *constructor = Napi::Persistent(func);
  constructor->SuppressDestruct();
  exports.Set("Chip", func);

  // Store the constructor as the add-on instance data. This will allow this
  // add-on to support multiple instances of itself running on multiple worker
  // threads, as well as multiple instances of itself running in different
  // contexts on the same thread.
  //
  // By default, the value set on the environment here will be destroyed when
  // the add-on is unloaded using the `delete` operator, but it is also
  // possible to supply a custom deleter.
  env.SetInstanceData<Napi::FunctionReference>(constructor);
  return exports;
}

Chip::Chip(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Chip>(info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DOUT("%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  std::string device;
  if (info[0].IsNumber())
  {
    device = info[0].ToString().Utf8Value();
  }
  else if (info[0].IsString())
  {
    device = info[0].As<Napi::String>().Utf8Value();
  }
  else
  {
    throw Napi::Error::New(env,
                           "Wrong argument type. Expected string or number");
  }
  chip = std::unique_ptr<gpiod::chip, ChipDeleter>(new gpiod::chip(device));
  DOUT("%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, chip);
  if (!chip)
  {
    throw Napi::Error::New(env, "Unable to open device");
  }
}

Chip::~Chip()
{
}

Napi::Value Chip::getNumberOfLines(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  DOUT("%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Napi::ObjectWrap<Chip>::Unwrap(info.This().As<Napi::Object>());
  int ret = obj->chip->get_info().num_lines();;
  if (-1 == ret)
  {
    throw Napi::Error::New(env, "::getNumberOfLines() failed");
  }
  else return Napi::Number::New(env, ret);
}

Napi::Value Chip::getChipName(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  DOUT("%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Napi::ObjectWrap<Chip>::Unwrap(info.This().As<Napi::Object>());
  if (!obj->chip)
  {
    throw Napi::Error::New(env, "::getChipLabel() for chip==NULL");
  }
  auto name = chip->get_info().name();
  return Napi::String::New(env, name);
}

Napi::Value Chip::getChipLabel(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  DOUT("%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Napi::ObjectWrap<Chip>::Unwrap(info.This().As<Napi::Object>());
  if (!obj->chip)
  {
    throw Napi::Error::New(env, "::getChipLabel() for chip==NULL");
  }
  const std::string label = chip->get_info().label();
  return Napi::String::New(env, label);
}

Napi::Value Chip::createRequest(const Napi::CallbackInfo &info)
{
  auto copied = new gpiod::request_builder(chip->prepare_request());
  Napi::Value ext = Napi::External<gpiod::request_builder>::New(
      info.Env(),
      copied
  );
  return RequestBuilder::constructor.New({ext});
}