#ifndef _XMM_NODE_GMM_H_
#define _XMM_NODE_GMM_H_

#include <node.h>
#include <node_object_wrap.h>
#include "../xmm/src/xmm.h"

class gmm : public node::ObjectWrap {
public:
	static void Init(v8::Local<v8::Object> exports);

private:
	explicit gmm(bool bimodal);
	~gmm();

	// args : bimodal ?
	static void New 			(const v8::FunctionCallbackInfo<v8::Value> & args);

	static void train 			(const v8::FunctionCallbackInfo<v8::Value> & args);
	static void training 		(const v8::FunctionCallbackInfo<v8::Value> & args);
	static void canceltraining	(const v8::FunctionCallbackInfo<v8::Value> & args);
	static void reset			(const v8::FunctionCallbackInfo<v8::Value> & args);
	static void filter			(const v8::FunctionCallbackInfo<v8::Value> & args);
	static void getresults		(const v8::FunctionCallbackInfo<v8::Value> & args);
	static void setmodel		(const v8::FunctionCallbackInfo<v8::Value> & args);
	static void getmodel		(const v8::FunctionCallbackInfo<v8::Value> & args);

	static v8::Persistent<v8::Function> constructor;
	xmm::GMM _gmm;
};



#endif // _XMM_NODE_GMM_H_