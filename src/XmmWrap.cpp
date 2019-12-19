#include "XmmWrap.h"
#include "XmmWrapUtils.h"
#include "JsonCppNapiConverters.h"

Napi::FunctionReference XmmWrap::constructor;

Napi::Object
XmmWrap::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "xmm", {
    InstanceMethod("addPhrase",             &XmmWrap::addPhrase),
    InstanceMethod("getPhrase",             &XmmWrap::getPhrase),
    InstanceMethod("getPhrasesOfLabel",     &XmmWrap::getPhrasesOfLabel),
    InstanceMethod("removePhrase",          &XmmWrap::removePhrase),
    InstanceMethod("removePhrasesOfLabel",  &XmmWrap::removePhrasesOfLabel),
    InstanceMethod("getTrainingSetSize",    &XmmWrap::getTrainingSetSize),
    InstanceMethod("getTrainingSetLabels",  &XmmWrap::getTrainingSetLabels),
    InstanceMethod("getTrainingSet",        &XmmWrap::getTrainingSet),
    InstanceMethod("setTrainingSet",        &XmmWrap::setTrainingSet),
    InstanceMethod("addTrainingSet",        &XmmWrap::addTrainingSet),
    InstanceMethod("clearTrainingSet",      &XmmWrap::clearTrainingSet),

    InstanceMethod("train",                 &XmmWrap::train),
    InstanceMethod("cancelTraining",        &XmmWrap::cancelTraining),
    InstanceMethod("getModel",              &XmmWrap::getModel),
    InstanceMethod("setModel",              &XmmWrap::setModel),
    InstanceMethod("getModelType",          &XmmWrap::getModelType),
    InstanceMethod("reset",                 &XmmWrap::reset),
    InstanceMethod("filter",                &XmmWrap::filter),

    InstanceMethod("getConfig",             &XmmWrap::getConfig),
    InstanceMethod("setConfig",             &XmmWrap::setConfig)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("xmm", func);
  return exports;
  // return func;
}

XmmWrap::XmmWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<XmmWrap>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  bool h = true;
  std::size_t s = 1;
  std::size_t g = 1;
  double rr = 0.01;
  double ar = 0.01;
  xmm::GaussianDistribution::CovarianceMode cm = xmm::GaussianDistribution::CovarianceMode::Full;
  xmm::HMM::TransitionMode tm = xmm::HMM::TransitionMode::LeftRight;
  xmm::HMM::RegressionEstimator re = xmm::HMM::RegressionEstimator::Full; // use Windowed as a compromise between LR / ergodic ?

  std::string modelType = "";

  if (info.Length() > 0 && info[0].IsString()) {
    // v8::String::Utf8Value val(info[0]->ToString());
    // modelType = std::string(*val);
    modelType = std::string(info[0].As<Napi::String>());
  }

  if (modelType != "gmm" &&
      modelType != "hhmm") {
    modelType = "gmm";
  }

  if (modelType == "gmm") {
    model_ = new XmmTool<xmm::GMM, xmm::GMM>();
    modelType_ = XmmGmmE;
  } else if (modelType == "hhmm") {
    model_ = new XmmTool<xmm::HierarchicalHMM, xmm::HMM>();
    modelType_ = XmmHhmmE;
    setHierarchical(this, h);
    setStates(this, s);
    setTransitionMode(this, tm);
    setRegressionEstimator(this, re);
  } else { // never happens
    modelType_ = XmmUndefinedModelE;
  }

  model_->setGaussians(g);
  model_->setRelativeRegularization(rr);
  model_->setAbsoluteRegularization(ar);
  model_->setCovarianceMode(cm);

  set_ = new xmm::TrainingSet();
}

XmmWrap::~XmmWrap() {
  delete set_;
  if (modelType_ != XmmUndefinedModelE) {
    delete model_;
  }
}

