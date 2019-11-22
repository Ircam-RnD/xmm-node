#ifndef _JSONCPP_NAPI_CONVERTERS_H_
#define _JSONCPP_NAPI_CONVERTERS_H_

#include <napi.h>
#include "../xmm/src/xmm.h"

Napi::Array valueToArray(Napi::Env env, Json::Value val);
Napi::Object valueToObject(Napi::Env env, Json::Value val);

Json::Value arrayToValue(Napi::Array arr);
Json::Value objectToValue(Napi::Object obj);

#endif /* _JSONCPP_NAPI_CONVERTERS_H_ */
