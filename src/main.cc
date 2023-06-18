
#include "misc.hh"
#include "chip.hh"
#include "line.hh"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set("version", Napi::Function::New(env, version));
  exports.Set("getInstantLineValue", Napi::Function::New(env, getInstantLineValue));
  exports.Set("setInstantLineValue", Napi::Function::New(env, setInstantLineValue));
  exports.Set("usleep", Napi::Function::New(env, usleep));
  exports.Set("readBit", Napi::Function::New(env, readBit));

  Chip::Init(env, exports);
  Line::Init(env, exports);

  return exports;
}

NODE_API_MODULE(NativeExtension, InitAll)