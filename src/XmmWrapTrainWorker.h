#include <nan.h>
#include "XmmWrap.h"

template<typename Model>
class XmmWrapTrainWorker : public Nan::AsyncWorker {
public:
	XmmWrapTrainWorker(Nan::Callback *callback, XmmTool<Model>& t, xmm::TrainingSet s) :
	Nan::AsyncWorker(callback), original(t),tool(t), set(s) {}

	~XmmWrapTrainWorker() {}

	void Execute() {
		tool.train(&set);
		while(tool.training());
	}

	void HandleOKCallback() {
		Nan::HandleScope scope;

		v8::Local<v8::Object> model = Nan::New<v8::Object>();
		// construct the object with the trained model
		Nan::Set(model, Nan::New<v8::String>("trained").ToLocalChecked(),
			Nan::New(true));

		v8::Local<v8::Value> results[] = {
			//v8::Local<v8::Value>::Cast(model)
			Nan::Null(),
			model
		};

		// this should be safe to do this, as HandleOKCallback is called
		// in the event loop, and threaded training is disabled in XmmTool
		original = tool;

		callback->Call(2, results);
	}

private:
	XmmTool<Model>& original;
	XmmTool<Model> tool;
	xmm::TrainingSet set;
};