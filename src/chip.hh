#ifndef CHIP_HH
#define CHIP_HH

#include <gpiod.h>
#include <napi.h>
#include <uv.h>

#define USE_PRINTF 0

#if USE_PRINTF
#define DOUT(fmt,args...) printf(fmt,##args)
#else
#define DOUT(fmt,args...)
#endif

class Chip : public Napi::ObjectWrap<Chip> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Chip(const Napi::CallbackInfo& info);
  ~Chip();

  gpiod_chip* getNativeChip();

 private:
  static Napi::FunctionReference constructor;

  static Napi::Value getNumberOfLines(const Napi::CallbackInfo& info);
  static Napi::Value getChipName(const Napi::CallbackInfo& info);
  static Napi::Value getChipLabel(const Napi::CallbackInfo& info);

  gpiod_chip *chip;
};

#endif // CHIP_HH
