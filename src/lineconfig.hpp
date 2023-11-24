//
// Created by Matt on 6/25/2023.
//

#ifndef NODE_LIBGPIOD_LINECONFIG_HPP
#define NODE_LIBGPIOD_LINECONFIG_HPP

#include <gpiod.hpp>
#include <napi.h>

class LineConfig : public Napi::ObjectWrap<LineConfig>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit LineConfig(const Napi::CallbackInfo &info);
    ~LineConfig() override;
    Napi::Value AddLineSetting(const Napi::CallbackInfo& info);
    Napi::Value Reset(const Napi::CallbackInfo& info);

    friend class LineRequest;

private:
    static Napi::FunctionReference constructor;

    std::unique_ptr<gpiod::line_config> lineConfigInstance;
};

#endif //NODE_LIBGPIOD_LINECONFIG_HPP
