#include "phrase.h"

// v8::Persistent<v8::Function> phrase::constructor;
Nan::Persistent<v8::Function> phrase::constructor;

phrase::phrase(bool bimodal)
// : phrase_(xmm::MemoryMode::OwnMemory,
// 	     (bimodal ? xmm::Multimodality::Bimodal
// 		 		  : xmm::Multimodality::Unimodal))
{

	// xmm::Phrase * p = new xmm::Phrase(xmm::MemoryMode::OwnMemory,
	// 					 	 		 (bimodal ? xmm::Multimodality::Bimodal
 // 									  		  : xmm::Multimodality::Unimodal));
	
	phrase_ = new xmm::Phrase(xmm::MemoryMode::OwnMemory,
						 	 (bimodal ? xmm::Multimodality::Bimodal
 									  : xmm::Multimodality::Unimodal));
}

phrase::~phrase() {
	delete phrase_;
}

void phrase::Init(v8::Local<v8::Object> exports) {
//void phrase::Init() {
	Nan::HandleScope scope;

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("phrase").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	Nan::SetPrototypeMethod(tpl, "bimodal",			bimodal);
	Nan::SetPrototypeMethod(tpl, "size",	 		size);
	Nan::SetPrototypeMethod(tpl, "inputsize",		inputsize);
	Nan::SetPrototypeMethod(tpl, "outputsize", 		outputsize);
	Nan::SetPrototypeMethod(tpl, "empty", 			empty);
	Nan::SetPrototypeMethod(tpl, "record", 			record);
	Nan::SetPrototypeMethod(tpl, "recordinput", 	recordinput);
	Nan::SetPrototypeMethod(tpl, "recordoutput", 	recordoutput);
	Nan::SetPrototypeMethod(tpl, "clear", 			clear);
	Nan::SetPrototypeMethod(tpl, "clearinput", 		clearinput);
	Nan::SetPrototypeMethod(tpl, "clearoutput", 	clearoutput);
	Nan::SetPrototypeMethod(tpl, "tojsobject", 		tojsobject);
	Nan::SetPrototypeMethod(tpl, "fromjsobject", 	fromjsobject);

	// solution found here :
	// http://stackoverflow.com/questions/33383493/how-do-i-update-my-function-to-use-the-new-v8-functiontemplates
	Nan::SetAccessor(tpl->InstanceTemplate(),
					 Nan::New("dimension").ToLocalChecked(),
					 phrase::getdimension, phrase::setdimension);

	Nan::SetAccessor(tpl->InstanceTemplate(),
					 Nan::New("dimension_input").ToLocalChecked(),
					 phrase::getdimensioninput, phrase::setdimensioninput);

	Nan::SetAccessor(tpl->InstanceTemplate(),
					 Nan::New("label").ToLocalChecked(),
					 phrase::getlabel, phrase::setlabel);

	Nan::SetAccessor(tpl->InstanceTemplate(),
					 Nan::New("column_names").ToLocalChecked(),
					 phrase::getcolumnnames, phrase::setcolumnnames);

	constructor.Reset(tpl->GetFunction());
	exports->Set(Nan::New("phrase").ToLocalChecked(), tpl->GetFunction());
}

//*
void phrase::New(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	if(args.IsConstructCall()) {
		// ==================== invoked as constructor : new gmm(...)
		bool bimodal = (args[0]->IsUndefined() || !args[0]->IsBoolean())
						? false : args[0]->BooleanValue();

		// our internal class' instantiation :
		phrase *obj = new phrase(bimodal);
		
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());		
	} else {
		// ==================== invoked as plain function : gmm(...) 
		//						turn it into constructor call
		const int argc = 1;
		v8::Local<v8::Value> argv[argc] = { args[0] };
		v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
		args.GetReturnValue().Set(cons->NewInstance(argc, argv));
	}
}
//*/

/*
void phrase::New(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	// ==================== invoked as constructor : new gmm(...)
	bool bimodal = (args[0]->IsUndefined() || !args[0]->IsBoolean())
					? false : args[0]->BooleanValue();

	// our internal class' instantiation :
	phrase *obj = new phrase(bimodal);
	obj->Wrap(args.This());

	args.GetReturnValue().Set(args.This());		
}
//*/

//*
v8::Local<v8::Object> phrase::NewInstance(v8::Local<v8::Value> arg) {
	Nan::EscapableHandleScope scope;

	const unsigned int argc = 1;
	v8::Local<v8::Value> argv[argc] = { arg };
	v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
	v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

	return scope.Escape(instance);
}
//*/

void phrase::bimodal(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	args.GetReturnValue().Set(Nan::New(obj->phrase_->bimodal()));
}

void phrase::size(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->phrase_->size())));
}

void phrase::inputsize(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->phrase_->inputSize())));
}

void phrase::outputsize(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->phrase_->outputSize())));
}

void phrase::empty(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	args.GetReturnValue().Set(Nan::New(obj->phrase_->empty()));
}

