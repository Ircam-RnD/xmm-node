#ifndef _XMMWRAP_UTILS_H_
#define _XMMWRAP_UTILS_H_

#include <nan.h>
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

v8::Local<v8::Object> makeObjectPhrase(xmm::Phrase xp) {
  Json::Value jp = xp.toJson();

  v8::Local<v8::Object> outputPhrase = Nan::New<v8::Object>();
  int dimension = jp.get("dimension", false).asInt();

  Nan::Set(outputPhrase, Nan::New<v8::String>("bimodal").ToLocalChecked(),
    Nan::New(jp.get("bimodal", false).asBool()));
  Nan::Set(outputPhrase, Nan::New<v8::String>("dimension").ToLocalChecked(),
    Nan::New(dimension));
  Nan::Set(outputPhrase, Nan::New<v8::String>("dimension_input").ToLocalChecked(),
    Nan::New(jp.get("dimension_input", false).asInt()));
  Nan::Set(outputPhrase, Nan::New<v8::String>("length").ToLocalChecked(),
    Nan::New(jp.get("length", false).asInt()));
  Nan::Set(outputPhrase, Nan::New<v8::String>("label").ToLocalChecked(),
    Nan::New<v8::String>(jp["label"].asString()).ToLocalChecked());

  int ncols = jp["column_names"].size();
  v8::Local<v8::Array> columnNames = Nan::New<v8::Array>(ncols);
  for(int i=0; i<ncols; i++) {
    columnNames->Set(i, Nan::New<v8::String>(jp["column_names"].get(i, "").asString()).ToLocalChecked());
  }
  Nan::Set(outputPhrase, Nan::New<v8::String>("column_names").ToLocalChecked(), columnNames);

  int datasize = jp["data"].size();
  v8::Local<v8::Array> data = Nan::New<v8::Array>(datasize);
  for(int i=0; i<datasize; i++) {
    data->Set(i, Nan::New(jp["data"][i].asFloat()));
  }
  Nan::Set(outputPhrase, Nan::New<v8::String>("data").ToLocalChecked(), data);

  int dataInputSize = jp["data_input"].size();
  v8::Local<v8::Array> dataInput = Nan::New<v8::Array>(dataInputSize);
  for(int i=0; i<dataInputSize; i++) {
    dataInput->Set(i, Nan::New(jp["data_input"][i].asFloat()));
  }
  Nan::Set(outputPhrase, Nan::New<v8::String>("data_input").ToLocalChecked(), dataInput);

  int dataOutputSize = jp["data_output"].size();
  v8::Local<v8::Array> dataOutput = Nan::New<v8::Array>(dataOutputSize);
  for(int i=0; i<dataOutputSize; i++) {
    dataOutput->Set(i, Nan::New(jp["data_output"][i].asFloat()));
  }
  Nan::Set(outputPhrase, Nan::New<v8::String>("data_output").ToLocalChecked(), dataOutput);

  return outputPhrase;
}


