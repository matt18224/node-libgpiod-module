//
// Created by Matt on 6/25/2023.
//

#ifndef NODE_LIBGPIOD_LINECONFIG_HPP
#define NODE_LIBGPIOD_LINECONFIG_HPP

#include <gpiod.hpp>
#include <napi.h>
#include "linesettings.hpp"

class LineConfig : public Napi::ObjectWrap<LineConfig>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit LineConfig(const Napi::CallbackInfo &info);
    explicit LineConfig(const Napi::CallbackInfo &info, const gpiod::line_config &config);
    ~LineConfig() override;
    Napi::Value AddLineSetting(const Napi::CallbackInfo& info);
    Napi::Value Reset(const Napi::CallbackInfo& info);

    static Napi::Value CreateFromLiteral(const Napi::CallbackInfo &info);

    friend class LineRequest;
    friend class RequestBuilder;

private:
    static Napi::FunctionReference constructor;

    void privateAddLineSetting(gpiod::line::offset offset,
                               const LineSettings *lineSettings);
    std::unique_ptr<gpiod::line_config> lineConfigInstance;
};

#endif //NODE_LIBGPIOD_LINECONFIG_HPP
