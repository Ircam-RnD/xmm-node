#ifndef _XMM_WRAP_H_
#define _XMM_WRAP_H_

#include <napi.h>
#include "XmmTool.h"
#include "../xmm/src/xmm.h"

enum ModelTypeE { XmmUndefinedModelE = 0, XmmGmmE, XmmHhmmE };

class XmmWrap : public Napi::ObjectWrap<XmmWrap> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  XmmWrap(const Napi::CallbackInfo& info);
  ~XmmWrap();

private:
  static Napi::FunctionReference constructor;

  Napi::Value addPhrase           (const Napi::CallbackInfo& info); 
  Napi::Value getPhrase           (const Napi::CallbackInfo& info);
  Napi::Value getPhrasesOfLabel   (const Napi::CallbackInfo& info);
  Napi::Value removePhrase        (const Napi::CallbackInfo& info);
  void        removePhrasesOfLabel(const Napi::CallbackInfo& info);
  Napi::Value getTrainingSetSize  (const Napi::CallbackInfo& info);
  Napi::Value getTrainingSetLabels(const Napi::CallbackInfo& info);
  Napi::Value getTrainingSet      (const Napi::CallbackInfo& info);
  void        setTrainingSet      (const Napi::CallbackInfo& info);
  void        addTrainingSet      (const Napi::CallbackInfo& info);
  void        clearTrainingSet    (const Napi::CallbackInfo& info);

  void        train               (const Napi::CallbackInfo& info);
  void        cancelTraining      (const Napi::CallbackInfo& info);
  Napi::Value getModel            (const Napi::CallbackInfo& info);
  void        setModel            (const Napi::CallbackInfo& info);
  Napi::Value getModelType        (const Napi::CallbackInfo& info);
  void        reset               (const Napi::CallbackInfo& info);
  Napi::Value filter              (const Napi::CallbackInfo& info);
  
  Napi::Value getConfig           (const Napi::CallbackInfo& info);
  void        setConfig           (const Napi::CallbackInfo& info);

  ////////// private variables

public:
  xmm::TrainingSet *set_;
  XmmToolBase *model_;
  xmm::ClassParameters<xmm::GMM> *params_;
  xmm::Configuration<xmm::GMM> *config_;
  ModelTypeE modelType_;
  std::vector<int> freeList; // list of free indices in training set
};

#endif /* _XMM_WRAP_H_ */
