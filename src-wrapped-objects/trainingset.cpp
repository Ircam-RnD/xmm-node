#include "phrase.h"
#include "trainingset.h"

// v8::Persistent<v8::Function> trainingset::constructor;
Nan::Persistent<v8::Function> trainingset::constructor;

trainingset::trainingset(bool bimodal) :
trainingset_(xmm::MemoryMode::OwnMemory,
	   		(bimodal ? xmm::Multimodality::Bimodal
					 : xmm::Multimodality::Unimodal)) {

}

trainingset::~trainingset() {

}

void trainingset::Init(v8::Local<v8::Object> exports) {
	Nan::HandleScope scope;

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("trainingset").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	Nan::SetPrototypeMethod(tpl, "bimodal",				bimodal);
	Nan::SetPrototypeMethod(tpl, "size",	 			size);
	Nan::SetPrototypeMethod(tpl, "empty", 				empty);
	Nan::SetPrototypeMethod(tpl, "addphrase",			addphrase);
	Nan::SetPrototypeMethod(tpl, "removephrase", 		removephrase);
	Nan::SetPrototypeMethod(tpl, "removephrasesofclass",	removephrasesofclass);
	Nan::SetPrototypeMethod(tpl, "getphrase", 			getphrase);
	Nan::SetPrototypeMethod(tpl, "getphrasesofclass", 	getphrasesofclass);
	Nan::SetPrototypeMethod(tpl, "clear", 				clear);
	Nan::SetPrototypeMethod(tpl, "labels", 				labels);
	Nan::SetPrototypeMethod(tpl, "tojsobject", 			tojsobject);
	Nan::SetPrototypeMethod(tpl, "fromjsobject", 		fromjsobject);

	// solution found here :
	// http://stackoverflow.com/questions/33383493/how-do-i-update-my-function-to-use-the-new-v8-functiontemplates
	Nan::SetAccessor(tpl->InstanceTemplate(),
					 Nan::New("dimension").ToLocalChecked(),
					 trainingset::getdimension, trainingset::setdimension);

	Nan::SetAccessor(tpl->InstanceTemplate(),
					 Nan::New("dimension_input").ToLocalChecked(),
					 trainingset::getdimensioninput, trainingset::setdimensioninput);

	Nan::SetAccessor(tpl->InstanceTemplate(),
					 Nan::New("column_names").ToLocalChecked(),
					 trainingset::getcolumnnames, trainingset::setcolumnnames);

	constructor.Reset(tpl->GetFunction());
	exports->Set(Nan::New("trainingset").ToLocalChecked(), tpl->GetFunction());
}

void trainingset::New(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	if(args.IsConstructCall()) {
		// ==================== invoked as constructor : new gmm(...)
		bool bimodal = (args[0]->IsUndefined() || !args[0]->IsBoolean())
						? false : args[0]->BooleanValue();

		// our internal class' instantiation :
		trainingset *obj = new trainingset(bimodal);
		
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


void trainingset::bimodal(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	args.GetReturnValue().Set(Nan::New(obj->trainingset_.bimodal()));
}

void trainingset::size(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->trainingset_.size())));
}

void trainingset::empty(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	args.GetReturnValue().Set(Nan::New(obj->trainingset_.empty()));
}

void trainingset::addphrase(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	// Nan::HandleScope scope;
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	// choose here which version of addPhrase() should be called

	// possible signatures are :
	// addPhrase(int index, std::string label = "");
	// addPhrase(int index, Phrase const& phrase); // MOST IMPORTANT ONE !!!!! (implemented)
	// addPhrase(int index, std::shared_ptr<Phrase> phrase); // -> this one might not be used
	// phrase *p;
	if(args[0]->IsNumber()) {
		//try {
		//	if(args[1]->ToObject()->InternalFieldCount() > 0) {
				phrase *p = Nan::ObjectWrap::Unwrap<phrase>(args[1]->ToObject());

				// xmm::Phrase xp;
				// xp = *p->GetVal();
				// obj->trainingset_.addPhrase(args[0]->NumberValue(), xp);

				obj->trainingset_.addPhrase(args[0]->NumberValue(), *p->GetVal());
		//	}
		//} catch(const std::exception& e) {
			//throw;
		//}
	}
}

