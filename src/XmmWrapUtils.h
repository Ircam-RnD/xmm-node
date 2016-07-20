#ifndef _XMMWRAP_UTILS_H_
#define _XMMWRAP_UTILS_H_

#include <nan.h>
#include "XmmWrap.h"

// TODO : PUT THESE GETTERS / SETTERS IN XMMTOOLBASE

template<typename T>
std::size_t getStates(XmmWrap *obj) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	std::size_t states = mod->model.configuration.states.get();
	return states;
}

template<typename T>
void setStates(XmmWrap *obj, std::size_t states) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	mod->model.configuration.states.set(states);
	mod->model.configuration.changed = true;
}

template<typename T>
std::size_t getGaussians(XmmWrap *obj) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	std::size_t gaussians = mod->model.configuration.gaussians.get();
	return gaussians;
}

template<typename T>
void setGaussians(XmmWrap *obj, std::size_t gaussians) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	mod->model.configuration.gaussians.set(gaussians);
	mod->model.configuration.changed = true;
}

template<typename T>
double getRelativeRegularization(XmmWrap *obj) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	double relative_regularization = mod->model.configuration.relative_regularization.get();
	return relative_regularization;
}

template<typename T>
void setRelativeRegularization(XmmWrap *obj, double relReg) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	mod->model.configuration.relative_regularization.set(relReg);
	mod->model.configuration.changed = true;
}

template<typename T>
double getAbsoluteRegularization(XmmWrap *obj) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	double absolute_regularization = mod->model.configuration.absolute_regularization.get();
	return absolute_regularization;
}

template<typename T>
void setAbsoluteRegularization(XmmWrap *obj, double absReg) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	mod->model.configuration.absolute_regularization.set(absReg);
	mod->model.configuration.changed = true;
}

template<typename T>
xmm::GaussianDistribution::CovarianceMode getCovarianceMode(XmmWrap *obj) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	xmm::GaussianDistribution::CovarianceMode covariance_mode = mod->model.configuration.covariance_mode.get();
	return covariance_mode;
}

template<typename T>
void setCovarianceMode(XmmWrap *obj, xmm::GaussianDistribution::CovarianceMode covmode) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	mod->model.configuration.covariance_mode.set(covmode);
	mod->model.configuration.changed = true;
}

template<typename T>
xmm::HMM::TransitionMode getTransitionMode(XmmWrap *obj) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	xmm::HMM::TransitionMode transmode = mod->model.configuration.transition_mode.get();
	return transmode;
}

template<typename T>
void setTransitionMode(XmmWrap *obj, xmm::HMM::TransitionMode transmode) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	mod->model.configuration.transition_mode.set(transmode);
	mod->model.configuration.changed = true;
}

template<typename T>
xmm::HMM::RegressionEstimator getRegressionEstimator(XmmWrap *obj) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	xmm::HMM::RegressionEstimator regestim = mod->model.configuration.regression_estimator.get();
	return regestim;
}

template<typename T>
void setRegressionEstimator(XmmWrap *obj, xmm::HMM::RegressionEstimator regestim) {
	XmmTool<T> *mod = reinterpret_cast<XmmTool<T> *>(obj->model_);
	mod->model.configuration.regression_estimator.set(regestim);
	mod->model.configuration.changed = true;
}

//=================== xmm to v8::Object converters : ==================//

// TODO : write a generic converter between jsoncpp and v8 !!!!!!!!!!!
// --> DONE (see JsonCppV8Converters.h)
/*
v8::Local<v8::Object> makeObjectGMM(xmm::Model xm) {
	return Nan::New<v8::Object>();
}

v8::Local<v8::Object> makeObjectConfiguration(xmm::Model xm) {
	return Nan::New<v8::Object>();
}

v8::Local<v8::Object> makeObjectSingleClassHMM(xmm::SingleClassHMM xhmm) {
	return Nan::New<v8::Object>();
}

v8::Local<v8::Object> makeObjectSingleClassGMM(xmm::SingleClassGMM xgmm) {
	return Nan::New<v8::Object>();
}

v8::Local<v8::Object> makeObjectSharedParameters(xmm::SharedParameters xsp) {
	return Nan::New<v8::Object>();	
}

v8::Local<v8::Object> makeObjectSet(xmm::TrainingSet xs) {
	return Nan::New<v8::Object>();
}
*/

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

//=================== v8::Object to xmm converters : =====================//

// obsolete since JsonCppV8Converters:
/*
bool makeXmmSet(v8::Local<v8::Object> inputSet, xmm::TrainingSet &xs) {
	return false;
}
//*/

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

	//xmm::Phrase xp;
	if((!bimodal && dimension * length == jp["data"].size()) ||
			(bimodal && (dimension - dimension_input) * length == jp["data_output"].size()
			&& dimension_input * length == jp["data_input"].size())) {

		//xp = xmm::Phrase(jp);
		xp.fromJson(jp);
		return true;
	}
	return false;
}

// translates naive index into real index (taking freeIndex into account) :
// useless because indices don't exist anymore after phrases are removed
// int translateIndex(XmmWrap *obj, int index) {
// 	int fi;
// 	for(fi=0; fi < obj->freeList.size() && obj->freeList[fi] < index; fi++);
// 	return (index + fi - 1);
// }

// ================================================================ //
// Utility for AsyncWorker ... ?

//class XmmModelWrap : public Nan::ObjectWrap {};

#endif /* _XMMWRAP_UTILS_H_ */