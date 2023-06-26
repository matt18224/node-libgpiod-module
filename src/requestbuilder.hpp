//
// Created by Matt on 6/25/2023.
//

#ifndef NODE_LIBGPIOD_REQUESTBUILDER_HPP
#define NODE_LIBGPIOD_REQUESTBUILDER_HPP

#include <gpiod.hpp>
#include <napi.h>

class RequestBuilder : public Napi::ObjectWrap<RequestBuilder>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    explicit RequestBuilder(const Napi::CallbackInfo &info);

    ~RequestBuilder() override;

    friend class Chip;

private:
    static Napi::FunctionReference constructor;

    Napi::Value AddLineSettings(const Napi::CallbackInfo &info);

    Napi::Value DoRequest(const Napi::CallbackInfo &info);

    std::unique_ptr<gpiod::request_builder> requestBuilderInstance;
};

#endif //NODE_LIBGPIOD_REQUESTBUILDER_HPP
