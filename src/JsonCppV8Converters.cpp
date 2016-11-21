#include "JsonCppV8Converters.h"

//====================================================================//
//============= Json::Value to [ v8::Array / v8::Object ] ============//
//====================================================================//

v8::Local<v8::Object> valueToObject(Json::Value & val) {
  Json::Value::Members members = val.getMemberNames();
  v8::Local<v8::Object> res = Nan::New<v8::Object>();

  unsigned int i = 0;
  for (auto it : members) {

    v8::Local<v8::String> key =
      Nan::New<v8::String>(it).ToLocalChecked();

    if (val[it].isBool()) {
      Nan::Set(res, key, Nan::New(val[it].asBool()));
    } else if (val[it].isInt()) {
      Nan::Set(res, key, Nan::New(val[it].asInt()));
    } else if (val[it].isDouble()) {
      Nan::Set(res, key, Nan::New(val[it].asDouble()));
    } else if (val[it].isString()) {
      Nan::Set(res, key, Nan::New<v8::String>(
                val[it].asString()
              ).ToLocalChecked()
      );
    } else if (val[it].isArray()) {
      Nan::Set(res, key, valueToArray(val[it]));
    } else if (val[it].isObject()) {
      Nan::Set(res, key, valueToObject(val[it]));
    }
    i++;  
  }
  return res;
}

v8::Local<v8::Array> valueToArray(Json::Value & val) {
  v8::Local<v8::Array> res = Nan::New<v8::Array>(val.size());

  unsigned int i = 0;
  for (auto it : val) {
    if (it.isBool()) {
      Nan::Set(res, i, Nan::New(it.asBool()));
    } else if (it.isInt()) {
      Nan::Set(res, i, Nan::New(it.asInt()));
    } else if (it.isDouble()) {
      Nan::Set(res, i, Nan::New(it.asDouble()));
    } else if (it.isString()) {
      Nan::Set(res, i, Nan::New<v8::String>(
                it.asString()
              ).ToLocalChecked()
      );
    } else if (it.isArray()) {
      Nan::Set(res, i, valueToArray(it));
    } else if (it.isObject()) {
      Nan::Set(res, i, valueToObject(it));
    }
    i++;  
  }
  return res;
}

//====================================================================//
//============= [ v8::Array / v8::Object ] to Json::Value ============//
//====================================================================//

Json::Value objectToValue(v8::Local<v8::Object> obj) {
  Json::Value res;

  v8::Local<v8::Array> keys = Nan::GetPropertyNames(obj).ToLocalChecked();

  std::size_t n = keys->Length();
  for (int i = 0; i < n; ++i) {
    v8::Local<v8::Value> key = keys->Get(i);
    v8::String::Utf8Value val(key->ToString());
    std::string skey = std::string(*val);

    v8::Local<v8::Value> prop = Nan::Get(obj, key).ToLocalChecked();

    if (prop->IsBoolean()) {
      res[skey] = prop->BooleanValue();
    } else if (prop->IsNumber()) {
      res[skey] = prop->NumberValue();
    // } else if (prop->IsInt32()) {
    //   res[skey] = prop->IntegerValue();
    // } else if (prop->IsNumber()) {
    //   Nan::Maybe<double> d = Nan::To<double>(prop);
    //   res[skey] = d.FromJust();
    } else if (prop->IsString()) {
      v8::String::Utf8Value val(prop->ToString());
      res[skey] = std::string(*val);
    } else if (prop->IsArray()) {
      res[skey] = arrayToValue(v8::Local<v8::Array>::Cast(prop));
      //res[skey] = arrayToValue(prop)
    } else if (prop->IsObject()) {
      res[skey] = objectToValue(v8::Local<v8::Object>::Cast(prop));
      //res[skey] = objectToValue(prop);
    }
  }
  return res;
}

Json::Value arrayToValue(v8::Local<v8::Array> arr) {
  Json::Value res;

  std::size_t n = arr->Length();
  res.resize(static_cast<Json::ArrayIndex>(n));

  for (int i = 0; i < n; ++i) {
    v8::Local<v8::Value> prop = arr->Get(i);

    if (prop->IsBoolean()) {
      res[i] = prop->BooleanValue();
    } else if (prop->IsNumber()) {
      res[i] = Json::Value(prop->NumberValue());
    // } else if (prop->IsInt32()) {
    //   res[i] = prop->IntegerValue();
    // } else if (prop->IsNumber()) {
    //   Nan::Maybe<double> d = Nan::To<double>(prop);
    //   res[i] = d.FromJust();
    } else if (prop->IsString()) {
      v8::String::Utf8Value val(prop->ToString());
      res[i] = std::string(*val);
    } else if (prop->IsArray()) {
      res[i] = arrayToValue(v8::Local<v8::Array>::Cast(prop));
      //res[skey] = arrayToValue(prop)
    } else if (prop->IsObject()) {
      res[i] = objectToValue(v8::Local<v8::Object>::Cast(prop));
      //res[skey] = objectToValue(prop);
    }
  }
  return res;
}
