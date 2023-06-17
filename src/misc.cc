#include "misc.hh"

NAN_METHOD(usleep) {
  if (info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  int delay = info[0]->NumberValue(Nan::GetCurrentContext()).FromJust();
  usleep(delay);

  info.GetReturnValue().Set(Nan::New("Delay done").ToLocalChecked());
}

NAN_METHOD(version) {
  info.GetReturnValue()
      .Set(Nan::New<v8::String>(gpiod_version_string())
               .ToLocalChecked());
}

NAN_METHOD(getInstantLineValue) {
  Nan::Utf8String device(info[0]);
  unsigned int offset = Nan::To<unsigned int>(info[1]).FromJust();
  bool active_low = Nan::To<bool>(info[2]).FromJust();
  Nan::Utf8String consumer(info[3]);

  int value = -1;
  if (-1 == (value = gpiod_ctxless_get_value(*device, offset, active_low, *consumer))) {
    Nan::ThrowError("Unable to get instant value");
    return;
  }

  info.GetReturnValue().Set(Nan::New<v8::Integer>(value));
}

NAN_METHOD(setInstantLineValue) {
  Nan::Utf8String device(info[0]);
  unsigned int offset = Nan::To<unsigned int>(info[1]).FromJust();
  unsigned int value = Nan::To<unsigned int>(info[2]).FromJust();
  bool active_low = Nan::To<bool>(info[3]).FromJust();
  Nan::Utf8String consumer(info[4]);

  if (-1 == gpiod_ctxless_set_value(*device, offset, value, active_low, *consumer, NULL, NULL)) {
    Nan::ThrowError("Unable to get instant value");
    return;
  }

  info.GetReturnValue().Set(true);
}

// Assumes Line class is defined and included.

NAN_METHOD(readBit) {
  if (info.Length() < 2 || !info[0]->IsObject() || !info[1]->IsObject()) {
    Nan::ThrowTypeError("Wrong arguments, expected two Line instances");
    return;
  }

  Line *pdSck = Nan::ObjectWrap::Unwrap<Line>(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());
  Line *dout = Nan::ObjectWrap::Unwrap<Line>(info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked());

  if (!pdSck || !dout) {
    Nan::ThrowError("Could not unwrap Line object");
    return;
  }

  // pdSck.setValue(1)
  v8::Local<v8::Value> setValueArgs1[] = { Nan::New(1) };
  Nan::MakeCallback(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked(), "setValue", 1, setValueArgs1);

  // usleep(1)
  usleep(1);

  // bitValue = dout.getValue()
  v8::Local<v8::Value> bitValue = Nan::MakeCallback(info[1]->ToObject(Nan::GetCurrentContext()).ToLocalChecked(), "getValue", 0, nullptr);
  if (!bitValue->IsNumber()) {
    Nan::ThrowError("bitValue is not a number");
    return;
  }

  // pdSck.setValue(0)
  v8::Local<v8::Value> setValueArgs0[] = { Nan::New(0) };
  Nan::MakeCallback(info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked(), "setValue", 1, setValueArgs0);

  info.GetReturnValue().Set(bitValue);
}