#include "gmm.h"

v8::Persistent<v8::Function> gmm::constructor;

gmm::gmm(bool bimodal) :
_gmm(bimodal) {

}

gmm::~gmm() {

}

void gmm::Init(v8::Local<v8::Object> exports) {
	v8::Isolate *isolate = exports->GetIsolate();

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
	tpl->SetClassName(v8::String::NewFromUtf8(isolate, "gmm"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// property getters / setters :
	// accessors :
	//tpl->SetAccessor(v8::String::NewFromUtf8(isolate, "gaussians"), GetGaussians, SetGaussians);
	// or interceptors (handle all properties) :
	//tpl->SetNamedPropertyHandler(GetProperty, SetProperty);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "train", 			train);
	NODE_SET_PROTOTYPE_METHOD(tpl, "training", 			training);
	NODE_SET_PROTOTYPE_METHOD(tpl, "canceltraining", 	canceltraining);
	NODE_SET_PROTOTYPE_METHOD(tpl, "reset", 			reset);
	NODE_SET_PROTOTYPE_METHOD(tpl, "filter", 			filter);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getresults", 		getresults);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getmodel", 			getmodels);
	NODE_SET_PROTOTYPE_METHOD(tpl, "setmodel", 			setmodels);

	constructor.Reset(isolate, tpl->GetFunction());
	exports->Set(String::NewFromUtf8(isolate, "gmm"),
               	tpl->GetFunction());
}

void gmm::New(const v8::FunctionCallbackInfo<v8::Value> & args) {
	v8::Isolate *isolate = args.GetIsolate();

	if(args.IsConstructCall()) {
		// ==================== invoked as constructor : new gmm(...)
		bool bimodal = (args[0]->IsUndefined() || !args[0]->IsBoolean())
						? false : args[0]->BooleanValue();

		// our internal class' instantiation :
		gmm *obj = new gmm(bimodal);
		
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
		// ==================== invoked as plain function : gmm(...) 
		//						turn it into constructor call
		const int argc = 1;
		v8::Local<v8::Value> argv[argc] = { args[0] };
		v8::Local<v8::Context> context = isolate->GetCurrentContext();
		v8::Local<v8::Function> cons =
			v8::Local<v8::Function::New(isolate, constructor);
		v8::Local<v8::Object> result = 
			cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.GetReturnValue().Set(result);
	}
}

void gmm::train(const v8::FunctionCallbackInfo<v8::Value> & args) {
	v8::Isolate *isolate = args.GetIsolate();

	gmm *obj = v8::ObjectWrap::Unwrap<gmm>(args.Holder());

}

