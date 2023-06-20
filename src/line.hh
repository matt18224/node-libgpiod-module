#ifndef LINE_HH
#define LINE_HH

#include <gpiod.h>
#include <napi.h>
#include <uv.h>

#include "chip.hh"

class Line : public Napi::ObjectWrap<Line> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Value New(const Napi::CallbackInfo& info);
  gpiod_line *getNativeLine();
  unsigned int getValueCpp();
  void setValueCpp(unsigned int value);

 private:
  explicit Line(Chip *chip, unsigned int pin);
  ~Line();

  static Napi::FunctionReference constructor;

  static Napi::Value getLineOffset(const Napi::CallbackInfo& info);
  static Napi::Value getLineName(const Napi::CallbackInfo& info);
  static Napi::Value getLineConsumer(const Napi::CallbackInfo& info);

  static Napi::Value getValue(const Napi::CallbackInfo& info);
  static Napi::Value setValue(const Napi::CallbackInfo& info);

  static Napi::Value requestInputMode(const Napi::CallbackInfo& info);
  static Napi::Value requestOutputMode(const Napi::CallbackInfo& info);
  static Napi::Value release(const Napi::CallbackInfo& info);

  static Napi::FunctionReference constructor;

  gpiod_line *line;
};

#endif  // LINE_HH