void phrase::record(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	std::vector<float> result;
	//v8::Handle<v8::Value> val;
	v8::Local<v8::Value> val;
	v8::Local<v8::Array> arr = Nan::New<v8::Array>();

	if(args[0]->IsArray()) {
		//v8::Handle<v8::Array> jsArray = v8::Handle<v8::Array>::Cast(args[0]);
		v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(args[0]);
		for(unsigned int i=0; i<jsArray->Length(); i++) {
			val = jsArray->Get(i);
			result.push_back(val->NumberValue());
			Nan::Set(arr, i, val);
		}
	}
	obj->phrase_->record(result);
	// args.GetReturnValue().Set(arr);
	// args.GetReturnValue().Set(Nan::New(obj->phrase_->record(result)));
}

void phrase::recordinput(const Nan::FunctionCallbackInfo<v8::Value> & args) {

}

void phrase::recordoutput(const Nan::FunctionCallbackInfo<v8::Value> & args) {

}

void phrase::clear(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	obj->phrase_->clear();
	//args.GetReturnValue().Set(Nan::New(obj->phrase_->clear()));
}

void phrase::clearinput(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	obj->phrase_->clearInput();
	//args.GetReturnValue().Set(Nan::New(obj->phrase_->clearInput()));
}

void phrase::clearoutput(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.Holder());
	obj->phrase_->clearOutput();
	//args.GetReturnValue().Set(Nan::New(obj->phrase_->clearOutput()));
}

void phrase::tojsobject(const Nan::FunctionCallbackInfo<v8::Value> & args) {

}

void phrase::fromjsobject(const Nan::FunctionCallbackInfo<v8::Value> & args) {

}

//==================== PROPERTY GETTERS / SETTERS ====================//

// example code for getters / setters :
// https://github.com/kneth/DemoNodeExtension/blob/master/person_wrap.cpp

void phrase::getdimension(v8::Local<v8::String> prop,
						  const Nan::PropertyCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.This());
	args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->phrase_->dimension.get())));
}

void phrase::setdimension(v8::Local<v8::String> prop,
						  v8::Local<v8::Value> value,
						  const Nan::PropertyCallbackInfo<void> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.This());
	unsigned int dim = value->IsNumber() ? value->NumberValue() : 1;
	obj->phrase_->dimension.set(dim);
	// args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->phrase_->dimension.set(dim))));
}

void phrase::getdimensioninput(v8::Local<v8::String> prop,
						  const Nan::PropertyCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.This());
	args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->phrase_->dimension_input.get())));
}

void phrase::setdimensioninput(v8::Local<v8::String> prop,
						  v8::Local<v8::Value> value,
						  const Nan::PropertyCallbackInfo<void> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.This());
	unsigned int dim = value->IsNumber() ? value->NumberValue() : 1;
	obj->phrase_->dimension_input.set(dim);
	// args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->phrase_->dimension.set(dim))));
}

void phrase::getlabel(v8::Local<v8::String> prop,
						  const Nan::PropertyCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.This());
	args.GetReturnValue().Set(Nan::New<v8::String>(obj->phrase_->label.get()).ToLocalChecked());
}

void phrase::setlabel(v8::Local<v8::String> prop,
						  v8::Local<v8::Value> value,
						  const Nan::PropertyCallbackInfo<void> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.This());
	std::string label;
	if(value->IsString()) {
		v8::String::Utf8Value val(value->ToString());
		label = std::string(*val);
	} else {	
		label = "";
	}
	obj->phrase_->label.set(label);
	// args.GetReturnValue().Set(Nan::New<v8::String>(obj->phrase_->label.get()).ToLocalChecked());
}

void phrase::getcolumnnames(v8::Local<v8::String> prop,
						  const Nan::PropertyCallbackInfo<v8::Value> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.This());
	v8::Local<v8::Array> arr = Nan::New<v8::Array>();
	for(int i=0; i<obj->phrase_->dimension.get(); i++) {
		arr->Set(i, Nan::New<v8::String>(obj->phrase_->column_names[i].c_str()).ToLocalChecked());
	}
	args.GetReturnValue().Set(arr);
	//args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->phrase_->dimension.get())));
}

void phrase::setcolumnnames(v8::Local<v8::String> prop,
						  v8::Local<v8::Value> value,
						  const Nan::PropertyCallbackInfo<void> & args) {
	phrase *obj = ObjectWrap::Unwrap<phrase>(args.This());

	obj->phrase_->column_names.clear();
	v8::Local<v8::Value> val;
	//v8::Local<v8::Array> arr = Nan::New<v8::Array>();

	if(value->IsArray()) {
		v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(value);
		for(unsigned int i=0; i<jsArray->Length(); i++) {
			val = jsArray->Get(i);
			v8::String::Utf8Value sval(val->ToString());
			obj->phrase_->column_names.push_back(std::string(*sval));
			//Nan::Set(arr, i, val);
		}
	}
}
