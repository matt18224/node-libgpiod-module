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
  gpiod_chip* getNativeChip();

 private:
  explicit Chip(const char* device = "0");
  ~Chip();

  static Napi::Value getNumberOfLines(const Napi::CallbackInfo& info);
  static Napi::Value getChipName(const Napi::CallbackInfo& info);
  static Napi::Value getChipLabel(const Napi::CallbackInfo& info);

  static Napi::Value New(const Napi::CallbackInfo& info);
  static Napi::FunctionReference constructor;

  gpiod_chip *chip;
};

#endif // CHIP_HH