Napi::Value
XmmWrap::addPhrase(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsObject()) {

    Napi::Object inputPhrase = info[0].As<Napi::Object>();
    xmm::Phrase xp;

    if (makeXmmPhrase(inputPhrase, xp)) {
      int index;
      if (freeList.size() == 0) {
        index = set_->size();
      } else {
        index = freeList.back();
        freeList.pop_back();
      }

      if (set_->size() == 0) {
        delete set_;
        if (xp.bimodal()) {
          set_ = new xmm::TrainingSet(xmm::MemoryMode::OwnMemory,
                                      xmm::Multimodality::Bimodal);
          set_->dimension_input.set(xp.dimension_input.get());
        } else {
          set_ = new xmm::TrainingSet();
        }

        set_->dimension.set(xp.dimension.get());
        set_->column_names.set(xp.column_names, true);

        model_->setBimodal(xp.bimodal());
      }

      set_->addPhrase(index, xp);
      return Napi::Number::New(info.Env(), index);
    }
  }

  return Napi::Number::New(info.Env(), -1);;
}

Napi::Value
XmmWrap::getPhrase(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsNumber()) {
    int index = info[0].As<Napi::Number>().Int32Value();

    for (int i = 0; i < freeList.size(); i++) {
      if (index == freeList[i]) return info.Env().Undefined();
    }

    if (index < set_->size() + freeList.size()) {
      std::shared_ptr<xmm::Phrase> xp = set_->getPhrase(index);

      if (xp != nullptr) {
        return makeObjectPhrase(info.Env(), *xp); // Napi::Object::New(info.Env(), makeObjectPhrase(*xp));
      }
    }

    return info.Env().Undefined();
  }
}

Napi::Value
XmmWrap::getPhrasesOfLabel(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsString()) {
    std::string label = std::string(info[0].As<Napi::String>());

    xmm::TrainingSet *s = set_->getPhrasesOfClass(label);
    Json::Value js;

    if (s != nullptr) {
      js = s->toJson();
    }

    return valueToObject(info.Env(), js);
  }
}

Napi::Value
XmmWrap::removePhrase(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsNumber()) {
    int pindex = info[0].As<Napi::Number>().Int32Value();

    for (int i = 0; i < freeList.size(); i++) {
      if (pindex == freeList[i]) {
        return Napi::Boolean::New(info.Env(), false);
      }
    }

    freeList.push_back(pindex);
    std::sort(freeList.begin(), freeList.end(), std::greater<int>());

    if (set_->getPhrase(pindex) != nullptr) {
      set_->removePhrase(pindex);
      return Napi::Boolean::New(info.Env(), true);
    }
  }

  return Napi::Boolean::New(info.Env(), false);
}

void
XmmWrap::removePhrasesOfLabel(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsString()) {
    std::string label = std::string(info[0].As<Napi::String>());

    xmm::TrainingSet *s = set_->getPhrasesOfClass(label);

    if (s != nullptr) {
      for (auto const &phrase : *s) {
        freeList.push_back(phrase.first);
      }
    }

    std::sort(freeList.begin(), freeList.end(), std::greater<int>());
    set_->removePhrasesOfClass(label);
  }
}

Napi::Value
XmmWrap::getTrainingSetSize(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), static_cast<int>(set_->size()));
}

Napi::Value
XmmWrap::getTrainingSetLabels(const Napi::CallbackInfo& info) {
  std::set<std::string> slabels = set_->labels();
  Napi::Array labels = Napi::Array::New(info.Env(), slabels.size());
  std::size_t i = 0;

  for (auto it : slabels) {
    labels.Set(i, Napi::String::New(info.Env(), it));
    ++i;
  }

  return labels;
}

Napi::Value
XmmWrap::getTrainingSet(const Napi::CallbackInfo& info) {
  // Json::Value = set_->toJson()
  return valueToObject(info.Env(), set_->toJson());
}

void
XmmWrap::setTrainingSet(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsObject()) {
    set_->fromJson(objectToValue(info[0].As<Napi::Object>())); // no json validation ????
    model_->setBimodal(set_->bimodal());
  }
}

void
XmmWrap::addTrainingSet(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsObject()) {
    xmm::TrainingSet xs(objectToValue(info[0].As<Napi::Object>()));

    for (auto xp : xs) {
      int index;

      if (freeList.size() == 0) {
        index = set_->size();
      } else {
        index = freeList.back();
        freeList.pop_back();
      }

      if (set_->size() == 0) {
        delete set_;

        if (xp.second->bimodal()) {
          set_ = new xmm::TrainingSet(xmm::MemoryMode::OwnMemory,
                                      xmm::Multimodality::Bimodal);
          set_->dimension_input.set(xp.second->dimension_input.get());
        } else {
          set_ = new xmm::TrainingSet();
        }

        set_->dimension.set(xp.second->dimension.get());
        set_->column_names.set(xp.second->column_names, true);
        model_->setBimodal(xp.second->bimodal());
      }

      set_->addPhrase(index, xp.second);
    }
  }
}

