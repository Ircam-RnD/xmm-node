#include <nan.h>
#include "XmmWrap.h"
#include "XmmWrapUtils.h"
#include "XmmWrapTrainWorker.h"
#include "JsonCppV8Converters.h"

//=================== CONSTRUCTOR / DESTRUCTOR / INIT / NEW ==================//

XmmWrap::XmmWrap() {
	set_ = new xmm::TrainingSet();
};

XmmWrap::~XmmWrap() {
	delete set_;
	if (modelType_ != XmmUndefinedModelE) {
		delete model_;
	}
};

Nan::Persistent<v8::Function> XmmWrap::constructor;

void XmmWrap::Init() {
	Nan::HandleScope scope;

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("xmm").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Nan::SetPrototypeMethod(tpl, "addPhrase", 						addPhrase);
	Nan::SetPrototypeMethod(tpl, "getPhrase", 						getPhrase);
	Nan::SetPrototypeMethod(tpl, "getPhrasesOfLabel",			getPhrasesOfLabel);
	Nan::SetPrototypeMethod(tpl, "removePhrase", 					removePhrase);
	Nan::SetPrototypeMethod(tpl, "removePhrasesOfLabel", 	removePhrasesOfLabel);
	Nan::SetPrototypeMethod(tpl, "getTrainingSetSize", 		getTrainingSetSize);
	Nan::SetPrototypeMethod(tpl, "getTrainingSetLabels", 	getTrainingSetLabels);
	Nan::SetPrototypeMethod(tpl, "setTrainingSet", 				setTrainingSet);
	Nan::SetPrototypeMethod(tpl, "addTrainingSet", 				addTrainingSet);
	Nan::SetPrototypeMethod(tpl, "getTrainingSet", 				getTrainingSet);
	Nan::SetPrototypeMethod(tpl, "clearTrainingSet", 			clearTrainingSet);
	Nan::SetPrototypeMethod(tpl, "train", 								train);
	Nan::SetPrototypeMethod(tpl, "getModel", 							getModel);
	Nan::SetPrototypeMethod(tpl, "getModelType", 					getModelType);
	Nan::SetPrototypeMethod(tpl, "filter", 								filter);

	/*
	// other way to write this with v8 :
	tpl->PrototypeTemplate()->Set(Nan::New("addPhrase").ToLocalChecked(),
	  Nan::New<v8::FunctionTemplate>(addPhrase)->GetFunction());
	//*/

	// Getters / setters :
	// solution found here :
	// http://stackoverflow.com/questions/33383493/how-do-i-update-my-function-to-use-the-new-v8-functiontemplates

	// Nan::SetAccessor(
	// 	tpl->InstanceTemplate(),
	// 	Nan::New("config").ToLocalChecked(),
	// 	getConfig, setConfig
	// );

	// now replaced by regular methods :
	Nan::SetPrototypeMethod(tpl, "getConfig",	getConfig);
	Nan::SetPrototypeMethod(tpl, "setConfig",	setConfig);

	constructor.Reset(tpl->GetFunction());
}

