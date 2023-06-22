#include "line.hh"

Napi::Object Line::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Line", {
        InstanceMethod<&Line::getLineOffset>("getLineOffset", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Line::getLineName>("getLineName", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Line::getLineConsumer>("getLineConsumer", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Line::getValue>("getValue", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Line::setValue>("setValue", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Line::requestInputMode>("requestInputMode", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Line::requestOutputMode>("requestOutputMode", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Line::release>("release", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        StaticMethod<&Chip::CreateNewInstance>("CreateNewInstance", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
    });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();

    // Create a persistent reference to the class constructor. This will allow
    // a function called on a class prototype and a function
    // called on instance of a class to be distinguished from each other.
    *constructor = Napi::Persistent(func);
    exports.Set("Line", func);

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

Line::Line(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Line>(info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);

  Chip *chip = Napi::ObjectWrap<Chip>::Unwrap(info[0].As<Napi::Object>());
  unsigned int pin = info[1].As<Napi::Number>().Uint32Value();
  line = gpiod_chip_get_line(chip->getNativeChip(), pin);
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, line);
  if (!line){
    Napi::Error::New(env, "Unable to open GPIO line ").ThrowAsJavaScriptException();
    return env.Null();
  }
}

Line::~Line() {
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, line);
  if ( !line) return;
  gpiod_line_close_chip(line);
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, line);
  line = NULL;
}

Napi::Value Line::getLineOffset(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = Napi::ObjectWrap<Line>::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->line) {
    Napi::Error::New(env, "::getLineOffset() for line==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  int ret = gpiod_line_offset(obj->getNativeLine());
  if(-1 == ret) {
    Napi::Error::New(env, "::getLineOffset() failed").ThrowAsJavaScriptException();
    return env.Null();
  } else return Napi::Number::New(env, ret);
}

Napi::Value Line::getLineName(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = Napi::ObjectWrap<Line>::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->line) {
    Napi::Error::New(env, "::getLineName() for line==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  const char *name = gpiod_line_name(obj->getNativeLine());
  if(!name) return env.Undefined();
  else return Napi::String::New(env, name);
}

Napi::Value Line::getLineConsumer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = Napi::ObjectWrap<Line>::Unwrap(info.This().As<Napi::Object>());
  if ( !obj->line) {
    Napi::Error::New(env, "::getLineConsumer() for line==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  const char *name = gpiod_line_consumer(obj->getNativeLine());
  if(!name) return env.Undefined();
  else return Napi::String::New(env, name);
}

unsigned int Line::getValueCpp() {
  if (!this->line) {
    throw std::runtime_error("Line is NULL");
  }
  int ret = gpiod_line_get_value(this->getNativeLine());
  if (-1 == ret) {
    throw std::runtime_error("getValue failed");
  }
  return ret;
}

void Line::setValueCpp(unsigned int value) {
  if (!this->line) {
    throw std::runtime_error("Line is NULL");
  }
  if (-1 == gpiod_line_set_value(this->getNativeLine(), value)) {
    throw std::runtime_error("setValue failed");
  }
}

Napi::Value Line::getValue(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Line *obj = Napi::ObjectWrap<Line>::Unwrap(info.This().As<Napi::Object>());
  try {
    unsigned int ret = obj->getValueCpp();
    return Napi::Number::New(env, ret);
  } catch (const std::runtime_error& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Line::setValue(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Line *obj = Napi::ObjectWrap<Line>::Unwrap(info.This().As<Napi::Object>());
  unsigned int value = info[0].As<Napi::Number>().Uint32Value();
  try {
    obj->setValueCpp(value);
  } catch (const std::runtime_error& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}

Napi::Value Line::requestInputMode(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = Napi::ObjectWrap<Line>::Unwrap(info.This().As<Napi::Object>());
  if (!obj->line) {
    Napi::Error::New(env, "::requestInputMode() for line==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string consumer = info[0].As<Napi::String>().Utf8Value();
  if (-1 == gpiod_line_request_input(obj->getNativeLine(), consumer.c_str()))
    Napi::Error::New(env, "::requestInputMode() failed").ThrowAsJavaScriptException();
    return env.Null();
}

Napi::Value Line::requestOutputMode(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = Napi::ObjectWrap<Line>::Unwrap(info.This().As<Napi::Object>());
  if (!obj->line) {
    Napi::Error::New(env, "::requestOutputMode() for line==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  unsigned int value = 0;
  Napi::Value defaultValue = info[0];
  if (!defaultValue.IsUndefined() && defaultValue.IsNumber()) {
    unsigned int val = defaultValue.As<Napi::Number>().Uint32Value();
    if (val > 1) {
      Napi::Error::New(env, "::requestOutputMode() value is not in {0,1} range").ThrowAsJavaScriptException();
      return env.Null();
    }
    value = val;
  }
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, obj);
  std::string consumer = info[1].As<Napi::String>().Utf8Value();
  if (-1 == gpiod_line_request_output(obj->getNativeLine(), consumer.c_str(), value))
    Napi::Error::New(env, "::requestOutputMode() failed").ThrowAsJavaScriptException();
    return env.Null();
}

Napi::Value Line::release(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Line *obj = Napi::ObjectWrap<Line>::Unwrap(info.This().As<Napi::Object>());
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  if ( !obj->getNativeLine())     return env.Null();
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  gpiod_line_release(obj->getNativeLine());
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  obj->line = NULL;
}

gpiod_line *Line::getNativeLine() {
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, line);
  return line;
}