void
XmmWrap::clearTrainingSet(const Napi::CallbackInfo& info) {
  set_->clear();
  freeList.clear();
}

void
XmmWrap::train(const Napi::CallbackInfo& info) {
  // XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

  /*
  // ============= synchronous callback ============ //
  obj->model_->train(obj->set_);
  while(obj->model_->training());

  v8::Local<v8::Function> cb = args[0].As<v8::Function>();
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New("finished training").ToLocalChecked() };

  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
  //*/

  // Nan::Callback *callback = new Nan::Callback(args[0].As<v8::Function>());
  // obj->model_->train(callback, obj->set_);
  Napi::Function callback = info[0].As<Napi::Function>();
  model_->train(callback, set_);
}

void
XmmWrap::cancelTraining(const Napi::CallbackInfo& info) {
  model_->cancelTraining();
}

Napi::Value
XmmWrap::getModel(const Napi::CallbackInfo& info) {
  return valueToObject(info.Env(), model_->getModel());
}

void       
XmmWrap::setModel(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsObject()) {
    model_->setModel(objectToValue(info[0].As<Napi::Object>()));
  }
}

Napi::Value
XmmWrap::getModelType(const Napi::CallbackInfo& info) {
  std::string stype = "unknown";
  switch (modelType_) {
    case XmmGmmE:
      stype = "gmm";
      break;
    case XmmHhmmE:
      stype = "hhmm";
      break;
    default:
      break;
  }

  return Napi::String::New(info.Env(), stype);
}

void       
XmmWrap::reset(const Napi::CallbackInfo& info) {
  model_->reset();
}

Napi::Value
XmmWrap::filter(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() > 0 && info[0].IsArray()) {
    Napi::Array arr = info[0].As<Napi::Array>();

    // std::cout << model_->model->

    if (arr.Length() > 0) { // should be more strict, sth like "== model_->model->dimension_input) {""
      std::vector<float> observation;

      for (unsigned int i = 0; i < arr.Length(); i++) {
        if (arr.Get(i).IsNumber()) {
          observation.push_back(arr.Get(i).ToNumber().DoubleValue());
        } else {
          observation.push_back(Napi::Number::New(env, 0));
        }
      }

      return valueToObject(env, model_->filter(observation));
    }
  }

  return env.Null();
}

//============================= CONFIGURATION ================================//

