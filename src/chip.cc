#include "chip.hh"

Napi::FunctionReference Chip::constructor;

Napi::Object Chip::Init(Napi::Env env, Napi::Object exports) {

  Napi::Function func = DefineClass(env, "Chip", {
  InstanceMethod("getNumberOfLines", &getNumberOfLines),
  InstanceMethod("getChipName", &getChipName),
  InstanceMethod("getChipLabel", &getChipLabel),
  });

  constructor = Napi::Persistent(func);

  exports.Set("Chip", func);
  env.SetInstanceData<Napi::FunctionReference>(constructor);
  return exports;
}

Chip::Chip(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Chip>(info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsString()) {
    Napi::TypeError::New(env, "Expected string argument for device").ThrowAsJavaScriptException();
    return;
  }

  std::string device = info[0].As<Napi::String>();

  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  chip = gpiod_chip_open_lookup(device.c_str());
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, chip);

  if (!chip) Napi::Error::New(env, "Unable to open device").ThrowAsJavaScriptException();
}

Chip::~Chip() {
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, chip);
  if ( !chip) return;
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, chip);
  gpiod_chip_close(chip);
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, chip);
  chip = NULL;
}

Napi::Value Chip::New(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::FunctionReference* constructor =
      info.Env().GetInstanceData<Napi::FunctionReference>()
  std::string device = info[0].As<Napi::String>().Utf8Value();
  return constructor->New({ Napi::String::New(info.Env(), device.c_str()) });
}

Napi::Value Chip::getNumberOfLines(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Chip::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->chip) {
    Napi::Error::New(env,  "::getNumberOfLines() for chip==NULL").ThrowAsJavaScriptException();

    return env.Null();
  }
  int ret = gpiod_chip_num_lines(obj->getNativeChip());
  if(-1 == ret) {
    Napi::Error::New(env,  "::getNumberOfLines() failed").ThrowAsJavaScriptException();
    return env.Null();

  } else return Napi::Number::New(env, ret);
}

Napi::Value Chip::getChipName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Chip::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->chip) {
    Napi::Error::New(env,  "::getChipName() for chip==NULL").ThrowAsJavaScriptException();

    return env.Null();
  }
  const char *name = gpiod_chip_name(obj->getNativeChip());
  if(!name) {
    Napi::Error::New(env,  "::getChipName() failed").ThrowAsJavaScriptException();
    return env.Null();

  } else return Napi::String::New(env, name);
}

Napi::Value Chip::getChipLabel(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Chip::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->chip) {
    Napi::Error::New(env,  "::getChipLabel() for chip==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  const char *label = gpiod_chip_label(obj->getNativeChip());
  if(!label) {
    Napi::Error::New(env,  "::getChipLabel() failed").ThrowAsJavaScriptException();
    return env.Null();
  } else return Napi::String::New(env, label);
}

gpiod_chip *Chip::getNativeChip() {
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, chip);
  return chip;
}
