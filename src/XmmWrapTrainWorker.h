#ifndef _XMMWRAPTRAINWORKER_H_
#define _XMMWRAPTRAINWORKER_H_

#include <nan.h>
// #include "XmmWrap.h"
#include "../xmm/src/xmm.h"
#include "JsonCppV8Converters.h"

template<typename Model>
class XmmWrapTrainWorker : public Nan::AsyncWorker {
public:
  XmmWrapTrainWorker(Nan::Callback *callback, Model& t, xmm::TrainingSet *s) :
  Nan::AsyncWorker(callback), original(t), tool(t), set(*s), cancel(false) {}

  ~XmmWrapTrainWorker() {}

  void Execute() {
    tool.train(&set);
    while(tool.training() && !cancel) {
      // leave loop if training ended OR user cancelled training
    }
  }

  void Stop() {
    cancel = true;
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    Json::Value jm = tool.toJson();
    v8::Local<v8::Object> model = valueToObject(jm);

    v8::Local<v8::Value> errMsg, res;
    if (!cancel) {
      errMsg = Nan::Null();
      res = model;
    } else {
      errMsg = Nan::New<v8::String>("training cancelled").ToLocalChecked();
      res = Nan::Null();
    }

    v8::Local<v8::Value> results[] = { errMsg, res };

    // this should be safe to do this, as HandleOKCallback is called
    // in the event loop, and threaded training is disabled in XmmTool
    original = tool;
    // call reset so the model is ready to filter
    original.reset();

    callback->Call(2, results);

  }

private:
  Model& original;
  Model tool;

  xmm::TrainingSet set;
  bool cancel;
};

#endif /* _XMMWRAPTRAINWORKER_H_ */