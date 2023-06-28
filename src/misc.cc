#include "misc.hh"
#include "gpiod.hpp"
#include "linerequest.hpp"

Napi::Value usleepWrapper(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsNumber())
  {
    throw Napi::TypeError::New(env, "Wrong arguments");
    return env.Null();
  }

  int delay = info[0].As<Napi::Number>().Int32Value();
  usleep(delay);

  return Napi::String::New(env, "Delay done");
}

Napi::Value version(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  return Napi::String::New(env, gpiod::api_version());
}

/*Napi::Value getInstantLineValue(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  std::string device = info[0].As<Napi::String>().Utf8Value();
  unsigned int offset = info[1].As<Napi::Number>().Uint32Value();
  bool active_low = info[2].As<Napi::Boolean>().Value();
  std::string consumer = info[3].As<Napi::String>().Utf8Value();

  int value = -1;
  if (-1 == (value = gpiod::(device.c_str(), offset, active_low,
                                             consumer.c_str())))
  {
    throw Napi::Error::New.As<Napi::Object>()(env, "Unable to get instant value");
    return env.Null();
  }

  return Napi::Number::New(env, value);
}

Napi::Value setInstantLineValue(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  std::string device = info[0].As<Napi::String>().Utf8Value();
  unsigned int offset = info[1].As<Napi::Number>().Uint32Value();
  unsigned int value = info[2].As<Napi::Number>().Uint32Value();
  bool active_low = info[3].As<Napi::Boolean>().Value();
  std::string consumer = info[4].As<Napi::String>().Utf8Value();

  if (-1 ==
      gpiod_ctxless_set_value(device.c_str(), offset, value, active_low, consumer.c_str(),
                              NULL, NULL))
  {
    throw Napi::Error::New(env, "Unable to set instant value");
    return env.Null();
  }

  return Napi::Boolean::New(env, true);
}*/

/*
Napi::Object readDHT11Reading(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsObject())
  {
    throw Napi::TypeError::New(env, "Wrong arguments, expected line argument");
  }
  Line *dout = Napi::ObjectWrap<Line>::Unwrap(info[0].As<Napi::Object>());
}
*/

bool isReady(const LineRequest *lineRequest, const unsigned int doutOffset)
{
  return lineRequest->lineRequestInstance->get_value(doutOffset) ==
         gpiod::line::value::INACTIVE;
}

unsigned char readBitCpp(const LineRequest *lineRequest, const unsigned int pdSckOffset,
                         const unsigned int doutOffset)
{
  lineRequest->lineRequestInstance
             ->set_value(pdSckOffset, gpiod::line::value::ACTIVE);
  const auto bitValue = (unsigned char) (lineRequest->lineRequestInstance
                                                    ->get_value(doutOffset) ==
                                         gpiod::line::value::ACTIVE);
  lineRequest->lineRequestInstance->set_value(pdSckOffset,
                                              gpiod::line::value::INACTIVE);
  return bitValue;
}

unsigned char readByte(const LineRequest *lineRequest, const unsigned int pdSckOffset,
                       const unsigned int doutOffset)
{
  unsigned char byteValue = 0;
  for (int i = 0; i < 8; ++i)
  {
    byteValue <<= 1;
    byteValue |= readBitCpp(lineRequest, pdSckOffset, doutOffset);
  }
  return byteValue;
}

long readLongCpp(const LineRequest *lineRequest,
                 const unsigned int gain,
                 const unsigned int pdSckOffset,
                 const unsigned int doutOffset)
{
  while (!isReady(lineRequest, doutOffset))
  {
  }
  unsigned long value = 0;
  uint8_t data[3] = {0};
  uint8_t filler = 0x00;

  data[2] = readByte(lineRequest, pdSckOffset, doutOffset);
  data[1] = readByte(lineRequest, pdSckOffset, doutOffset);
  data[0] = readByte(lineRequest, pdSckOffset, doutOffset);

  for (unsigned int i = 0; i < gain; i++)
  {
    lineRequest->lineRequestInstance->set_value(pdSckOffset, gpiod::line::value::ACTIVE);
    lineRequest->lineRequestInstance
               ->set_value(pdSckOffset, gpiod::line::value::INACTIVE);
  }

  if (data[2] & 0x80)
  {
    filler = 0xFF;
  }
  else
  {
    filler = 0x00;
  }

  value = (static_cast<unsigned long>(filler) << 24
           | static_cast<unsigned long>(data[2]) << 16
           | static_cast<unsigned long>(data[1]) << 8
           | static_cast<unsigned long>(data[0]));

  return static_cast<long>(value);
}

Napi::Value readLong(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  LineRequest *lineRequest = Napi::ObjectWrap<LineRequest>::Unwrap(info[0]
                                                                       .As<Napi::Object>());
  unsigned int pdSckPinOffset = info[1].As<Napi::Number>();
  unsigned int doutPinOffset = info[2].As<Napi::Number>();
  unsigned int gain = info[3].As<Napi::Number>();

  return Napi::Number::New(env, (double) readLongCpp(lineRequest, gain, pdSckPinOffset,
                                                     doutPinOffset));
}

Napi::Value readBit(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  LineRequest *lineRequest = Napi::ObjectWrap<LineRequest>::Unwrap(info[0]
                                                                       .As<Napi::Object>());
  unsigned int pdSckPinOffset = info[1].As<Napi::Number>();
  unsigned int doutPinOffset = info[2].As<Napi::Number>();

  try
  {
    return Napi::Number::New(env, readBitCpp(lineRequest, pdSckPinOffset, doutPinOffset));
  }
  catch (const std::runtime_error &e)
  {
    throw Napi::Error::New(env, e.what());
  }
}
