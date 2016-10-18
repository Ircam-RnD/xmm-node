#ifndef _JSONCPPV8CONVERTERS_H_
#define _JSONCPPV8CONVERTERS_H_

#include <nan.h>
#include "../xmm/src/xmm.h"

v8::Local<v8::Array> valueToArray(Json::Value & val);
v8::Local<v8::Object> valueToObject(Json::Value & val);

Json::Value arrayToValue(v8::Local<v8::Array> arr);
Json::Value objectToValue(v8::Local<v8::Object> obj);

#endif /* _JSONCPPV8CONVERTERS_H_ */
