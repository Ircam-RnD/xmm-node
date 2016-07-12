#ifndef _XMM_NODE_TRAININGSET_H_
#define _XMM_NODE_TRAININGSET_H_

#include <nan.h>
#include "../xmm/src/xmm.h"

class trainingset : public Nan::ObjectWrap {
public:
	static void Init(v8::Local<v8::Object> exports);

private:
	explicit trainingset(bool bimodal);
	~trainingset();

	static void New 					(const Nan::FunctionCallbackInfo<v8::Value> & args);
	
	static void bimodal					(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void size					(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void empty					(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void addphrase				(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void removephrase			(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void removephrasesofclass	(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void getphrase				(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void getphrasesofclass		(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void clear					(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void labels					(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void tojsobject				(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void fromjsobject			(const Nan::FunctionCallbackInfo<v8::Value> & args);

	static void getdimension		(v8::Local<v8::String> prop,
									const Nan::PropertyCallbackInfo<v8::Value> & args);

	static void setdimension		(v8::Local<v8::String> prop,
									v8::Local<v8::Value> value,
									const Nan::PropertyCallbackInfo<void> & args);

	static void getdimensioninput 	(v8::Local<v8::String> prop,
									const Nan::PropertyCallbackInfo<v8::Value> & args);

	static void setdimensioninput	(v8::Local<v8::String> prop,
									v8::Local<v8::Value> value,
									const Nan::PropertyCallbackInfo<void> & args);

	static void getcolumnnames	 	(v8::Local<v8::String> prop,
									const Nan::PropertyCallbackInfo<v8::Value> & args);

	static void setcolumnnames		(v8::Local<v8::String> prop,
									v8::Local<v8::Value> value,
									const Nan::PropertyCallbackInfo<void> & args);

	static Nan::Persistent<v8::Function> constructor;
	xmm::TrainingSet trainingset_;
};

#endif // _XMM_NODE_TRAININGSET_H_