#ifndef _XMMWRAP_UTILS_H_
#define _XMMWRAP_UTILS_H_

#include <napi.h>
#include "XmmWrap.h"

//=============== HierarchicalHMM-specific getters / setters =================//

XmmTool<xmm::HierarchicalHMM, xmm::HMM> *getHierarchicalHMM(XmmWrap *obj) {
  return reinterpret_cast<XmmTool<xmm::HierarchicalHMM, xmm::HMM> *>(obj->model_);
}

bool getHierarchical(XmmWrap *obj) {
  return getHierarchicalHMM(obj)->model.configuration.hierarchical.get();
}

void setHierarchical(XmmWrap *obj, bool hierarchical) {
  XmmTool<xmm::HierarchicalHMM, xmm::HMM> *mod = getHierarchicalHMM(obj);
  mod->model.configuration.hierarchical.set(hierarchical);
  mod->model.configuration.changed = true;
}

std::size_t getStates(XmmWrap *obj) {
  return getHierarchicalHMM(obj)->model.configuration.states.get();
}

void setStates(XmmWrap *obj, std::size_t states) {
  XmmTool<xmm::HierarchicalHMM, xmm::HMM> *mod = getHierarchicalHMM(obj);
  mod->model.configuration.states.set(states);
  mod->model.configuration.changed = true;  
}

xmm::HMM::TransitionMode getTransitionMode(XmmWrap *obj) {
  return getHierarchicalHMM(obj)->model.configuration.transition_mode.get();
}

void setTransitionMode(XmmWrap *obj, xmm::HMM::TransitionMode transition_mode) {
  XmmTool<xmm::HierarchicalHMM, xmm::HMM> *mod = getHierarchicalHMM(obj);
  mod->model.configuration.transition_mode.set(transition_mode);
  mod->model.configuration.changed = true;
}

xmm::HMM::RegressionEstimator getRegressionEstimator(XmmWrap *obj) {
  return getHierarchicalHMM(obj)->model.configuration.regression_estimator.get();
}

void setRegressionEstimator(XmmWrap *obj,
                            xmm::HMM::RegressionEstimator regression_estimator) {
  XmmTool<xmm::HierarchicalHMM, xmm::HMM> *mod = getHierarchicalHMM(obj);
  mod->model.configuration.regression_estimator.set(regression_estimator);
  mod->model.configuration.changed = true;
}

//============================ Phrase converters =============================//

Napi::Object makeObjectPhrase(Napi::Env env, xmm::Phrase xp) {
  Json::Value jp = xp.toJson();
  Napi::Object outputPhrase = Napi::Object::New(env);

  outputPhrase.Set("bimodal", Napi::Boolean::New(env, jp.get("bimodal", false).asBool()));
  outputPhrase.Set("dimension", Napi::Number::New(env, jp.get("dimension", false).asInt()));
  outputPhrase.Set("dimension_input", Napi::Number::New(env, jp.get("dimension_input", false).asInt()));
  outputPhrase.Set("length", Napi::Number::New(env, jp.get("length", false).asInt()));
  outputPhrase.Set("label", Napi::String::New(env, jp["label"].asString()));

  int ncols = jp["column_names"].size();
  int datasize = jp["data"].size();
  int dataInputSize = jp["data_input"].size();
  int dataOutputSize = jp["data_output"].size();

  Napi::Array columnNames = Napi::Array::New(env, ncols);
  for (int i = 0; i < ncols; i++) {
    columnNames.Set(i, Napi::String::New(env, jp["column_names"].get(i, "").asString()));
  }

  Napi::Array data = Napi::Array::New(env, datasize);
  for (int i = 0; i < datasize; i++) {
    data.Set(i, Napi::Number::New(env, jp["data"][i].asFloat()));
  }

  Napi::Array dataInput = Napi::Array::New(env, dataInputSize);
  for (int i = 0; i < dataInputSize; i++) {
    dataInput.Set(i, Napi::Number::New(env, jp["data_input"][i].asFloat()));
  }

  Napi::Array dataOutput = Napi::Array::New(env, dataOutputSize);
  for (int i = 0; i < dataOutputSize; i++) {
    dataOutput.Set(i, Napi::Number::New(env, jp["data_output"][i].asFloat()));
  }

  outputPhrase.Set("column_names", columnNames);
  outputPhrase.Set("data", data);
  outputPhrase.Set("data_input", dataInput);
  outputPhrase.Set("data_output", dataOutput);

  return outputPhrase;
}

