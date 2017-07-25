#ifndef _XMMWRAPTRAINWORKER_H_
#define _XMMWRAPTRAINWORKER_H_

#include <nan.h>
#include "../xmm/src/xmm.h"
#include "JsonCppV8Converters.h"

template<typename Model>
class XmmWrapTrainWorker : public Nan::AsyncWorker {
public:
  XmmWrapTrainWorker(Nan::Callback *callback, Model& t, xmm::TrainingSet *s) :
  Nan::AsyncWorker(callback), original(t), tool(t), set(*s), cancel(false), done(false) {
    // With threads :
    tool.configuration.multithreading = xmm::MultithreadingMode::Background;

    // No threads !
    // tool.configuration.multithreading = xmm::MultithreadingMode::Sequential;
  }

  ~XmmWrapTrainWorker() {}

  void Execute() {
    // No threads !
    // if (set.size() > 0) {
    //   tool.train(&set);
    // } else {
    //   tool.clear();
    // }

    // With threads :
    if (set.size() > 0) {
      tool.train(&set);
    } else {
      tool.clear();
    }

    while (tool.training()) {
      if (cancel) {
        tool.cancelTraining();
      }
    }
  }

  // With threads :
  void Stop() {
    cancel = true;
  }

  bool Done() {
    return done;
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    Json::Value jm = tool.toJson();
    v8::Local<v8::Object> model = valueToObject(jm);

    v8::Local<v8::Value> errMsg, res;

    // With threads :
    if (!cancel) {
      errMsg = Nan::Null();
      res = model;
    } else {
      errMsg = Nan::New<v8::String>("training cancelled").ToLocalChecked();
      res = Nan::Null();
    }

    // No threads :
    // errMsg = Nan::Null();
    // res = model;

    v8::Local<v8::Value> results[] = { errMsg, res };

    // this should be safe to do this, as HandleOKCallback
    // is called in the event loop
    original = tool;

    // call reset so the model is ready to filter
    original.reset();
    done = true;
    callback->Call(2, results);
  }

private:
  Model& original;
  Model tool;

  xmm::TrainingSet set;
  bool cancel, done;
};

#endif /* _XMMWRAPTRAINWORKER_H_ */