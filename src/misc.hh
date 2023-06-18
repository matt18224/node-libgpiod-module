#ifndef NODE_LIBGPIOD_HH
#define NODE_LIBGPIOD_HH

#include <iostream>

#include <gpiod.h>
#include <napi.h>
#include <uv.h>
#include <unistd.h>
#include "line.hh"

Napi::Value version(const Napi::CallbackInfo& info);
Napi::Value getInstantLineValue(const Napi::CallbackInfo& info);
Napi::Value setInstantLineValue(const Napi::CallbackInfo& info);
Napi::Value usleepWrapper(const Napi::CallbackInfo& info);
Napi::Value readBit(const Napi::CallbackInfo& info);

Napi::Object InitAll(Napi::Env env, Napi::Object exports);

#endif  // NODE_LIBGPIOD_HH