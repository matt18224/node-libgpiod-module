#include "misc.hh"

Napi::Value usleepWrapper(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  int delay = info[0].As<Napi::Number>().Int32Value();
  usleep(delay);

  return Napi::String::New(env, "Delay done");
}

Napi::Value version(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, gpiod_version_string());
}

Napi::Value getInstantLineValue(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  std::string device = info[0].As<Napi::String>().Utf8Value();
  unsigned int offset = info[1].As<Napi::Number>().Uint32Value();
  bool active_low = info[2].As<Napi::Boolean>().Value();
  std::string consumer = info[3].As<Napi::String>().Utf8Value();

  int value = -1;
  if (-1 == (value = gpiod_ctxless_get_value(device.c_str(), offset, active_low, consumer.c_str()))) {
    Napi::Error::New(env, "Unable to get instant value").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Number::New(env, value);
}

Napi::Value setInstantLineValue(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  std::string device = info[0].As<Napi::String>().Utf8Value();
  unsigned int offset = info[1].As<Napi::Number>().Uint32Value();
  unsigned int value = info[2].As<Napi::Number>().Uint32Value();
  bool active_low = info[3].As<Napi::Boolean>().Value();
  std::string consumer = info[4].As<Napi::String>().Utf8Value();

  if (-1 == gpiod_ctxless_set_value(device.c_str(), offset, value, active_low, consumer.c_str(), NULL, NULL)) {
    Napi::Error::New(env, "Unable to set instant value").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::New(env, true);
}

Napi::Value readBit(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsObject()) {
    Napi::TypeError::New(env, "Wrong arguments, expected two Line instances").ThrowAsJavaScriptException();
    return env.Null();
  }

  Line* pdSck = Napi::ObjectWrap<Line>::Unwrap(info[0].As<Napi::Object>());
  Line* dout = Napi::ObjectWrap<Line>::Unwrap(info[1].As<Napi::Object>());

  if (!pdSck || !dout) {
    Napi::Error::New(env, "Could not unwrap Line object").ThrowAsJavaScriptException();
    return env.Null();
  }

  try {
    pdSck->setValueCpp(1);
    // usleep(1);
    unsigned int bitValue = dout->getValueCpp();
    pdSck->setValueCpp(0);

    return Napi::Number::New(env, bitValue);
  } catch (const std::runtime_error& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}