//
// Created by Matt on 6/25/2023.
//

#ifndef NODE_LIBGPIOD_ENUMHELPERS_HPP
#define NODE_LIBGPIOD_ENUMHELPERS_HPP

#include <unordered_map>
#include <string>
#include <napi.h>
#include <gpiod.hpp>

using Value = gpiod::line::value;
using Direction = gpiod::line::direction;
using Env = Napi::Env;

std::string valueToName(const Napi::CallbackInfo &info, const Value v);
Value nameToValue(const Napi::CallbackInfo &info, const std::string &name);
bool valueToBool(const Napi::CallbackInfo &info, const Value v);
Value boolToValue(const Napi::CallbackInfo &info, const bool &v);
std::string directionToName(const Napi::CallbackInfo &info, const Direction v);
Direction nameToDirection(const Napi::CallbackInfo &info, const std::string &name);




#endif //NODE_LIBGPIOD_ENUMHELPERS_HPP
