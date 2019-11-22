#ifndef _XMMWRAPTRAINWORKER_H_
#define _XMMWRAPTRAINWORKER_H_

#include <napi.h>
#include "../xmm/src/xmm.h"
#include "JsonCppNapiConverters.h"

template<typename Model>
class XmmWrapTrainWorker : public Napi::AsyncWorker {
private:
  Model& original;
  Model tool;

  xmm::TrainingSet set;
  bool cancel, done;

public:
  // static void DoWork(const Napi::CallbackInfo& info) {
  //   bool succeed = info[0].As<Napi::Boolean>();
  //   Napi::Object resource = info[1].As<Napi::Object>();
  //   Napi::Function cb = info[2].As<Napi::Function>();
  //   Napi::Value data = info[3];

  //   XmmWrapTrainWorker* worker = new XmmWrapTrainWorker(cb, tool, &set);
  //   worker->Queue();
  // }

  XmmWrapTrainWorker(Napi::Function& callback, Model& t, xmm::TrainingSet *s) :
  Napi::AsyncWorker(callback),
  original(t),
  tool(t),
  set(*s),
  cancel(false),
  done(false) {
    // With threads :
    tool.configuration.multithreading = xmm::MultithreadingMode::Background;

    // No threads !
    // tool.configuration.multithreading = xmm::MultithreadingMode::Sequential;
  }

  ~XmmWrapTrainWorker() {}

  void Execute() override {
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

  //*
  void OnOk() {
    Json::Value jm = tool.toJson();
    Napi::Object model = valueToObject(Env(), jm);

    // this should be safe to do this, as HandleOKCallback
    // is called in the event loop
    // is Napi::OnOk also called in the event loop ?
    // or are there drawbacks ?
    original = tool;
    
    // call reset so the model is ready to filter
    original.reset();

    done = true;
  }
  //*/

  std::vector<napi_value> GetResult(Napi::Env env) override {
    Json::Value jm = tool.toJson();
    Napi::Object model = valueToObject(env, jm);
    Napi::Value err = cancel ? Napi::String::New(env, "training cancelled") : env.Null();
    Napi::Value res = cancel ? env.Null() : model;

    return { err, res };
  }

  // With threads :
  void Stop() {
    cancel = true;
  }

  bool Done() {
    return done;
  }


  /*
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
  //*/
};

#endif /* _XMMWRAPTRAINWORKER_H_ */