#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
  int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
  if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
  auto size = static_cast<size_t>( size_s );
  std::unique_ptr<char[]> buf( new char[ size ] );
  std::snprintf( buf.get(), size, format.c_str(), args ... );
  return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

typedef std::function<bool(const Napi::Value&)> Validator;

void checkArgTypes(const Napi::CallbackInfo& info, const std::vector<Validator>& validators) {
  Napi::Env env = info.Env();

  if (info.Length() != validators.size()) {
    std::string errorMsg = "Expected " + std::to_string(validators.size()) + " arguments, but got " + std::to_string(info.Length());
    Napi::TypeError::New(env, errorMsg).ThrowAsJavaScriptException();
    return;
  }

  for (size_t i = 0; i < validators.size(); ++i) {
    if (!validators[i](info[i])) {
      Napi::TypeError::New(env, "Argument at index " + std::to_string(i) + " is of incorrect type").ThrowAsJavaScriptException();
      return;
    }
  }
}