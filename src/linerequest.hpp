//
// Created by Matt on 6/23/2023.
//

#ifndef NODE_LIBGPIOD_LINEREQUEST_HPP
#define NODE_LIBGPIOD_LINEREQUEST_HPP

#include <gpiod.h>
#include <gpiod.hpp>
#include <napi.h>

class LineRequest : public Napi::ObjectWrap<LineRequest>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    explicit LineRequest(const Napi::CallbackInfo &info);

    friend class RequestBuilder;

    struct LineRequestDestructor
    {
        void operator()(gpiod::line_request *l) const
        {
          l->release();
          delete l;
        }
    };

    std::unique_ptr<gpiod::line_request, LineRequestDestructor> lineRequestInstance;

private:
    static Napi::FunctionReference constructor;

    Napi::Value GetValue(const Napi::CallbackInfo &info);

    Napi::Value SetValue(const Napi::CallbackInfo &info);

    Napi::Value Release(const Napi::CallbackInfo &info);

    Napi::Value ReconfigureLines(const Napi::CallbackInfo &info);
};

#endif //NODE_LIBGPIOD_LINEREQUEST_HPP