Napi::Value
XmmWrap::getConfig(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  std::size_t gaussians = model_->getGaussians();
  double relative_regularization = model_->getRelativeRegularization();
  double absolute_regularization = model_->getAbsoluteRegularization();
  xmm::GaussianDistribution::CovarianceMode covariance_mode = model_->getCovarianceMode();
  xmm::MultiClassRegressionEstimator multiclass_regression_estimator = model_->getMultiClassRegressionEstimator();

  // HierarchicalHMM-specific :
  bool hierarchical = true;
  std::size_t states = 1;
  xmm::HMM::TransitionMode transition_mode = xmm::HMM::TransitionMode::LeftRight;
  xmm::HMM::RegressionEstimator regression_estimator = xmm::HMM::RegressionEstimator::Full;

  if (modelType_ == XmmHhmmE) {
    hierarchical = getHierarchical(this);
    states = getStates(this);
    transition_mode = getTransitionMode(this);
    regression_estimator = getRegressionEstimator(this);
  }

  std::string cm = (covariance_mode == xmm::GaussianDistribution::CovarianceMode::Full)
                 ? "full"
                 : ((covariance_mode == xmm::GaussianDistribution::CovarianceMode::Diagonal)
                    ? "diagonal"
                    : "");

  std::string tm = (transition_mode == xmm::HMM::TransitionMode::Ergodic)
                 ? "ergodic"
                 : ((transition_mode == xmm::HMM::TransitionMode::LeftRight)
                    ? "leftright"
                    : "");

  std::string re = (regression_estimator == xmm::HMM::RegressionEstimator::Full)
                 ? "full"
                 : ((regression_estimator == xmm::HMM::RegressionEstimator::Windowed)
                    ? "windowed"
                    : ((regression_estimator == xmm::HMM::RegressionEstimator::Likeliest)
                       ? "likeliest"
                       : ""));

  std::string mre = (multiclass_regression_estimator == xmm::MultiClassRegressionEstimator::Likeliest)
                  ? "likeliest"
                  : ((multiclass_regression_estimator == xmm::MultiClassRegressionEstimator::Mixture)
                    ? "mixture"
                    : "");

  std::string modelType;
  switch (modelType_) {
    case XmmGmmE:
      modelType = "gmm";
      break;

    case XmmHhmmE:
      modelType = "hhmm";
      break;

    default:
      modelType = "unknown"; // never happens
      break;
  }

  if (info.Length() > 0 && info[0].IsString()) {
    // Nan::MaybeLocal<v8::Value> maybeItem = args[0];
    // v8::Local<v8::Value> vitem = maybeItem.ToLocalChecked();
    // if (!vitem->IsString()) return;

    // v8::String::Utf8Value val(vitem->ToString());
    // std::string item = std::string(*val);

    std::string item = std::string(info[0].As<Napi::String>());

    if (item == "gaussians") {
      return Napi::Number::New(env, gaussians);
      // args.GetReturnValue().Set(Nan::New<v8::Number>(gaussians));
    } else if (item == "relative_regularization") {
      return Napi::Number::New(env, relative_regularization);
      // args.GetReturnValue().Set(Nan::New<v8::Number>(relative_regularization));
    } else if (item == "absolute_regularization") {
      return Napi::Number::New(env, absolute_regularization);
      // args.GetReturnValue().Set(Nan::New<v8::Number>(absolute_regularization));
    } else if (item == "covariance_mode") {
      return Napi::String::New(env, cm);
      // args.GetReturnValue().Set(Nan::New<v8::String>(cm).ToLocalChecked());
    } else if (item == "multiclass_regression_etimator") {
      return Napi::String::New(env, mre);
      // args.GetReturnValue().Set(Nan::New<v8::String>(mre).ToLocalChecked());
    }

    if (modelType_ == XmmHhmmE) {
      if (item == "hierarchical") {
        return Napi::Boolean::New(env, hierarchical);
        // args.GetReturnValue().Set(Nan::New<v8::Boolean>(hierarchical));
      } else if (item == "states") {
        return Napi::Number::New(env, states);
        // args.GetReturnValue().Set(Nan::New<v8::Number>(states));
      } else if (item == "transition_mode") {
        return Napi::String::New(env, tm);
        // args.GetReturnValue().Set(Nan::New<v8::String>(tm).ToLocalChecked());
      } else if (item == "regression_estimator") {
        return Napi::String::New(env, re);
        // args.GetReturnValue().Set(Nan::New<v8::String>(re).ToLocalChecked());
      }
    }

  } else {
    // v8::Local<v8::Object> outputConfig = Nan::New<v8::Object>();
    Napi::Object outputConfig = Napi::Object::New(env);

    outputConfig.Set("gaussians", Napi::Number::New(env, gaussians));
    outputConfig.Set("relative_regularization", Napi::Number::New(env, relative_regularization));
    outputConfig.Set("absolute_regularization", Napi::Number::New(env, absolute_regularization));
    outputConfig.Set("covariance_mode", Napi::String::New(env, cm));
    outputConfig.Set("multiClass_regression_estimator", Napi::String::New(env, mre));

    if (modelType_ == XmmHhmmE) {
      outputConfig.Set("hierarchical", Napi::Boolean::New(env, hierarchical));
      outputConfig.Set("states", Napi::Number::New(env, states));
      outputConfig.Set("transition_mode", Napi::String::New(env, tm));
      outputConfig.Set("regression_estimator", Napi::String::New(env, re));
    }

    // args.GetReturnValue().Set(outputConfig);
    return outputConfig;
  }
}

