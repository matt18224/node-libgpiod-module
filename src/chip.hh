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
  static Napi::Value CreateNewInstance(const Napi::CallbackInfo& info);
  Chip(const Napi::CallbackInfo& info);
  ~Chip();

  gpiod_chip* getNativeChip();

 private:
  Napi::Value getNumberOfLines(const Napi::CallbackInfo& info);
  Napi::Value getChipName(const Napi::CallbackInfo& info);
  Napi::Value getChipLabel(const Napi::CallbackInfo& info);

  gpiod_chip *chip;
};

#endif // CHIP_HH
