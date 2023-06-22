#include "chip.hh"

Napi::FunctionReference Chip::constructor;

Napi::Object Chip::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Chip", {
        InstanceMethod<&Chip::getNumberOfLines>("getNumberOfLines", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Chip::getChipName>("getChipName", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Chip::getChipLabel>("getChipLabel", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        StaticMethod<&Chip::New>("New", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
    });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();

    // Create a persistent reference to the class constructor. This will allow
    // a function called on a class prototype and a function
    // called on instance of a class to be distinguished from each other.
    *constructor = Napi::Persistent(func);
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
}

Napi::Value Chip::New(const Napi::CallbackInfo& info) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Napi::FunctionReference* constructor = info.Env().GetInstanceData<Napi::FunctionReference>();
  return constructor->New({
      std::string device = info[0].As<Napi::String>()
  });
}

Chip::Chip(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Chip>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  std::string device = info[0].As<Napi::String>();
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

Napi::Value Chip::getNumberOfLines(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Napi::ObjectWrap<Chip>::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->chip) {
    Napi::Error::New(env, "::getNumberOfLines() for chip==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  int ret = gpiod_chip_num_lines(obj->getNativeChip());
  if(-1 == ret) {
    Napi::Error::New(env, "::getNumberOfLines() failed").ThrowAsJavaScriptException();
  } else return Napi::Number::New(env, ret);
}

Napi::Value Chip::getChipName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Napi::ObjectWrap<Chip>::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->chip) {
    Napi::Error::New(env, "::getChipName() for chip==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  const char *name = gpiod_chip_name(obj->getNativeChip());
  if(!name) {
    Napi::Error::New(env, "::getChipName() failed").ThrowAsJavaScriptException();
  } else return Napi::String::New(env, name);
}

Napi::Value Chip::getChipLabel(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Chip *obj = Napi::ObjectWrap<Chip>::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->chip) {
    Napi::Error::New(env, "::getChipLabel() for chip==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  const char *label = gpiod_chip_label(obj->getNativeChip());
  if(!label) {
    Napi::Error::New(env, "::getChipLabel() failed").ThrowAsJavaScriptException();
  } else return Napi::String::New(env, label);
}

gpiod_chip *Chip::getNativeChip() {
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, chip);
  return chip;
}