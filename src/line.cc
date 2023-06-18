#include "line.hh"

Napi::FunctionReference Line::constructor;

Napi::Object Line::Init(Napi::Env env, Napi::Object exports) {
  Napi::FunctionReference tpl = Napi::Function::New(env, New);
  tpl->SetClassName(Napi::String::New(env, "Line"));


  InstanceMethod("getLineOffset", &getLineOffset),
  InstanceMethod("getLineName", &getLineName),
  InstanceMethod("getLineConsumer", &getLineConsumer),
  InstanceMethod("getValue", &getValue),
  InstanceMethod("setValue", &setValue),
  InstanceMethod("requestInputMode", &requestInputMode),
  InstanceMethod("requestOutputMode", &requestOutputMode),
  InstanceMethod("release", &release),

  constructor.Reset(Napi::GetFunction(tpl));
  (target).Set(Napi::String::New(env, "Line"), Napi::GetFunction(tpl));
}

Line::Line(Chip *chip, unsigned int pin) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  line = gpiod_chip_get_line(chip->getNativeChip(), pin);
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, line);
  if (!line) Napi::Error::New(env, "Unable to open GPIO line ").ThrowAsJavaScriptException();

}

Line::~Line() {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  if ( !line) return;
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  gpiod_line_close_chip(line);
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  line = NULL;
}

Napi::Value Line::New(const Napi::CallbackInfo& info) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  if (info.IsConstructCall()) {
    DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
    Chip *chip = info[0].To<Napi::Object>().Unwrap<Chip>();
    unsigned int pin = Napi::To<unsigned int>(info[1]);
    Line *obj = new Line(chip, pin);
    DOUT( "%s %s(%d):%d %p\n", __FILE__, __FUNCTION__, pin, __LINE__, obj);
    if ( !obj->line) return;
    DOUT( "%s %s(%d):%d %p->%p\n", __FILE__, __FUNCTION__, pin, __LINE__, obj, obj->line);
    obj->Wrap(info.This());
    return info.This();
  } else {
    const int argc = 1;
    DOUT( "%s %s():%d !construct\n", __FILE__, __FUNCTION__, __LINE__);
    Napi::Value argv[argc] = {info[0]};
    Napi::Function cons = Napi::New(env, constructor);
    return Napi::NewInstance(cons, argc, argv);
  }
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
}

Napi::Value Line::getLineOffset(const Napi::CallbackInfo& info) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = info.This().Unwrap<Line>();
  if ( !obj->line) {
    Napi::Error::New(env,  "::getLineOffset() for line==NULL").ThrowAsJavaScriptException();

    return env.Null();
  }
  int ret = gpiod_line_offset(obj->getNativeLine());
  if(-1 == ret) {
    Napi::Error::New(env,  "::getLineOffset() failed").ThrowAsJavaScriptException();
    return env.Null();
  } else return ret;
}

Napi::Value Line::getLineName(const Napi::CallbackInfo& info) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = info.This().Unwrap<Line>();
  if ( !obj->line) {
    Napi::Error::New(env,  "::getLineName() for line==NULL").ThrowAsJavaScriptException();
    return env.Null();
  }
  const char *name = gpiod_line_name(obj->getNativeLine());
  if(!name) return env.Undefined();
  else return Napi::String::New(env, name);
}

Napi::Value Line::getLineConsumer(const Napi::CallbackInfo& info) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = info.This().Unwrap<Line>();
  if ( !obj->line) {
    Napi::Error::New(env,  "::getLineConsumer() for line==NULL").ThrowAsJavaScriptException();
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
  Line *obj = info.This().Unwrap<Line>();
  try {
    unsigned int ret = obj->getValueCpp();
    return ret;
  } catch (const std::runtime_error& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();

  }
}

Napi::Value Line::setValue(const Napi::CallbackInfo& info) {
  Line *obj = info.This().Unwrap<Line>();
  unsigned int value = Napi::To<unsigned int>(info[0]);
  try {
    obj->setValueCpp(value);
  } catch (const std::runtime_error& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();

  }
}

Napi::Value Line::requestInputMode(const Napi::CallbackInfo& info) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = info.This().Unwrap<Line>();
  if (!obj->line) {
    Napi::Error::New(env,  "::requestInputMode() for line==NULL").ThrowAsJavaScriptException();

        return env.Null();
  }
  std::string consumer = info[0].As<Napi::String>();
  if (-1 == gpiod_line_request_input(obj->getNativeLine(), *consumer))
    Napi::Error::New(env,  "::requestInputMode() failed").ThrowAsJavaScriptException();

}

Napi::Value Line::requestOutputMode(const Napi::CallbackInfo& info) {
  DOUT( "%s %s():%d\n", __FILE__, __FUNCTION__, __LINE__);
  Line *obj = info.This().Unwrap<Line>();
  if (!obj->line) {
      Napi::Error::New(env,  "::requestOutputMode() for line==NULL").ThrowAsJavaScriptException();

          return env.Null();
    }
  unsigned int value = 0;
  Napi::Value defaultValue = info[0];
  if (!defaultValue->IsUndefined() && defaultValue.IsNumber()) {
    unsigned int val = Napi::To<unsigned int>(defaultValue);
    if (val > 1) {
      Napi::Error::New(env,  "::requestOutputMode() value is not in {0,1} range").ThrowAsJavaScriptException();

          return env.Null();
    }
    value = val;
  }
  DOUT( "%s %s():%d %p\n", __FILE__, __FUNCTION__, __LINE__, obj);
  std::string consumer = info[1].As<Napi::String>();
  if (-1 == gpiod_line_request_output(obj->getNativeLine(), *consumer, value))
    Napi::Error::New(env,  "::requestOutputMode() failed").ThrowAsJavaScriptException();

}


Napi::Value Line::release(const Napi::CallbackInfo& info) {
  Line *obj = info.This().Unwrap<Line>();
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