void XmmWrap::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	XmmWrap* obj = new XmmWrap();
  	// deal with arguments
  	// obj->counter_ = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();

	std::string modelType = "gmm";
	obj->modelType_ = XmmGmmE;

	bool h = true;
	std::size_t s = 1;
	std::size_t g = 1;
	double rr = 0.01;
	double ar = 0.01;
	xmm::GaussianDistribution::CovarianceMode cm = xmm::GaussianDistribution::CovarianceMode::Full;
	xmm::HMM::TransitionMode tm = xmm::HMM::TransitionMode::LeftRight;
	xmm::HMM::RegressionEstimator re = xmm::HMM::RegressionEstimator::Full; // use Windowed as a compromise between LR / ergodic ?

	if (info.Length() > 0 && info[0]->IsString()) {
		v8::String::Utf8Value val(info[0]->ToString());
		modelType = std::string(*val);

		// this doesn't work for some (undocumented) reason ... 
		// it seems that the New function always takes into account only 1 arg
		// info.Length() is always 1

		if (info.Length() > 1 && info[1]->IsObject()) {
			v8::Local<v8::Object> initConfig = v8::Local<v8::Object>::Cast(info[1]);

			v8::Local<v8::Value> hierarchical
				= initConfig->Get(Nan::New<v8::String>("hierarchical").ToLocalChecked());
			if (hierarchical->IsBoolean()) {
				h = hierarchical->BooleanValue();
			}

			v8::Local<v8::Value> states
				= initConfig->Get(Nan::New<v8::String>("states").ToLocalChecked());
			if (states->IsNumber()) {
				s = states->NumberValue();
			}

			v8::Local<v8::Value> gaussians
				= initConfig->Get(Nan::New<v8::String>("gaussians").ToLocalChecked());
			if (gaussians->IsNumber()) {
				g = gaussians->NumberValue();
			}

			v8::Local<v8::Value> relative_regularization
				= initConfig->Get(Nan::New<v8::String>("relative_regularization").ToLocalChecked());
			if (relative_regularization->IsNumber()) {
				rr = relative_regularization->NumberValue();
			}

			v8::Local<v8::Value> absolute_regularization
				= initConfig->Get(Nan::New<v8::String>("absolute_regularization").ToLocalChecked());
			if (absolute_regularization->IsNumber()) {
				ar = absolute_regularization->NumberValue();
			}

			v8::Local<v8::Value> covariance_mode
				= initConfig->Get(Nan::New<v8::String>("covariance_mode").ToLocalChecked());
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
				= initConfig->Get(Nan::New<v8::String>("transition_mode").ToLocalChecked());
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
				= initConfig->Get(Nan::New<v8::String>("regression_estimator").ToLocalChecked());
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

		}
	}

	/*
	//========================== OLD INITIALIZATION ============================//

	if (info[0]->IsObject()) {
		v8::Local<v8::Object> initConfig = v8::Local<v8::Object>::Cast(info[0]);

		v8::Local<v8::Value> hierarchical
			= initConfig->Get(Nan::New<v8::String>("hierarchical").ToLocalChecked());
		if (hierarchical->IsBoolean()) {
			h = hierarchical->BooleanValue();
		}

		v8::Local<v8::Value> states
			= initConfig->Get(Nan::New<v8::String>("states").ToLocalChecked());
		if (states->IsNumber()) {
			s = states->NumberValue();
		}

		v8::Local<v8::Value> gaussians
			= initConfig->Get(Nan::New<v8::String>("gaussians").ToLocalChecked());
		if (gaussians->IsNumber()) {
			g = gaussians->NumberValue();
		}

		v8::Local<v8::Value> relative_regularization
			= initConfig->Get(Nan::New<v8::String>("relative_regularization").ToLocalChecked());
		if (relative_regularization->IsNumber()) {
			rr = relative_regularization->NumberValue();
		}

		v8::Local<v8::Value> absolute_regularization
			= initConfig->Get(Nan::New<v8::String>("absolute_regularization").ToLocalChecked());
		if (absolute_regularization->IsNumber()) {
			ar = absolute_regularization->NumberValue();
		}

		v8::Local<v8::Value> covariance_mode
			= initConfig->Get(Nan::New<v8::String>("covariance_mode").ToLocalChecked());
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
			= initConfig->Get(Nan::New<v8::String>("transition_mode").ToLocalChecked());
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
			= initConfig->Get(Nan::New<v8::String>("regression_estimator").ToLocalChecked());
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

		v8::Local<v8::Value> model
			= initConfig->Get(Nan::New<v8::String>("model").ToLocalChecked());
		if (model->IsString()) {
			v8::String::Utf8Value val(model->ToString());
			modelType = std::string(*val);
		}

		//obj->model_.model.configuration.gaussians.set(ng);
		//obj->model_.model.configuration.changed = true;
	} else if (info[0]->IsString()) {
		v8::String::Utf8Value val(info[0]->ToString());
		modelType = std::string(*val);
	}
	//*/

	//=========================== CREATE MODEL OBJECT ==========================//

	if (modelType == "gmm") {
		obj->model_ = new XmmTool<xmm::GMM>();
		obj->modelType_ = XmmGmmE;
		setGaussians<xmm::GMM>(obj, g);	
		setRelativeRegularization<xmm::GMM>(obj, rr);
		setAbsoluteRegularization<xmm::GMM>(obj, ar);
		setCovarianceMode<xmm::GMM>(obj, cm);
	} else if (modelType == "hhmm") {
		obj->model_ = new XmmTool<xmm::HierarchicalHMM>();
		obj->modelType_ = XmmHhmmE;
		setHierarchical<xmm::HierarchicalHMM>(obj, h);
		setStates<xmm::HierarchicalHMM>(obj, s);
		setGaussians<xmm::HierarchicalHMM>(obj, g);	
		setRelativeRegularization<xmm::HierarchicalHMM>(obj, rr);
		setAbsoluteRegularization<xmm::HierarchicalHMM>(obj, ar);
		setCovarianceMode<xmm::HierarchicalHMM>(obj, cm);
		setTransitionMode<xmm::HierarchicalHMM>(obj, tm);
		setRegressionEstimator<xmm::HierarchicalHMM>(obj, re);
	} else {
		//obj->model_ = new XmmTool<xmm::GMM>();
		obj->modelType_ = XmmUndefinedModelE;
	}

  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> XmmWrap::NewInstance(v8::Local<v8::Value> arg) {
	Nan::EscapableHandleScope scope;

	const unsigned int argc = 1;
	v8::Local<v8::Value> argv[argc] = { arg };
	v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
	v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

	return scope.Escape(instance);
}

//=============================== CLASS METHODS ==============================//