bool makeXmmPhrase(Napi::Object inputPhrase, xmm::Phrase& xp) {
  Napi::Env env = inputPhrase.Env();

  Napi::Value maybeBimodal = inputPhrase.Get("bimodal");
  Napi::Value maybeDimension = inputPhrase.Get("dimension");
  Napi::Value maybeDimensionInput = inputPhrase.Get("dimension_input");
  Napi::Value maybeLength = inputPhrase.Get("length");
  Napi::Value maybeLabel = inputPhrase.Get("label");

  Napi::Boolean vbimodal = (!maybeBimodal.IsEmpty() &&
                            maybeBimodal.IsBoolean())
                         ? maybeBimodal.As<Napi::Boolean>()
                         : Napi::Boolean::New(env, false);

  Napi::Number vdimension = (!maybeDimension.IsEmpty() &&
                             maybeDimension.IsNumber())
                          ? maybeDimension.As<Napi::Number>()
                          : Napi::Number::New(env, 1);
                        
  Napi::Number vdimensionInput = (!maybeDimensionInput.IsEmpty() &&
                                  maybeDimensionInput.IsNumber())
                               ? maybeDimensionInput.As<Napi::Number>()
                               : Napi::Number::New(env, 0);

  Napi::Number vlength = (!maybeLength.IsEmpty() &&
                          maybeLength.IsNumber())
                       ? maybeLength.As<Napi::Number>()
                       : Napi::Number::New(env, 0);

  Napi::String vlabel = (!maybeLabel.IsEmpty() &&
                         maybeLabel.IsString())
                      ? maybeLabel.As<Napi::String>()
                      : Napi::String::New(env, "");

  ////////// get array fields

  Napi::Value maybeColumnNames = inputPhrase.Get("column_names");
  Napi::Value maybeData = inputPhrase.Get("data");
  Napi::Value maybeDataInput = inputPhrase.Get("data_input");
  Napi::Value maybeDataOutput = inputPhrase.Get("data_output");

  Napi::Array vcolumnNames = (!maybeColumnNames.IsEmpty() &&
                              maybeColumnNames.IsArray())
                           ? maybeColumnNames.As<Napi::Array>()
                           : Napi::Array::New(env, 0);

  Napi::Array vdata = (!maybeData.IsEmpty() &&
                        maybeData.IsArray())
                    ? maybeData.As<Napi::Array>()
                    : Napi::Array::New(env, 0);

  Napi::Array vdataInput = (!maybeDataInput.IsEmpty() &&
                            maybeDataInput.IsArray())
                         ? maybeDataInput.As<Napi::Array>()
                         : Napi::Array::New(env, 0);

  Napi::Array vdataOutput = (!maybeDataOutput.IsEmpty() &&
                             maybeDataOutput.IsArray())
                          ? maybeDataOutput.As<Napi::Array>()
                          : Napi::Array::New(env, 0);

  ////////// get values in native types

  bool bimodal = vbimodal.Value();
  int dimension = vdimension.Int32Value();
  int dimensionInput = vdimensionInput.Int32Value();
  int length = vlength.Int32Value();
  std::string label(vlabel);

  // ============== BUILD JSON::VALUE ============ //
  
  Json::Value jp;

  jp["bimodal"] = bimodal;
  jp["dimension"] = dimension;
  jp["dimension_input"] = dimensionInput;
  jp["length"] = length;
  jp["label"] = label;

  for (int i = 0; i < dimension; i++) {

    jp["column_names"][i] = (i < vcolumnNames.Length() &&
                             vcolumnNames.Get(i).IsString())
                          ? std::string(vcolumnNames.Get(i).As<Napi::String>())
                          : "";
  }

  for (int i = 0; i < vdata.Length(); i++) {
    if(vdata.Get(i).IsNumber()) {
      jp["data"][i] = vdata.Get(i).As<Napi::Number>().DoubleValue();
    }
  }

  jp["data_input"] = Json::Value(Json::arrayValue);

  for (int i = 0; i < vdataInput.Length(); i++) {
    if(vdataInput.Get(i).IsNumber()) {
      jp["data"][i] = vdataInput.Get(i).As<Napi::Number>().DoubleValue();
    }
  }

  jp["data_output"] = Json::Value(Json::arrayValue);
  for (int i = 0; i < vdataOutput.Length(); i++) {
    if(vdataOutput.Get(i).IsNumber()) {
      jp["data"][i] = vdataOutput.Get(i).As<Napi::Number>().DoubleValue();
    }
  }

  // check for possible errors in xmm::Phrase creation and ...

  if((!bimodal && dimension * length == jp["data"].size()) ||
      (bimodal && (dimension - dimensionInput) * length == jp["data_output"].size()
      && dimensionInput * length == jp["data_input"].size())) {
    xp.fromJson(jp);
    return true;
  }

  return false;
}

#endif /* _XMMWRAP_UTILS_H_ */