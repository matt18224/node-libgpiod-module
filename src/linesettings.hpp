//
// Created by Matt on 6/24/2023.
//

#ifndef NODE_LIBGPIOD_LINESETTINGS_HPP
#define NODE_LIBGPIOD_LINESETTINGS_HPP

#include <gpiod.hpp>
#include <napi.h>

class LineSettings : public Napi::ObjectWrap<LineSettings> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit LineSettings(const Napi::CallbackInfo& info);
    ~LineSettings() override;

    friend class RequestBuilder;
    friend class LineConfig;

private:
    static Napi::FunctionReference constructor;
    Napi::Value GetDirection(const Napi::CallbackInfo& info);
    Napi::Value SetDirection(const Napi::CallbackInfo& info);
    Napi::Value Reset(const Napi::CallbackInfo& info);
//    Napi::Value SetEdgeDetection(const Napi::CallbackInfo& info);
//    Napi::Value GetEdgeDetection(const Napi::CallbackInfo& info);
//    Napi::Value SetBias(const Napi::CallbackInfo& info);
//    Napi::Value GetBias(const Napi::CallbackInfo& info);
//    Napi::Value SetDrive(const Napi::CallbackInfo& info);
//    Napi::Value GetDrive(const Napi::CallbackInfo& info);
//    Napi::Value SetActiveLow(const Napi::CallbackInfo& info);
//    Napi::Value GetActiveLow(const Napi::CallbackInfo& info);
//    Napi::Value SetDebouncePeriod(const Napi::CallbackInfo& info);
//    Napi::Value GetDebouncePeriod(const Napi::CallbackInfo& info);
//    Napi::Value SetEventClock(const Napi::CallbackInfo& info);
//    Napi::Value GetEventClock(const Napi::CallbackInfo& info);
    Napi::Value SetOutputValue(const Napi::CallbackInfo& info);
    Napi::Value GetOutputValue(const Napi::CallbackInfo& info);

    std::shared_ptr<gpiod::line_settings> lineSettingsInstance;
};

#endif //NODE_LIBGPIOD_LINESETTINGS_HPP
