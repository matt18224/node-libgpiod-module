#include <memory>
#include <string>
#include <stdexcept>
#include <set>

#ifndef NODE_ADDON_API_ENABLE_TYPE_CHECK_ON_AS
#define NODE_ADDON_API_ENABLE_TYPE_CHECK_ON_AS 1
#endif

#ifndef NODE_LIBGPIOD_UTILS_HPP
#define NODE_LIBGPIOD_UTILS_HPP

template<typename ... Args>
std::string string_format(const std::string &format, Args ... args)
{
  int size_s =
      std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
  if (size_s <= 0)
  { throw std::runtime_error("Error during formatting."); }
  auto size = static_cast<size_t>( size_s );
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args ...);
  return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

typedef std::function<bool(const Napi::Value &)> Validator;

void checkArgTypes(const Napi::CallbackInfo &info,
                   const std::vector<Validator> &validators)
{
  Napi::Env env = info.Env();

  if (info.Length() != validators.size())
  {
    std::string errorMsg =
        "Expected " + std::to_string(validators.size()) + " arguments, but got " +
        std::to_string(info.Length());
    throw Napi::TypeError::New(env, errorMsg);
  }

  for (size_t i = 0; i < validators.size(); ++i)
  {
    if (!validators[i](info[i]))
    {
      throw Napi::TypeError::New(env, "Argument at index " + std::to_string(i) +
                                      " is of incorrect type");
    }
  }
}

//Validator isIntInClosedRange(const int start, const int end)
//{
//  return [start, end](const Napi::Value &value) {
//      bool isNum = value.IsNumber();
//      if (isNum)
//      {
//        int num = value.As<Napi::Number>();
//        return num >= start && num <= end;
//      }
//      return false;
//  };
//}
//
//Validator isString()
//{
//  return [](const Napi::Value &value) {
//      return value.IsString();
//  };
//}

//Validator isInSet(const std::set<std::string> &validValues)
//{
//  return [validValues](const Napi::Value &value) {
//      return value.IsString() && validValues.contains(value.As<Napi::String>());
//  };
//}

Validator intersection(std::initializer_list<Validator> validators) {
  return [validators](const Napi::Value& value) -> bool {
      for (const auto& validator : validators) {
        if (!validator(value)) {
          return false;
        }
      }
      return true;
  };
}

Validator union_(std::initializer_list<Validator> validators) {
  return [validators](const Napi::Value& value) -> bool {
      for (const auto& validator : validators) {
        if (validator(value)) {
          return true;
        }
      }
      return false;
  };
}

#endif //NODE_LIBGPIOD_UTILS_HPP