void
XmmWrap::setConfig(const Napi::CallbackInfo& info) {
  if (info.Length() > 0 && info[0].IsObject()) {
    Napi::Object inputConfig = info[0].As<Napi::Object>();

    int g = model_->getGaussians();
    double rr = model_->getRelativeRegularization();
    double ar = model_->getAbsoluteRegularization();
    xmm::GaussianDistribution::CovarianceMode cm = model_->getCovarianceMode();
    xmm::MultiClassRegressionEstimator mre = model_->getMultiClassRegressionEstimator();

    // HierarchicalHMM-specific :
    bool h = true;
    int s = 1;
    xmm::HMM::TransitionMode tm = xmm::HMM::TransitionMode::LeftRight;
    xmm::HMM::RegressionEstimator re = xmm::HMM::RegressionEstimator::Full;

    if (modelType_ == XmmHhmmE) {
      h = getHierarchical(this);
      s = getStates(this);
      tm = getTransitionMode(this);
      re = getRegressionEstimator(this);
    }

    if (inputConfig.Get("hierarchical").IsBoolean()) {
      h = inputConfig.Get("hierarchical").As<Napi::Boolean>().Value();
    }

    if (inputConfig.Get("states").IsNumber()) {
      s = inputConfig.Get("states").As<Napi::Number>().Int32Value();
    }

    if (inputConfig.Get("gaussians").IsNumber()) {
      g = inputConfig.Get("gaussians").As<Napi::Number>().Int32Value();
    }    

    if (inputConfig.Get("relative_regularization").IsNumber()) {
      rr = inputConfig.Get("relative_regularization").As<Napi::Number>().DoubleValue();
    }

    if (inputConfig.Get("absolute_regularization").IsNumber()) {
      ar = inputConfig.Get("absolute_regularization").As<Napi::Number>().DoubleValue();
    }  

    if (inputConfig.Get("covariance_mode").IsString()) {
      std::string scm = std::string(inputConfig.Get("covariance_mode").As<Napi::String>());
      if (scm == "full") {
        cm = xmm::GaussianDistribution::CovarianceMode::Full;
      } else if (scm == "diagonal") {
        cm = xmm::GaussianDistribution::CovarianceMode::Diagonal;
      }
    }

    if (inputConfig.Get("transition_mode").IsString()) {
      std::string stm = std::string(inputConfig.Get("transition_mode").As<Napi::String>());
      if (stm == "ergodic") {
        tm = xmm::HMM::TransitionMode::Ergodic;
      } else if (stm == "leftright") {
        tm = xmm::HMM::TransitionMode::LeftRight;
      }
    }

    if (inputConfig.Get("regression_estimator").IsString()) {
      std::string sre = std::string(inputConfig.Get("regression_estimator").As<Napi::String>());
      if (sre == "full") {
        re = xmm::HMM::RegressionEstimator::Full;
      } else if (sre == "windowed") {
        re = xmm::HMM::RegressionEstimator::Windowed;
      } else if (sre == "likeliest") {
        re = xmm::HMM::RegressionEstimator::Likeliest;
      }
    }

    if (inputConfig.Get("multiClass_regression_estimator").IsString()) {
      std::string smre = std::string(inputConfig.Get("multiClass_regression_estimator").As<Napi::String>());
      if (smre == "likeliest") {
        mre = xmm::MultiClassRegressionEstimator::Likeliest;
      } else if (smre == "mixture") {
        mre = xmm::MultiClassRegressionEstimator::Mixture;
      }
    }

    model_->setGaussians(g);
    model_->setRelativeRegularization(rr);
    model_->setAbsoluteRegularization(ar);
    model_->setCovarianceMode(cm);
    model_->setMultiClassRegressionEstimator(mre);

    if (modelType_ == XmmHhmmE) {
      setHierarchical(this, h);
      setStates(this, s);
      setTransitionMode(this, tm);
      setRegressionEstimator(this, re);
    }
  }
}

