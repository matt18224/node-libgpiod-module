#define NAPI_CPP_EXCEPTIONS 1

#include "misc.hh"
#include "chip.hh"
#include "lineconfig.hpp"
#include "linerequest.hpp"
#include "linesettings.hpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "version"), Napi::Function::New(env, version));
//  exports.Set(Napi::String::New(env, "getInstantLineValue"),
//              Napi::Function::New(env, getInstantLineValue));
//  exports.Set(Napi::String::New(env, "setInstantLineValue"),
//              Napi::Function::New(env, setInstantLineValue));
  exports.Set(Napi::String::New(env, "usleep"), Napi::Function::New(env, usleepWrapper));
//  exports.Set(Napi::String::New(env, "readBit"), Napi::Function::New(env, readBit));

  Chip::Init(env, exports);
  LineRequest::Init(env, exports);
  RequestBuilder::Init(env, exports);
  LineSettings::Init(env, exports);
//  LineConfig::Init(env, exports);

  return exports;
}

NODE_API_MODULE(NativeExtension, InitAll)