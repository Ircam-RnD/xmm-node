#ifndef _XMMWRAP_H_
#define _XMMWRAP_H_

#include <nan.h>
#include "XmmTool.h"
#include "../xmm/src/xmm.h"

enum ModelTypeE { XmmUndefinedModelE = 0, XmmGmmE, XmmHhmmE };

class XmmWrap : public Nan::ObjectWrap {
public:
  static void Init();
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);

  xmm::TrainingSet *set_;
  XmmToolBase *model_;
  xmm::ClassParameters<xmm::GMM> *params_;
  xmm::Configuration<xmm::GMM> *config_;
  ModelTypeE modelType_;
  std::vector<int> freeList; // list of free indices in training set

private:
  XmmWrap();
  ~XmmWrap();

  static Nan::Persistent<v8::Function> constructor;
  static void New                 (const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void addPhrase           (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getPhrase           (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getPhrasesOfLabel   (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void removePhrase        (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void removePhrasesOfLabel(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getTrainingSetSize  (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getTrainingSetLabels(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getTrainingSet      (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void setTrainingSet      (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void addTrainingSet      (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void clearTrainingSet    (const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void train               (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void cancelTraining      (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getModel            (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void setModel            (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getModelType        (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void reset               (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void filter              (const Nan::FunctionCallbackInfo<v8::Value>& info);

  // static void getConfig  (v8::Local<v8::String> prop,
  //            const Nan::PropertyCallbackInfo<v8::Value> & args);

  // static void setConfig  (v8::Local<v8::String> prop,
  //            v8::Local<v8::Value> value,
  //            const Nan::PropertyCallbackInfo<void> & args);

  static void getConfig           (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void setConfig           (const Nan::FunctionCallbackInfo<v8::Value>& info);
};


#endif // _XMMWRAP_H_