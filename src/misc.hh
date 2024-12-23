#ifndef NODE_LIBGPIOD_HH
#define NODE_LIBGPIOD_HH

#include <gpiod.h>
#include <napi.h>
#include <unistd.h>

Napi::Value version(const Napi::CallbackInfo& info);
//Napi::Value getInstantLineValue(const Napi::CallbackInfo& info);
//Napi::Value setInstantLineValue(const Napi::CallbackInfo& info);
Napi::Value usleepWrapper(const Napi::CallbackInfo& info);
Napi::Value readBit(const Napi::CallbackInfo& info);
//Napi::Object readDHT11Reading(const Napi::CallbackInfo& info);
Napi::Value readLong(const Napi::CallbackInfo &info);

Napi::Object InitAll(Napi::Env env, Napi::Object exports);

#endif  // NODE_LIBGPIOD_HH
