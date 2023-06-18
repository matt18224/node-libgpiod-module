#include "misc.hh"

Napi::Value usleepWrapper(const Napi::CallbackInfo& info) {
  if (info.Length() < 1 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();

    return env.Null();
  }

  int delay = info[0].As<Napi::Number>().Int32Value();
  usleep(delay);

  return Napi::String::New(env, "Delay done");
}

Napi::Value version(const Napi::CallbackInfo& info) {
  return Napi::String::New(env, gpiod_version_string());
}

Napi::Value getInstantLineValue(const Napi::CallbackInfo& info) {
  std::string device = info[0].As<Napi::String>();
  unsigned int offset = Napi::To<unsigned int>(info[1]);
  bool active_low = info[2].As<Napi::Boolean>().Value();
  std::string consumer = info[3].As<Napi::String>();

  int value = -1;
  if (-1 == (value = gpiod_ctxless_get_value(*device, offset, active_low, *consumer))) {
    Napi::Error::New(env, "Unable to get instant value").ThrowAsJavaScriptException();

    return env.Null();
  }

  return Napi::Number::New(env, value);
}

Napi::Value setInstantLineValue(const Napi::CallbackInfo& info) {
  std::string device = info[0].As<Napi::String>();
  unsigned int offset = Napi::To<unsigned int>(info[1]);
  unsigned int value = Napi::To<unsigned int>(info[2]);
  bool active_low = info[3].As<Napi::Boolean>().Value();
  std::string consumer = info[4].As<Napi::String>();

  if (-1 == gpiod_ctxless_set_value(*device, offset, value, active_low, *consumer, NULL, NULL)) {
    Napi::Error::New(env, "Unable to get instant value").ThrowAsJavaScriptException();

    return env.Null();
  }

  return Napi::Boolean::New(env, true);
}

// Assumes Line class is defined and included.

Napi::Value readBit(const Napi::CallbackInfo& info) {
  if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsObject()) {
    Napi::TypeError::New(env, "Wrong arguments, expected two Line instances").ThrowAsJavaScriptException();

    return env.Null();
  }

  Line *pdSck = info[0].ToObject(Napi::GetCurrentContext()).Unwrap<Line>();
  Line *dout = info[1].ToObject(Napi::GetCurrentContext()).Unwrap<Line>();

  if (!pdSck || !dout) {
    Napi::Error::New(env, "Could not unwrap Line object").ThrowAsJavaScriptException();

    return env.Null();
  }

  try {
    // pdSck.setValueCpp(1)
    pdSck->setValueCpp(1);

    // usleep(1)
//     usleep(1);

    // bitValue = dout.getValueCpp()
    unsigned int bitValue = dout->getValueCpp();

    // pdSck.setValueCpp(0)
    pdSck->setValueCpp(0);

    return bitValue;
  } catch (const std::runtime_error& e) {
    Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
    return env.Null();
  }
}