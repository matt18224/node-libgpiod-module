//
// Created by Matt on 6/25/2023.
//

#include "enumhelpers.hpp"

std::unordered_map<Value, std::string> valueToString{
    {Value::INACTIVE, "INACTIVE"},
    {Value::ACTIVE,   "ACTIVE"},
};

std::unordered_map<std::string, Value> stringToValue{
    {"INACTIVE", Value::INACTIVE},
    {"ACTIVE",   Value::ACTIVE},
};

std::unordered_map<Value, bool> valueToBoolMap{
    {Value::INACTIVE, false},
    {Value::ACTIVE,   true},
};

std::unordered_map<bool, Value> boolToValueMap{
    {false, Value::INACTIVE},
    {true,   Value::ACTIVE},
};

std::unordered_map<Direction, std::string> directionToString{
    {Direction::INPUT,  "INPUT"},
    {Direction::OUTPUT, "OUTPUT"},
    {Direction::AS_IS,  "AS_IS"},
};

std::unordered_map<std::string, Direction> stringToDirection{
    {"INPUT",  Direction::INPUT},
    {"OUTPUT", Direction::OUTPUT},
    {"AS_IS",  Direction::AS_IS},
};

std::string valueToName(const Napi::CallbackInfo &info, const Value v)
{
  if (valueToString.count(v) == 0)
  {
    Napi::Error::New(info.Env(),
                     "Invalid value enum").ThrowAsJavaScriptException();
  }
  return valueToString[v];
}

Value nameToValue(const Napi::CallbackInfo &info, const std::string &name)
{
  if (stringToValue.count(name) == 0)
  {
    Napi::Error::New(info.Env(),
                     "Invalid value name").ThrowAsJavaScriptException();
  }
  return stringToValue[name];
}

bool valueToBool(const Napi::CallbackInfo &info, const Value v)
{
  if (valueToBoolMap.count(v) == 0)
  {
    Napi::Error::New(info.Env(),
                     "Invalid value enum").ThrowAsJavaScriptException();
  }
  return valueToBoolMap[v];
}

Value boolToValue(const Napi::CallbackInfo &info, const bool &v)
{
  if (boolToValueMap.count(v) == 0)
  {
    Napi::Error::New(info.Env(),
                     "Invalid value name").ThrowAsJavaScriptException();
  }
  return boolToValueMap[v];
}

std::string directionToName(const Napi::CallbackInfo &info, const Direction v)
{
  if (directionToString.count(v) == 0)
  {
    Napi::Error::New(info.Env(),
                     "Invalid direction enum").ThrowAsJavaScriptException();
  }
  return directionToString[v];
}

Direction nameToDirection(const Napi::CallbackInfo &info, const std::string &name)
{
  if (stringToDirection.count(name) == 0)
  {
    Napi::Error::New(info.Env(),
                     "Invalid direction name").ThrowAsJavaScriptException();
  }
  return stringToDirection[name];
}