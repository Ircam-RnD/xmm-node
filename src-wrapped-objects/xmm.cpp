#include <node.h>
#include "../xmm/src/xmm.h"
//#include "gmm.h"
//#include "hhmm.h"
#include "phrase.h"
#include "trainingset.h"

/*
void Method(const v8::FunctionCallbackInfo<v8::Value> & args) {
	v8::Isolate *isolate = args.GetIsolate();
	args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "world"));
}

void Init(v8::Local<v8::Object> exports) {
	NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(xmm, Init)
*/

void InitAll(v8::Local<v8::Object> exports) {
	phrase::Init(exports);
	//phrase::Init();
	trainingset::Init(exports);
}

NODE_MODULE(xmm, InitAll)