/*
void XmmWrap::setConfig(const Nan::FunctionCallbackInfo<v8::Value> & args) {
  XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.This());

  if (args.Length() > 0) {
    Nan::MaybeLocal<v8::Value> maybeInputConfig = args[0];
    v8::Local<v8::Object> inputConfig
      = v8::Local<v8::Object>::Cast(maybeInputConfig.ToLocalChecked());
    if (!inputConfig->IsObject()) return;

    std::size_t g = obj->model_->getGaussians();
    double rr = obj->model_->getRelativeRegularization();
    double ar = obj->model_->getAbsoluteRegularization();
    xmm::GaussianDistribution::CovarianceMode cm = obj->model_->getCovarianceMode();
    xmm::MultiClassRegressionEstimator mre = obj->model_->getMultiClassRegressionEstimator();

    // HierarchicalHMM-specific :
    bool h = true;
    std::size_t s = 1;
    xmm::HMM::TransitionMode tm = xmm::HMM::TransitionMode::LeftRight;
    xmm::HMM::RegressionEstimator re = xmm::HMM::RegressionEstimator::Full;

    if (obj->modelType_ == XmmHhmmE) {
      h = getHierarchical(obj);
      s = getStates(obj);
      tm = getTransitionMode(obj);
      re = getRegressionEstimator(obj);
    }

    // get object argument's fields :
    v8::Local<v8::Value> hierarchical
      = inputConfig->Get(Nan::New<v8::String>("hierarchical").ToLocalChecked());
    if (hierarchical->IsBoolean()) {
      h = hierarchical->BooleanValue();
    }

    v8::Local<v8::Value> states
      = inputConfig->Get(Nan::New<v8::String>("states").ToLocalChecked());
    if (states->IsNumber()) {
      s = states->NumberValue();
    }

    v8::Local<v8::Value> gaussians
      = inputConfig->Get(Nan::New<v8::String>("gaussians").ToLocalChecked());
    if (gaussians->IsNumber()) {
      g = gaussians->NumberValue();
    }
    v8::Local<v8::Value> relative_regularization
      = inputConfig->Get(Nan::New<v8::String>("relative_regularization").ToLocalChecked());
    if (relative_regularization->IsNumber()) {
      rr = relative_regularization->NumberValue();
    }

    v8::Local<v8::Value> absolute_regularization
      = inputConfig->Get(Nan::New<v8::String>("absolute_regularization").ToLocalChecked());
    if (absolute_regularization->IsNumber()) {
      ar = absolute_regularization->NumberValue();
    }

    v8::Local<v8::Value> covariance_mode
      = inputConfig->Get(Nan::New<v8::String>("covariance_mode").ToLocalChecked());
    if (covariance_mode->IsString()) {
      v8::String::Utf8Value val(covariance_mode->ToString());
      std::string scm = std::string(*val);
      if (scm == "full") {
        cm = xmm::GaussianDistribution::CovarianceMode::Full;
      } else if (scm == "diagonal") {
        cm = xmm::GaussianDistribution::CovarianceMode::Diagonal;
      }
    }

    v8::Local<v8::Value> transition_mode
      = inputConfig->Get(Nan::New<v8::String>("transition_mode").ToLocalChecked());
    if (transition_mode->IsString()) {
      v8::String::Utf8Value val(transition_mode->ToString());
      std::string stm = std::string(*val);
      if (stm == "ergodic") {
        tm = xmm::HMM::TransitionMode::Ergodic;
      } else if (stm == "leftright") {
        tm = xmm::HMM::TransitionMode::LeftRight;
      }
    }

    v8::Local<v8::Value> regression_estimator
      = inputConfig->Get(Nan::New<v8::String>("regression_estimator").ToLocalChecked());
    if (regression_estimator->IsString()) {
      v8::String::Utf8Value val(regression_estimator->ToString());
      std::string sre = std::string(*val);
      if (sre == "full") {
        re = xmm::HMM::RegressionEstimator::Full;
      } else if (sre == "windowed") {
        re = xmm::HMM::RegressionEstimator::Windowed;
      } else if (sre == "likeliest") {
        re = xmm::HMM::RegressionEstimator::Likeliest;
      }
    }

    v8::Local<v8::Value> multiclass_regression_estimator
      = inputConfig->Get(Nan::New<v8::String>("multiClass_regression_estimator").ToLocalChecked());
    if (multiclass_regression_estimator->IsString()) {
      v8::String::Utf8Value val(multiclass_regression_estimator->ToString());
      std::string smre = std::string(*val);
      if (smre == "likeliest") {
        mre = xmm::MultiClassRegressionEstimator::Likeliest;
      } else if (smre == "mixture") {
        mre = xmm::MultiClassRegressionEstimator::Mixture;
      }
    }

    // =============== SET NEW VALUES ============== //

    obj->model_->setGaussians(g);
    obj->model_->setRelativeRegularization(rr);
    obj->model_->setAbsoluteRegularization(ar);
    obj->model_->setCovarianceMode(cm);
    obj->model_->setMultiClassRegressionEstimator(mre);

    if (obj->modelType_ == XmmHhmmE) {
      setHierarchical(obj, h);
      setStates(obj, s);
      setTransitionMode(obj, tm);
      setRegressionEstimator(obj, re);
    }
  }
}
//*/