void XmmWrap::addPhrase(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());
	Nan::MaybeLocal<v8::Value> maybeInputPhrase = args[0];
	v8::Local<v8::Object> inputPhrase
		= v8::Local<v8::Object>::Cast(maybeInputPhrase.ToLocalChecked());
	if (!inputPhrase->IsObject()) return;

	xmm::Phrase xp;
	if (makeXmmPhrase(inputPhrase, xp)) {
		int index;
		if (obj->freeList.size() == 0) {
			index = obj->set_->size();
		} else {
			index = obj->freeList.back();
			obj->freeList.pop_back();
		}

		//xmm::Phrase xp(jp);
		if (obj->set_->size() == 0) {
			delete obj->set_;
			if (xp.bimodal()) {
				obj->set_ = new xmm::TrainingSet(xmm::MemoryMode::OwnMemory,
																				 xmm::Multimodality::Bimodal);
				obj->set_->dimension_input.set(xp.dimension_input.get());
				obj->model_->setBimodal(true);
			} else {
				obj->set_ = new xmm::TrainingSet();
				obj->model_->setBimodal(false);
			}
			obj->set_->dimension.set(xp.dimension.get());
			obj->set_->column_names.set(xp.column_names, true);
		}
		obj->set_->addPhrase(index, xp);

		args.GetReturnValue().Set(Nan::New(index));
	} else {
		//args.GetReturnValue().Set(Nan::New(false));		
	}
}

void XmmWrap::getPhrasesOfLabel(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());
	Nan::MaybeLocal<v8::Value> maybeLabel = args[0];
	v8::Local<v8::Value> vlabel = maybeLabel.ToLocalChecked();

	if (vlabel->IsString()) {
		v8::String::Utf8Value val(vlabel->ToString());
		std::string label = std::string(*val);

		xmm::TrainingSet *s = obj->set_->getPhrasesOfClass(label);
		Json::Value js;

		if (s != nullptr) {
			js = s->toJson();
		}

		args.GetReturnValue().Set(valueToObject(js));		
	}
}

void XmmWrap::getPhrase(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	// getIndex and return corresponding phrase as JS object
	Nan::MaybeLocal<v8::Value> maybePIndex = args[0];
	v8::Local<v8::Value> vpindex;
	if (!maybePIndex.IsEmpty()) {
		vpindex = maybePIndex.ToLocalChecked();
	}
	int pindex = (vpindex->IsNumber() ? vpindex->NumberValue() : -1);
	//pindex = translateIndex(obj, pindex);

	for (int i=0; i<obj->freeList.size(); i++) {
		if (pindex == obj->freeList[i]) return;
	}

	if (pindex < obj->set_->size() + obj->freeList.size()) {
		std::shared_ptr<xmm::Phrase> xp = obj->set_->getPhrase(pindex);
		//Json::Value jp = xp->toJson();
		args.GetReturnValue().Set(makeObjectPhrase(*xp));
		//args.GetReturnValue().Set(valueToObject(jp));
	} else {
		args.GetReturnValue().Set(Nan::New(false)); // if phrase doesn't exist return false
	}
}

void XmmWrap::removePhrase(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	// get index and return corresponding phrase as JS object
	Nan::MaybeLocal<v8::Value> maybePIndex = args[0];
	v8::Local<v8::Value> vpindex;
	if (!maybePIndex.IsEmpty()) {
		vpindex = maybePIndex.ToLocalChecked();
	}
	int pindex = (vpindex->IsNumber() ? vpindex->NumberValue() : -1);

	for (int i=0; i<obj->freeList.size(); i++) {
		if (pindex == obj->freeList[i]) {
			args.GetReturnValue().Set(Nan::New(false));
			return;
		}
	}

	obj->freeList.push_back(pindex);
	std::sort(obj->freeList.begin(), obj->freeList.end(), std::greater<int>());
	obj->set_->removePhrase(pindex);

	args.GetReturnValue().Set(Nan::New(true));
}

void XmmWrap::removePhrasesOfLabel(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	Nan::MaybeLocal<v8::Value> maybeLabel = args[0];
	v8::Local<v8::Value> vlabel = maybeLabel.ToLocalChecked();

	if (vlabel->IsString()) {
		v8::String::Utf8Value val(vlabel->ToString());
		std::string label = std::string(*val);

		xmm::TrainingSet *s = obj->set_->getPhrasesOfClass(label);
		for (auto const &phrase : *s) {
			obj->freeList.push_back(phrase.first);
		}
		std::sort(obj->freeList.begin(), obj->freeList.end(), std::greater<int>());
		obj->set_->removePhrasesOfClass(label);
	}
}

void XmmWrap::getTrainingSetSize(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());
	args.GetReturnValue().Set(Nan::New(static_cast<int>(obj->set_->size())));
}