bool makeXmmPhrase(v8::Local<v8::Object> inputPhrase, xmm::Phrase &xp) {

  Nan::MaybeLocal<v8::Value> maybeBimodal
    = inputPhrase->Get(Nan::New("bimodal").ToLocalChecked());
  Nan::MaybeLocal<v8::Value> maybeDimension
    = inputPhrase->Get(Nan::New("dimension").ToLocalChecked());
  Nan::MaybeLocal<v8::Value> maybeDimensionInput
    = inputPhrase->Get(Nan::New("dimension_input").ToLocalChecked());
  Nan::MaybeLocal<v8::Value> maybeLength
    = inputPhrase->Get(Nan::New("length").ToLocalChecked());
  Nan::MaybeLocal<v8::Value> maybeLabel
    = inputPhrase->Get(Nan::New("label").ToLocalChecked());

  v8::Local<v8::Value> vbimodal;
  if(!maybeBimodal.IsEmpty()) {
    vbimodal = maybeBimodal.ToLocalChecked();
  } else return false;

  v8::Local<v8::Value> vdimension;
  if(!maybeDimension.IsEmpty()) {
    vdimension = maybeDimension.ToLocalChecked();
  } else return false;

  v8::Local<v8::Value> vdimension_input;
  if(!maybeDimensionInput.IsEmpty()) {
    vdimension_input = maybeDimensionInput.ToLocalChecked();
  } else return false;

  v8::Local<v8::Value> vlength;
  if(!maybeLength.IsEmpty()) {
    vlength = maybeLength.ToLocalChecked();
  } else return false;

  v8::Local<v8::Value> vlabel;
  if(!maybeLabel.IsEmpty()) {
    vlabel = maybeLabel.ToLocalChecked();
  } else return false;

  // ============= get arrays' contents ============ //

  Nan::MaybeLocal<v8::Value> maybeColumnNames
    = inputPhrase->Get(Nan::New("column_names").ToLocalChecked());
  v8::Local<v8::Array> vcolumn_names
    = v8::Local<v8::Array>::Cast(maybeColumnNames.ToLocalChecked());
  if(!vcolumn_names->IsArray()) {
    vcolumn_names = Nan::New<v8::Array>(0);
  }

  Nan::MaybeLocal<v8::Value> maybeData
    = inputPhrase->Get(Nan::New("data").ToLocalChecked());
  v8::Local<v8::Array> vdata
    = v8::Local<v8::Array>::Cast(maybeData.ToLocalChecked());
  if(!vdata->IsArray()) {
    vdata = Nan::New<v8::Array>(0);
  }

  Nan::MaybeLocal<v8::Value> maybeDataInput
    = inputPhrase->Get(Nan::New("data_input").ToLocalChecked());
  v8::Local<v8::Array> vdata_input
    = v8::Local<v8::Array>::Cast(maybeDataInput.ToLocalChecked());
  if(!vdata_input->IsArray()) {
    vdata_input = Nan::New<v8::Array>(0);
  }

  Nan::MaybeLocal<v8::Value> maybeDataOutput
    = inputPhrase->Get(Nan::New("data_output").ToLocalChecked());
  v8::Local<v8::Array> vdata_output
    = v8::Local<v8::Array>::Cast(maybeDataOutput.ToLocalChecked());
  if(!vdata_output->IsArray()) {
    vdata_output = Nan::New<v8::Array>(0);
  }

  bool bimodal = (vbimodal->IsBoolean() ? vbimodal->BooleanValue() : false);
  int dimension = (vdimension->IsNumber() ? vdimension->NumberValue() : 1);
  int dimension_input = (vdimension_input->IsNumber() ? vdimension_input->NumberValue() : 0);
  int length = (vlength->IsNumber() ? vlength->NumberValue() : 0);
  std::string label = "";
  if(vlabel->IsString()) {
    v8::String::Utf8Value val(vlabel->ToString());
    label = std::string(*val);
  }

  // ============== BUILD JSON::VALUE ============ //
  
  Json::Value jp;

  jp["bimodal"] = bimodal;
  jp["dimension"] = static_cast<int>(dimension);
  jp["dimension_input"] = static_cast<int>(dimension_input);
  jp["length"] = static_cast<int>(length);
  jp["label"] = label;

  for(int i=0; i<dimension; i++) {
    if(i<vcolumn_names->Length()
      && vcolumn_names->Get(i)->IsString()) {

      v8::String::Utf8Value val(vcolumn_names->Get(i)->ToString());
      jp["column_names"][i] = std::string(*val);
    } else {
      jp["column_names"][i] = "";
    }
  }

  for(int i=0; i<vdata->Length(); i++) {
    if(vdata->Get(i)->IsNumber()) {
      jp["data"][i] = vdata->Get(i)->NumberValue();
    }
  }
  jp["data_input"] = Json::Value(Json::arrayValue);
  for(int i=0; i<vdata_input->Length(); i++) {
    if(vdata_input->Get(i)->IsNumber()) {
      jp["data_input"][i] = vdata_input->Get(i)->NumberValue();
    }
  }
  jp["data_output"] = Json::Value(Json::arrayValue);
  for(int i=0; i<vdata_output->Length(); i++) {
    if(vdata_output->Get(i)->IsNumber()) {
      jp["data_output"][i] = vdata_output->Get(i)->NumberValue();
    }
  }

  // check for possible errors in xmm::Phrase creation and ...

  if((!bimodal && dimension * length == jp["data"].size()) ||
      (bimodal && (dimension - dimension_input) * length == jp["data_output"].size()
      && dimension_input * length == jp["data_input"].size())) {
    xp.fromJson(jp);
    return true;
  }
  return false;
}

#endif /* _XMMWRAP_UTILS_H_ */