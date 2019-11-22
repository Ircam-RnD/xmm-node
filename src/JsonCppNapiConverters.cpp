#include "JsonCppNapiConverters.h"

// placeholders. do nothing atm

Napi::Array valueToArray(Napi::Env env, Json::Value val) {
  Napi::Array res = Napi::Array::New(env, val.size());

  unsigned int i = 0;
  for (auto it : val) {
    if (it.isBool()) {
      res.Set(i, Napi::Boolean::New(env, it.asBool()));
    } else if (it.isInt()) {
      res.Set(i, Napi::Number::New(env, static_cast<int64_t>(it.asInt())));
    } else if (it.isDouble()) {
      res.Set(i, Napi::Number::New(env, it.asDouble()));
    } else if (it.isString()) {
      res.Set(i, Napi::String::New(env, it.asString()));
    } else if (it.isArray()) {
      res.Set(i, valueToArray(env, it));
    } else if (it.isObject()) {
      res.Set(i, valueToObject(env, it));
    }
    i++;  
  }
  return res;
}

Napi::Object valueToObject(Napi::Env env, Json::Value val) {
  Json::Value::Members members = val.getMemberNames();
  Napi::Object res = Napi::Object::New(env);

  unsigned int i = 0;
  for (auto it : members) {
    Napi::String key = Napi::String::New(env, it);

    if (val[it].isBool()) {
      res.Set(key, Napi::Boolean::New(env, val[it].asBool()));
    } else if (val[it].isInt()) {
      res.Set(key, Napi::Number::New(env, val[it].asInt()));
    } else if (val[it].isDouble()) {
      res.Set(key, Napi::Number::New(env, val[it].asDouble()));
    } else if (val[it].isString()) {
      res.Set(key, Napi::String::New(env, val[it].asString()));
    } else if (val[it].isArray()) {
      res.Set(key, valueToArray(env, val[it]));
    } else if (val[it].isObject()) {
      res.Set(key, valueToObject(env, val[it]));
    }
    i++;  
  }
  return res;
}

Json::Value arrayToValue(Napi::Array arr) {
  Json::Value res;

  std::size_t n = arr.Length();
  res.resize(static_cast<Json::ArrayIndex>(n));

  for (int i = 0; i < n; ++i) {
    Napi::Value val = arr.Get(i);

    if (val.IsBoolean()) {
      res[i] = val.ToBoolean().Value();
    } else if (val.IsNumber()) {
      res[i] = Json::Value(val.ToNumber().DoubleValue());
    } else if (val.IsString()) {
      res[i] = std::string(val.ToString());
    } else if (val.IsArray()) {
      res[i] = arrayToValue(val.As<Napi::Array>());
    } else if (val.IsObject()) {
      res[i] = objectToValue(val.As<Napi::Object>());
    }
  }
  return res;
}

Json::Value objectToValue(Napi::Object obj) {
  Json::Value res;

  Napi::Array keys = obj.GetPropertyNames();
  std::size_t n = keys.Length();

  for (int i = 0; i < n; ++i) {
    std::string key = std::string(keys.Get(i).As<Napi::String>());
    Napi::Value val = obj.Get(keys.Get(i));

    if (val.IsBoolean()) {
      res[key] = val.ToBoolean().Value();
    } else if (val.IsNumber()) {
      res[key] = val.ToNumber().DoubleValue();
    } else if (val.IsString()) {
      res[key] = std::string(val.ToString());
    } else if (val.IsArray()) {
      res[key] = arrayToValue(val.As<Napi::Array>());
    } else if (val.IsObject()) {
      res[key] = objectToValue(val.As<Napi::Object>());
    }
  }
  return res;
}