void XmmWrap::getTrainingSetLabels(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());
	std::set<std::string> slabels = obj->set_->labels();

	std::size_t nlabels = slabels.size();
	v8::Local<v8::Array> labels = Nan::New<v8::Array>(nlabels);
	std::size_t i = 0;
	for (auto it : slabels) {
		Nan::Set(labels, i, Nan::New<v8::String>(it).ToLocalChecked());
		++i;
	}
	args.GetReturnValue().Set(labels);
}

void XmmWrap::getTrainingSet(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());
	Json::Value js = obj->set_->toJson();
	args.GetReturnValue().Set(valueToObject(js));
}

void XmmWrap::setTrainingSet(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());
	Nan::MaybeLocal<v8::Value> maybeInputSet = args[0];
	v8::Local<v8::Object> inputSet
		= v8::Local<v8::Object>::Cast(maybeInputSet.ToLocalChecked());
	if (!inputSet->IsObject()) return;

	obj->set_->fromJson(objectToValue(inputSet));
}

void XmmWrap::addTrainingSet(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());
	//args.GetReturnValue().Set(Nan::New(obj->set_->size()));
	// TODO
	Nan::MaybeLocal<v8::Value> maybeInputSet = args[0];
	v8::Local<v8::Object> inputSet
		= v8::Local<v8::Object>::Cast(maybeInputSet.ToLocalChecked());
	if (!inputSet->IsObject()) return;

	xmm::TrainingSet xs(objectToValue(inputSet));
	for (auto xp : xs) {
		int index;
		if (obj->freeList.size() == 0) {
			index = obj->set_->size();
		} else {
			index = obj->freeList.back();
			obj->freeList.pop_back();
		}

		//xmm::Phrase xp(jp);
		if (obj->set_->size() == 0) {
			obj->set_->dimension.set(xp.second->dimension.get());
			obj->set_->column_names.set(xp.second->column_names, true);
		}
		obj->set_->addPhrase(index, xp.second);
	}
}

void XmmWrap::clearTrainingSet(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());
	obj->set_->clear();
	obj->freeList.clear();
}

//================================= TRAINING =================================//
//========================== THIS IS A CALL (BACK) ===========================//

void XmmWrap::train(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	/*
	// ============= synchronous callback ============ //
	obj->model_->train(obj->set_);
	while(obj->model_->training());

	v8::Local<v8::Function> cb = args[0].As<v8::Function>();
	const unsigned argc = 1;
	v8::Local<v8::Value> argv[argc] = { Nan::New("finished training").ToLocalChecked() };

	Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
	//*/

	Nan::Callback *callback = new Nan::Callback(args[0].As<v8::Function>());

	switch(obj->modelType_) {
		case XmmGmmE: {
			Nan::AsyncQueueWorker(new XmmWrapTrainWorker<xmm::GMM>(
				callback, *reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_), *obj->set_
			));
			break;
		}

		case XmmHhmmE: {
			Nan::AsyncQueueWorker(new XmmWrapTrainWorker<xmm::HierarchicalHMM>(
				callback, *reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_), *obj->set_
			));
			break;
		}

		default:
			break;
	}
}

void XmmWrap::getModel(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	Json::Value jm;
	switch(obj->modelType_) {
		case XmmGmmE:
 			jm = reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_)->model.toJson();
 			break;
 		case XmmHhmmE:
 			jm = reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_)->model.toJson();
 			break;
 		default:
 			break;
 	}

	args.GetReturnValue().Set(valueToObject(jm));
}

void XmmWrap::getModelType(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	std::string stype = "unknown";
	switch (obj->modelType_) {
		case XmmGmmE:
			stype = "gmm";
			break;

		case XmmHhmmE:
			stype = "hhmm";
			break;

		default:
			break;
	}

	args.GetReturnValue().Set(Nan::New<v8::String>(stype).ToLocalChecked());
}

//================================ FILTERING =================================//

void XmmWrap::reset(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	if (obj->modelType_ == XmmGmmE) {
		reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_)->model.reset();
	} else if (obj->modelType_ == XmmHhmmE) {
		reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_)->model.reset();
	} // else do nothing
}

