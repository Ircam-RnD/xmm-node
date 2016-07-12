#ifndef _XMM_NODE_PHRASE_H_
#define _XMM_NODE_PHRASE_H_

#include <nan.h>
#include "../xmm/src/xmm.h"

class phrase : public Nan::ObjectWrap {
public:
	static void Init(v8::Local<v8::Object> exports);
	//static void Init();
	static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
	xmm::Phrase *GetVal() const { return phrase_; }
	void SetVal(xmm::Phrase xp) { delete phrase_; phrase_ = new xmm::Phrase(xp); }
	
	//xmm::Phrase& Val() { return phrase_; }
	//std::shared_ptr<xmm::Phrase> GetVal() const { return phrase_; }
	//void SetVal(xmm::Phrase p) { phrase_ = std::make_shared<xmm::Phrase>(p); }

private:
	explicit phrase(bool bimodal = false);
	~phrase();

	static void New 			(const Nan::FunctionCallbackInfo<v8::Value> & args);
	
	static void bimodal			(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void size			(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void inputsize		(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void outputsize		(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void empty			(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void record 			(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void recordinput		(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void recordoutput	(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void clear			(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void clearinput		(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void clearoutput		(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void tojsobject		(const Nan::FunctionCallbackInfo<v8::Value> & args);
	static void fromjsobject	(const Nan::FunctionCallbackInfo<v8::Value> & args);

	// + properties : dimension, dimension_input, label, column_names
	// as described in :
	// https://developers.google.com/v8/embed#accessing-dynamic-variables
	/*
	static void GetProperty		(v8::Local<v8::String> name,
								const v8::PropertyCallbackInfo<v8::Value> & info);
	static void SetProperty		(v8::Local<v8::String> name,
								v8::Local<v8::Value> valobj,
								const v8::PropertyCallbackInfo<v8::Value> & info);
	//*/

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

	static void getlabel		 	(v8::Local<v8::String> prop,
									const Nan::PropertyCallbackInfo<v8::Value> & args);

	static void setlabel			(v8::Local<v8::String> prop,
									v8::Local<v8::Value> value,
									const Nan::PropertyCallbackInfo<void> & args);

	static void getcolumnnames	 	(v8::Local<v8::String> prop,
									const Nan::PropertyCallbackInfo<v8::Value> & args);

	static void setcolumnnames		(v8::Local<v8::String> prop,
									v8::Local<v8::Value> value,
									const Nan::PropertyCallbackInfo<void> & args);

	// static v8::Persistent<v8::Function> constructor;
	static Nan::Persistent<v8::Function> constructor;
	xmm::Phrase *phrase_;
	//std::shared_ptr<xmm::Phrase> phrase_;
};

#endif // _XMM_NODE_PHRASE_H_