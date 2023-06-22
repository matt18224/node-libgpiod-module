#ifndef LINE_HH
#define LINE_HH

#include <gpiod.h>
#include <napi.h>
#include <uv.h>

#include "chip.hh"

class Line : public Napi::ObjectWrap<Line> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Value CreateNewInstance(const Napi::CallbackInfo& info);
  Line(const Napi::CallbackInfo& info);
  ~Line();

  gpiod_line *getNativeLine();
  unsigned int getValueCpp();
  void setValueCpp(unsigned int value);

 private:
  Napi::Value getLineOffset(const Napi::CallbackInfo& info);
  Napi::Value getLineName(const Napi::CallbackInfo& info);
  Napi::Value getLineConsumer(const Napi::CallbackInfo& info);

  Napi::Value getValue(const Napi::CallbackInfo& info);
  Napi::Value setValue(const Napi::CallbackInfo& info);

  Napi::Value requestInputMode(const Napi::CallbackInfo& info);
  Napi::Value requestOutputMode(const Napi::CallbackInfo& info);
  Napi::Value release(const Napi::CallbackInfo& info);

  gpiod_line *line;
};

#endif  // LINE_HH