void XmmWrap::filter(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	v8::Local<v8::Object> outputResults = Nan::New<v8::Object>();

	// args must be an array of size dimension / dimension_input
	Nan::MaybeLocal<v8::Value> maybeInputObs = args[0];
	v8::Local<v8::Array> inputObs
		= v8::Local<v8::Array>::Cast(maybeInputObs.ToLocalChecked());
	if (!inputObs->IsArray()) return;

	std::vector<float> observation;
	for (unsigned int i = 0; i < inputObs->Length(); ++i) {
		observation.push_back(inputObs->Get(i)->NumberValue());
	}

  bool bimodal;
  unsigned int nmodels;
  unsigned int dimension;
  unsigned int dimension_input;
  unsigned int dimension_output;

  if (obj->modelType_ == XmmGmmE) {
		bimodal =
		reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_)->model.shared_parameters->bimodal.get();
  	nmodels =
  	reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_)->model.size();
		dimension =
		reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_)->model.shared_parameters->dimension.get();
		dimension_input =
		reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_)->model.shared_parameters->dimension_input.get();
		dimension_output = dimension - dimension_input;
  } else if (obj->modelType_ == XmmHhmmE) {
		bimodal =
		reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_)->model.shared_parameters->bimodal.get();
  	nmodels =
  	reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_)->model.size();
		dimension =
		reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_)->model.shared_parameters->dimension.get();
		dimension_input =
		reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_)->model.shared_parameters->dimension_input.get();
		dimension_output = dimension - dimension_input;
  } else {
  	bimodal = false;
  	nmodels = 0;
  	dimension = 0;
  	dimension_input = 0;
  	dimension_output = 0;
  }

	v8::Local<v8::Array> instant_likelihoods = Nan::New<v8::Array>(nmodels);
	v8::Local<v8::Array> instant_normalized_likelihoods = Nan::New<v8::Array>(nmodels);
	v8::Local<v8::Array> smoothed_likelihoods = Nan::New<v8::Array>(nmodels);
	v8::Local<v8::Array> smoothed_normalized_likelihoods = Nan::New<v8::Array>(nmodels);
	v8::Local<v8::Array> smoothed_log_likelihoods = Nan::New<v8::Array>(nmodels);

	// stupid template system force too much code duplication ... avoid this later

	//obj->modelType_ = XmmUndefinedModelE;

	//======================================================================== GMM
	if (obj->modelType_ == XmmGmmE) {

		reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_)->model.filter(observation);
		xmm::Results<xmm::GMM> res = reinterpret_cast<XmmTool<xmm::GMM> *>(obj->model_)->model.results;

		for (std::size_t i = 0; i < nmodels; ++i) {
			Nan::Set(instant_likelihoods, i, Nan::New(res.instant_likelihoods[i]));
			Nan::Set(instant_normalized_likelihoods, i, Nan::New(res.instant_normalized_likelihoods[i]));
			Nan::Set(smoothed_likelihoods, i, Nan::New(res.smoothed_likelihoods[i]));
			Nan::Set(smoothed_normalized_likelihoods, i, Nan::New(res.smoothed_normalized_likelihoods[i]));
			Nan::Set(smoothed_log_likelihoods, i, Nan::New(res.smoothed_log_likelihoods[i]));
		}

		outputResults->Set(
			Nan::New<v8::String>("likeliest").ToLocalChecked(),
			Nan::New<v8::String>(res.likeliest).ToLocalChecked()
		);

		if (bimodal) {
			v8::Local<v8::Array> output_values = Nan::New<v8::Array>(dimension_output);
			for (unsigned int i = 0; i < dimension_output; ++i) { 
				Nan::Set(output_values, i, Nan::New(res.output_values[i]));
			}
			outputResults->Set(
				Nan::New<v8::String>("output_values").ToLocalChecked(),
				output_values
			);

			unsigned int dim_out_cov = res.output_covariance.size();
			v8::Local<v8::Array> output_covariance = Nan::New<v8::Array>(dim_out_cov);
			for (unsigned int i = 0; i < dim_out_cov; ++i) { 
				Nan::Set(output_covariance, i, Nan::New(res.output_covariance[i]));
			}
			outputResults->Set(
				Nan::New<v8::String>("output_covariance").ToLocalChecked(),
				output_covariance
			);
		}

	//======================================================================= HHMM
	} else if (obj->modelType_ == XmmHhmmE) {

		reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_)->model.filter(observation);
		xmm::Results<xmm::HMM> res =
			reinterpret_cast<XmmTool<xmm::HierarchicalHMM> *>(obj->model_)->model.results;

		for (std::size_t i = 0; i < nmodels; ++i) {
			Nan::Set(instant_likelihoods, i, Nan::New(res.instant_likelihoods[i]));
			Nan::Set(instant_normalized_likelihoods, i, Nan::New(res.instant_normalized_likelihoods[i]));
			Nan::Set(smoothed_likelihoods, i, Nan::New(res.smoothed_likelihoods[i]));
			Nan::Set(smoothed_normalized_likelihoods, i, Nan::New(res.smoothed_normalized_likelihoods[i]));
			Nan::Set(smoothed_log_likelihoods, i, Nan::New(res.smoothed_log_likelihoods[i]));
		}

		outputResults->Set(
			Nan::New<v8::String>("likeliest").ToLocalChecked(),
			Nan::New<v8::String>(res.likeliest).ToLocalChecked()
		);

		if (bimodal) {
			unsigned int dim_out_val = res.output_values.size();
			v8::Local<v8::Array> output_values = Nan::New<v8::Array>(dim_out_val);
			for (unsigned int i = 0; i < dim_out_val; ++i) { 
				Nan::Set(output_values, i, Nan::New(res.output_values[i]));
			}
			outputResults->Set(
				Nan::New<v8::String>("output_values").ToLocalChecked(),
				output_values
			);

			unsigned int dim_out_cov = res.output_covariance.size();
			v8::Local<v8::Array> output_covariance = Nan::New<v8::Array>(dim_out_cov);
			for (unsigned int i = 0; i < dim_out_cov; ++i) { 
				Nan::Set(output_covariance, i, Nan::New(res.output_covariance[i]));
			}
			outputResults->Set(
				Nan::New<v8::String>("output_covariance").ToLocalChecked(),
				output_covariance
			);
		}

	} else { // /* do nothing */ }
			// outputResults->Set(
			// 	Nan::New<v8::String>("obs_length").ToLocalChecked(),
			// 	Nan::New(static_cast<int>(observation.size()))
			// );
	}

	outputResults->Set(
		Nan::New<v8::String>("instant_likelihoods").ToLocalChecked(),
		instant_likelihoods
	);
	outputResults->Set(
		Nan::New<v8::String>("instant_normalized_likelihoods").ToLocalChecked(),
		instant_normalized_likelihoods
	);
	outputResults->Set(
		Nan::New<v8::String>("smoothed_likelihoods").ToLocalChecked(),
		smoothed_likelihoods
	);
	outputResults->Set(
		Nan::New<v8::String>("smoothed_normalized_likelihoods").ToLocalChecked(),
		smoothed_normalized_likelihoods
	);
	outputResults->Set(
		Nan::New<v8::String>("smoothed_log_likelihoods").ToLocalChecked(),
		smoothed_log_likelihoods
	);

	// then, output !
	args.GetReturnValue().Set(outputResults);
}

