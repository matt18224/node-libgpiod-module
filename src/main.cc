
#include "misc.hh"
#include "chip.hh"
#include "line.hh"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "version"), Napi::Function::New(env, version));
  exports.Set(Napi::String::New(env, "getInstantLineValue"), Napi::Function::New(env, getInstantLineValue));
  exports.Set(Napi::String::New(env, "setInstantLineValue"), Napi::Function::New(env, setInstantLineValue));
  exports.Set(Napi::String::New(env, "usleep"), Napi::Function::New(env, usleepWrapper));
  exports.Set(Napi::String::New(env, "readBit"), Napi::Function::New(env, readBit));

  Chip::Init(env, exports);
  Line::Init(env, exports);

  return exports;
}

NODE_API_MODULE(NativeExtension, InitAll)