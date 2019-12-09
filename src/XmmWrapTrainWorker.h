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
  XmmWrapTrainWorker(Napi::Function& callback, Model& t, xmm::TrainingSet* s) :
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
  void OnOK() override {
    Napi::Env env = Env();
    Napi::HandleScope scope(env);

    // this should be safe to do this, as HandleOKCallback
    // is called in the event loop
    // is Napi::OnOk also called in the event loop ?
    // or are there drawbacks ?
    original = tool;
    
    // call reset so the model is ready to filter
    original.reset();

    Json::Value jm = tool.toJson();
    Napi::Object model = valueToObject(env, jm);
    Napi::Value err = cancel ? Napi::String::New(env, "training cancelled") : env.Null();
    Napi::Value res = cancel ? env.Null() : model;
    Callback().Call({ err, res });

    done = true;
    // std::cout << "model trained, OnOk called" << std::endl;
  }
  //*/

  /*
  // THIS ONE SHOULD BE OVERRIDDEN IF OnOK IS NOT :
  std::vector<napi_value> GetResult(Napi::Env env) override {
    Json::Value jm = tool.toJson();
    Napi::Object model = valueToObject(env, jm);
    Napi::Value err = cancel ? Napi::String::New(env, "training cancelled") : env.Null();
    Napi::Value res = cancel ? env.Null() : model;
    std::cout << "model trained, GetResult called" << std::endl;

    return { err, res };
  }
  //*/

  // With threads :
  void Stop() {
    cancel = true;
  }

  bool Done() {
    return done;
  }
};

#endif /* _XMMWRAPTRAINWORKER_H_ */