//============================= CONFIGURATION ================================//

void XmmWrap::getConfig(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.Holder());

	bool hierarchical = true;
	std::size_t states = 1;
	std::size_t gaussians = 1;
	double relative_regularization = 0.01;
	double absolute_regularization = 0.01;
	xmm::GaussianDistribution::CovarianceMode covariance_mode = xmm::GaussianDistribution::CovarianceMode::Full;
	xmm::HMM::TransitionMode transition_mode = xmm::HMM::TransitionMode::LeftRight;
	xmm::HMM::RegressionEstimator regression_estimator = xmm::HMM::RegressionEstimator::Full;

	if (obj->modelType_ == XmmGmmE) {
		gaussians = getGaussians<xmm::GMM>(obj);
		relative_regularization = getRelativeRegularization<xmm::GMM>(obj);
		absolute_regularization = getAbsoluteRegularization<xmm::GMM>(obj);
		covariance_mode = getCovarianceMode<xmm::GMM>(obj);
	} else if (obj->modelType_ == XmmHhmmE) {
		hierarchical = getHierarchical<xmm::HierarchicalHMM>(obj);
		states = getStates<xmm::HierarchicalHMM>(obj);
		gaussians = getGaussians<xmm::HierarchicalHMM>(obj);
		relative_regularization = getRelativeRegularization<xmm::HierarchicalHMM>(obj);
		absolute_regularization = getAbsoluteRegularization<xmm::HierarchicalHMM>(obj);
		covariance_mode = getCovarianceMode<xmm::HierarchicalHMM>(obj);
		transition_mode = getTransitionMode<xmm::HierarchicalHMM>(obj);
		regression_estimator = getRegressionEstimator<xmm::HierarchicalHMM>(obj);
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

	std::string modelType;
	switch (obj->modelType_) {
		case XmmGmmE:
			modelType = "gmm";
			break;

		case XmmHhmmE:
			modelType = "hhmm";
			break;

		default:
			modelType = "unknown";
			break;
	}

	// generate return value according to arguments

	if (args.Length() > 0) {
		Nan::MaybeLocal<v8::Value> maybeItem = args[0];
		v8::Local<v8::Value> vitem = maybeItem.ToLocalChecked();
		if (!vitem->IsString()) return;

		v8::String::Utf8Value val(vitem->ToString());
		std::string item = std::string(*val);

		if (item == "model") {
			// now the model type has its own method to be retrieved
			// args.GetReturnValue().Set(Nan::New<v8::String>(modelType).ToLocalChecked());
		} else if (item == "gaussians") {
			args.GetReturnValue().Set(Nan::New<v8::Number>(gaussians));
		} else if (item == "relative_regularization") {
			args.GetReturnValue().Set(Nan::New<v8::Number>(relative_regularization));
		} else if (item == "absolute_regularization") {
			args.GetReturnValue().Set(Nan::New<v8::Number>(absolute_regularization));
		} else if (item == "covariance_mode") {
			args.GetReturnValue().Set(Nan::New<v8::String>(cm).ToLocalChecked());
		}

		if (obj->modelType_ == XmmHhmmE) {
			if (item == "hierarchical") {
				args.GetReturnValue().Set(Nan::New<v8::Boolean>(hierarchical));
			} else if (item == "states") {
				args.GetReturnValue().Set(Nan::New<v8::Number>(states));
			} else if (item == "transition_mode") {
				args.GetReturnValue().Set(Nan::New<v8::String>(tm).ToLocalChecked());
			} else if (item == "regression_estimator") {
				args.GetReturnValue().Set(Nan::New<v8::String>(re).ToLocalChecked());
			}
		}

	} else {
		v8::Local<v8::Object> outputConfig = Nan::New<v8::Object>();

		// now the model type has its own method to be retrieved
		// outputConfig->Set(Nan::New<v8::String>("model").ToLocalChecked(),
		//  					Nan::New<v8::String>(modelType).ToLocalChecked());
		outputConfig->Set(Nan::New<v8::String>("gaussians").ToLocalChecked(),
		 					Nan::New<v8::Number>(gaussians));
		outputConfig->Set(Nan::New<v8::String>("relative_regularization").ToLocalChecked(),
							Nan::New<v8::Number>(relative_regularization));
		outputConfig->Set(Nan::New<v8::String>("absolute_regularization").ToLocalChecked(),
							Nan::New<v8::Number>(absolute_regularization));
		outputConfig->Set(Nan::New<v8::String>("covariance_mode").ToLocalChecked(),
							Nan::New<v8::String>(cm).ToLocalChecked());

		switch (obj->modelType_) {
			case XmmHhmmE: {
				outputConfig->Set(Nan::New<v8::String>("hierarchical").ToLocalChecked(),
									Nan::New<v8::Boolean>(hierarchical));
				outputConfig->Set(Nan::New<v8::String>("states").ToLocalChecked(),
				 					Nan::New<v8::Number>(states));
				outputConfig->Set(Nan::New<v8::String>("transition_mode").ToLocalChecked(),
									Nan::New<v8::String>(tm).ToLocalChecked());
				outputConfig->Set(Nan::New<v8::String>("regression_estimator").ToLocalChecked(),
									Nan::New<v8::String>(re).ToLocalChecked());
				break;
			}

			default:
				break;
		}
		args.GetReturnValue().Set(outputConfig);
	}
}

