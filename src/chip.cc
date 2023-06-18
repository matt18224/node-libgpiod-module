#include "chip.hh"

Napi::FunctionReference Chip::constructor;

Napi::Object Chip::Init(Napi::Env env, Napi::Object exports) {
  Napi::FunctionReference tpl = Napi::Function::New(env, New);
  tpl->SetClassName(Napi::String::New(env, "Chip"));


  InstanceMethod("getNumberOfLines", &getNumberOfLines),
  InstanceMethod("getChipName", &getChipName),
  InstanceMethod("getChipLabel", &getChipLabel),

  constructor.Reset(Napi::GetFunction(tpl));
  (target).Set(Napi::String::New(env, "Chip"), Napi::GetFunction(tpl));
}

Chip::Chip(Napi::Env env, const char *device) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  chip = gpiod_chip_open_lookup(device);
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
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  if (info.IsConstructCall()) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
    std::string device = info[0].As<Napi::String>();
    Chip *obj = new Chip(env, *device);
    DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, obj);
    if ( !obj->chip) return env.Null();
    DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, obj);
    obj->Wrap(info.This());
    return info.This();
  } else {
    DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
    const int argc = 1;
    Napi::Value argv[argc] = {info[0]};
    Napi::Function cons = Napi::New(env, constructor);
    return Napi::NewInstance(cons, argc, argv);
  }
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
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
