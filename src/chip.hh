#ifndef CHIP_HH
#define CHIP_HH

#include <gpiod.h>
#include <napi.h>
#include "gpiod.hpp"
#include "requestbuilder.hpp"

#define USE_PRINTF 0

#if USE_PRINTF
#define DOUT(fmt,args...) printf(fmt,##args)
#else
#define DOUT(fmt, args...)
#endif

#ifndef NAPI_CPP_EXCEPTIONS
#define NAPI_CPP_EXCEPTIONS 1
#endif

class Chip : public Napi::ObjectWrap<Chip>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    explicit Chip(const Napi::CallbackInfo &info);

    ~Chip() override;

    gpiod::chip *getNativeChip();

    gpiod::line_request requestLine(const Napi::CallbackInfo &info);

private:
    Napi::Value getNumberOfLines(const Napi::CallbackInfo &info);

    Napi::Value getChipName(const Napi::CallbackInfo &info);

    Napi::Value getChipLabel(const Napi::CallbackInfo &info);

    Napi::Value createRequest(const Napi::CallbackInfo &info);

    struct ChipDeleter
    {
        void operator()(gpiod::chip *c) const
        {
          c->close();
          delete c;
        }
    };

    std::unique_ptr<gpiod::chip, ChipDeleter> chip;
};

#endif // CHIP_HH