void trainingset::removephrase(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	// Nan::HandleScope scope;
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	// TODO :
	//*
	if(args[0]->IsNumber()) {
		obj->trainingset_.removePhrase(args[0]->NumberValue());
	}
	//*/
}

void trainingset::removephrasesofclass(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	// TODO :
}

void trainingset::getphrase(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	// TODO :
	/*
	//phrase *p;
	if(args[0]->IsNumber()) {
		v8::Local<v8::Value> v;
		v8::Local<v8::Object> o = phrase::NewInstance(v);
		phrase *p = ObjectWrap::Unwrap<phrase>(o);
		//p = Nan::New()
		//xmm::Phrase *xp = obj->trainingset_.getPhrase(args[0]->NumberValue()).get();
		//p->SetVal(*xp);
		p->SetVal(*obj->trainingset_.getPhrase(args[0]->NumberValue()).get());
		
		//args.GetReturnValue().Set(Nan::New<v8::Local<v8::Object>>(o));
		args.GetReturnValue().Set(o);
	}
	//*/
}

void trainingset::getphrasesofclass(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	// TODO :
}

void trainingset::clear(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	obj->trainingset_.clear();
	//args.GetReturnValue().Set(Nan::New(obj->trainingset_.clear()));
}

void trainingset::labels(const Nan::FunctionCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.Holder());
	// TODO :
	//args.GetReturnValue().Set(Nan::New(obj->trainingset_.clearInput()));
}

void trainingset::tojsobject(const Nan::FunctionCallbackInfo<v8::Value> & args) {

}

void trainingset::fromjsobject(const Nan::FunctionCallbackInfo<v8::Value> & args) {

}

//==================== PROPERTY GETTERS / SETTERS ====================//

// example code for getters / setters :
// https://github.com/kneth/DemoNodeExtension/blob/master/person_wrap.cpp

void trainingset::getdimension(v8::Local<v8::String> prop,
						  const Nan::PropertyCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.This());
	args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->trainingset_.dimension.get())));
}

void trainingset::setdimension(v8::Local<v8::String> prop,
						  v8::Local<v8::Value> value,
						  const Nan::PropertyCallbackInfo<void> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.This());
	unsigned int dim = value->IsNumber() ? value->NumberValue() : 1;
	obj->trainingset_.dimension.set(dim);
	// args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->trainingset_.dimension.set(dim))));
}

void trainingset::getdimensioninput(v8::Local<v8::String> prop,
						  const Nan::PropertyCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.This());
	args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->trainingset_.dimension_input.get())));
}

void trainingset::setdimensioninput(v8::Local<v8::String> prop,
						  v8::Local<v8::Value> value,
						  const Nan::PropertyCallbackInfo<void> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.This());
	unsigned int dim = value->IsNumber() ? value->NumberValue() : 1;
	obj->trainingset_.dimension_input.set(dim);
	// args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->trainingset_.dimension.set(dim))));
}

void trainingset::getcolumnnames(v8::Local<v8::String> prop,
						  const Nan::PropertyCallbackInfo<v8::Value> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.This());
	v8::Local<v8::Array> arr = Nan::New<v8::Array>();
	std::vector<std::string> colnames = obj->trainingset_.column_names.get();
	for(int i=0; i<obj->trainingset_.dimension.get(); i++) {
		arr->Set(i, Nan::New<v8::String>(colnames[i].c_str()).ToLocalChecked());
	}
	args.GetReturnValue().Set(arr);
	//args.GetReturnValue().Set(Nan::New(static_cast<unsigned int>(obj->trainingset_.dimension.get())));
}

void trainingset::setcolumnnames(v8::Local<v8::String> prop,
						  v8::Local<v8::Value> value,
						  const Nan::PropertyCallbackInfo<void> & args) {
	trainingset *obj = ObjectWrap::Unwrap<trainingset>(args.This());

	std::vector<std::string> colnames;
	//obj->trainingset_.column_names.clear();
	v8::Local<v8::Value> val;
	//v8::Local<v8::Array> arr = Nan::New<v8::Array>();

	if(value->IsArray()) {
		v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(value);
		for(unsigned int i=0; i<jsArray->Length(); i++) {
			val = jsArray->Get(i);
			v8::String::Utf8Value sval(val->ToString());
			colnames.push_back(std::string(*sval));
			obj->trainingset_.column_names.set(colnames);
			//Nan::Set(arr, i, val);
		}
	}
}