//============================ GETTERS / SETTERS =============================//

// example code for getters / setters :
// https://github.com/kneth/DemoNodeExtension/blob/master/person_wrap.cpp

/*
void XmmWrap::getConfig(v8::Local<v8::String> prop,
						  const Nan::PropertyCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.This());

	v8::Local<v8::Object> outputConfig = Nan::New<v8::Object>();

	bool hierarchical = true;
	std::size_t states = 1;
	std::size_t gaussians = 1;
	double relative_regularization = 0.01;
	double absolute_regularization = 0.01;
	xmm::GaussianDistribution::CovarianceMode covariance_mode = xmm::GaussianDistribution::CovarianceMode::Full;
	xmm::HMM::TransitionMode transition_mode = xmm::HMM::TransitionMode::LeftRight;
	xmm::HMM::RegressionEstimator regression_estimator = xmm::HMM::RegressionEstimator::Full;

	if (obj->modelType_ == XmmGmmE) {
		gaussians = getGaussians<xmm::GMM>(obj);
		relative_regularization = getRelativeRegularization<xmm::GMM>(obj);
		absolute_regularization = getAbsoluteRegularization<xmm::GMM>(obj);
		covariance_mode = getCovarianceMode<xmm::GMM>(obj);
	} else if (obj->modelType_ == XmmHhmmE) {
		hierarchical = getHierarchical<xmm::HierarchicalHMM>(obj);
		states = getStates<xmm::HierarchicalHMM>(obj);
		gaussians = getGaussians<xmm::HierarchicalHMM>(obj);
		relative_regularization = getRelativeRegularization<xmm::HierarchicalHMM>(obj);
		absolute_regularization = getAbsoluteRegularization<xmm::HierarchicalHMM>(obj);
		covariance_mode = getCovarianceMode<xmm::HierarchicalHMM>(obj);
		transition_mode = getTransitionMode<xmm::HierarchicalHMM>(obj);
		regression_estimator = getRegressionEstimator<xmm::HierarchicalHMM>(obj);
	}

	std::string modelType;
	switch (obj->modelType_) {
		case XmmGmmE:
			modelType = "gmm";
			break;

		case XmmHhmmE:
			modelType = "hhmm";
			break;

		default:
			modelType = "unknown";
			break;
	}

	outputConfig->Set(Nan::New<v8::String>("model").ToLocalChecked(),
	 					Nan::New<v8::String>(modelType).ToLocalChecked());

	outputConfig->Set(Nan::New<v8::String>("gaussians").ToLocalChecked(),
	 					Nan::New<v8::Number>(gaussians));
	outputConfig->Set(Nan::New<v8::String>("relative_regularization").ToLocalChecked(),
						Nan::New<v8::Number>(relative_regularization));
	outputConfig->Set(Nan::New<v8::String>("absolute_regularization").ToLocalChecked(),
						Nan::New<v8::Number>(absolute_regularization));

	std::string cm = (covariance_mode == xmm::GaussianDistribution::CovarianceMode::Full) ? "full"
					: ((covariance_mode == xmm::GaussianDistribution::CovarianceMode::Diagonal) ? "diagonal" : "");
	outputConfig->Set(Nan::New<v8::String>("covariance_mode").ToLocalChecked(),
						Nan::New<v8::String>(cm).ToLocalChecked());

	switch (obj->modelType_) {
		case XmmHhmmE: {
			outputConfig->Set(Nan::New<v8::String>("hierarchical").ToLocalChecked(),
								Nan::New<v8::Boolean>(hierarchical));

			outputConfig->Set(Nan::New<v8::String>("states").ToLocalChecked(),
			 					Nan::New<v8::Number>(states));

			std::string tm = (transition_mode == xmm::HMM::TransitionMode::Ergodic) ? "ergodic"
					: ((transition_mode == xmm::HMM::TransitionMode::LeftRight) ? "leftright" : "");
			outputConfig->Set(Nan::New<v8::String>("transition_mode").ToLocalChecked(),
								Nan::New<v8::String>(tm).ToLocalChecked());

			std::string re = (regression_estimator == xmm::HMM::RegressionEstimator::Full) ? "full"
					: ((regression_estimator == xmm::HMM::RegressionEstimator::Windowed) ? "windowed"
						: ((regression_estimator == xmm::HMM::RegressionEstimator::Likeliest) ? "likeliest" : ""));
			outputConfig->Set(Nan::New<v8::String>("regression_estimator").ToLocalChecked(),
								Nan::New<v8::String>(re).ToLocalChecked());
			break;
		}

		default:
			break;
	}

	args.GetReturnValue().Set(outputConfig);
}
//*/

