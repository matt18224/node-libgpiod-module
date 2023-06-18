
#include "misc.hh"
#include "chip.hh"
#include "line.hh"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  (target).Set(Napi::String::New(env, "version"),
           Napi::GetFunction(Napi::Function::New(env, version)));
  (target).Set(Napi::String::New(env, "getInstantLineValue"),
           Napi::GetFunction(Napi::Function::New(env, getInstantLineValue)));
  (target).Set(Napi::String::New(env, "setInstantLineValue"),
           Napi::GetFunction(Napi::Function::New(env, setInstantLineValue)));
  (target).Set(Napi::String::New(env, "usleep"),
           Napi::GetFunction(Napi::Function::New(env, usleep)));
  (target).Set(Napi::String::New(env, "readBit"),
           Napi::GetFunction(Napi::Function::New(env, readBit)));
  Chip::Init(env, target, module);
  Line::Init(env, target, module);
}

NODE_API_MODULE(NativeExtension, InitAll)