//============================================================================//

// ye old setter signature, replaced by a regular method signature :

/*
void XmmWrap::setConfig(v8::Local<v8::String> prop,
						  v8::Local<v8::Value> value,
						  const Nan::PropertyCallbackInfo<void> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.This());
	if (value->IsObject()) {

		v8::Local<v8::Object> inputConfig = v8::Local<v8::Object>::Cast(value);
//*/

void XmmWrap::setConfig(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	XmmWrap *obj = ObjectWrap::Unwrap<XmmWrap>(args.This());

	if (args.Length() > 0) {
		Nan::MaybeLocal<v8::Value> maybeInputConfig = args[0];
		v8::Local<v8::Object> inputConfig
			= v8::Local<v8::Object>::Cast(maybeInputConfig.ToLocalChecked());
		if (!inputConfig->IsObject()) return;

		bool h;
		std::size_t s;
		std::size_t g;
		double rr;
		double ar;
		xmm::GaussianDistribution::CovarianceMode cm;
		xmm::HMM::TransitionMode tm;
		xmm::HMM::RegressionEstimator re;

		switch (obj->modelType_) {
			case XmmGmmE:
				g = getGaussians<xmm::GMM>(obj);
				rr = getRelativeRegularization<xmm::GMM>(obj);
				ar = getAbsoluteRegularization<xmm::GMM>(obj);
				cm = getCovarianceMode<xmm::GMM>(obj);
				break;

			case XmmHhmmE:
				h = getHierarchical<xmm::HierarchicalHMM>(obj);
				s = getStates<xmm::HierarchicalHMM>(obj);
				g = getGaussians<xmm::HierarchicalHMM>(obj);
				rr = getRelativeRegularization<xmm::HierarchicalHMM>(obj);
				ar = getAbsoluteRegularization<xmm::HierarchicalHMM>(obj);
				cm = getCovarianceMode<xmm::HierarchicalHMM>(obj);
				tm = getTransitionMode<xmm::HierarchicalHMM>(obj);
				re = getRegressionEstimator<xmm::HierarchicalHMM>(obj);
				break;

			default:
				break;
		}

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

		// =============== SET NEW VALUES ============== //

		if(obj->modelType_ == XmmGmmE) {
			setGaussians<xmm::GMM>(obj, g);
			setRelativeRegularization<xmm::GMM>(obj, rr);
			setAbsoluteRegularization<xmm::GMM>(obj, ar);
			setCovarianceMode<xmm::GMM>(obj, cm);

		} else if (obj->modelType_ == XmmHhmmE) {
			setHierarchical<xmm::HierarchicalHMM>(obj, h);
			setStates<xmm::HierarchicalHMM>(obj, s);
			setGaussians<xmm::HierarchicalHMM>(obj, g);
			setRelativeRegularization<xmm::HierarchicalHMM>(obj, rr);
			setAbsoluteRegularization<xmm::HierarchicalHMM>(obj, ar);
			setCovarianceMode<xmm::HierarchicalHMM>(obj, cm);
			setTransitionMode<xmm::HierarchicalHMM>(obj, tm);
			setRegressionEstimator<xmm::HierarchicalHMM>(obj, re);
		}
	}
}

