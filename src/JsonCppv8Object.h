#include <nan.h>
#include "../xmm/src/xmm.h"


//v8::Local<v8::Object> valueToObject(Json::Value & val);
v8::Local<v8::Array> valueToArray(Json::Value & val);

// v8::Local<v8::Array> valueToArray(Json::Value & val) {

// 	v8::Local<v8::Array> res = Nan::New<v8::Array>(val.size());

// 	for(int i=0; i<val.size(); ++i) {
// 		if(val[i].isBool()) {
// 			res->Set(i, Nan::New(val[i].asBool()));
// 		} else if(val[i].isInt()) {
// 			res->Set(i, Nan::New(val[i].asInt()));
// 		} else if(val[i].isDouble()) {
// 			res->Set(i, Nan::New(val[i].asDouble()));
// 		} else if(val[i].isString()) {
// 			res->Set(i, Nan::New<v8::String>(
// 							val[i].asString()
// 						).ToLocalChecked()
// 			);
// 		} else if(val[i].isArray()) {
// 			res->Set(i, valueToArray(val[i]));
// 		} else if(val[i].isInt()) {
// 			res->Set(i, valueToObject(val[i]));
// 		}
// 	}
// 	return res;
// }

v8::Local<v8::Object> valueToObject(Json::Value & val) {

	Json::Value::Members members = val.getMemberNames();
	v8::Local<v8::Object> res = Nan::New<v8::Object>();

	unsigned int i = 0;
	//*
	for(auto it : members) {

		v8::Local<v8::String> key =
			Nan::New<v8::String>(it).ToLocalChecked();

		if(val[it].isBool()) {
			Nan::Set(res, key, Nan::New(val[it].asBool()));
		} else if(val[it].isInt()) {
			Nan::Set(res, key, Nan::New(val[it].asInt()));
		} else if(val[it].isDouble()) {
			Nan::Set(res, key, Nan::New(val[it].asDouble()));
		} else if(val[it].isString()) {
			Nan::Set(res, key, Nan::New<v8::String>(
								val[it].asString()
							).ToLocalChecked()
			);
		} else if(val[it].isArray()) {
			Nan::Set(res, key, valueToArray(val[it]));
		} else if(val[it].isObject()) {
			Nan::Set(res, key, valueToObject(val[it]));
		}
		i++;	
	}
	return res;
	//*/

	/*
	for(int i=0; i<members.size(); ++i) {
		v8::Local<v8::String> key
			= Nan::New<v8::String>(members[i]).ToLocalChecked();

		if(val[members[i]].isBool()) {
			Nan::Set(res, key, Nan::New(val[members[i]].asBool()));
		} else if(val[members[i]].isInt()) {
			Nan::Set(res, key, Nan::New(val[members[i]].asInt()));
		} else if(val[members[i]].isDouble()) {
			Nan::Set(res, key, Nan::New(val[members[i]].asDouble()));
		} else if(val[members[i]].isString()) {
			Nan::Set(res, key, Nan::New<v8::String>(
									val[members[i]].asString()
								).ToLocalChecked()
			);
		} else if(val[members[i]].isArray()) {
			Nan::Set(res, key, valueToArray(val[members[i]]));
			//Nan::Set(res, key, Nan::New(valueToArray(val[members[i]])));
			//Nan::Set(res, key, Nan::New(val[members[i]].size()));
		} else if(val[members[i]].isObject()) {
			Nan::Set(res, key, valueToObject(val[members[i]]));
		}
	}
	return res;
	//*/
}

v8::Local<v8::Array> valueToArray(Json::Value & val) {

	v8::Local<v8::Array> res = Nan::New<v8::Array>(val.size());

	unsigned int i = 0;
	//*
	for(auto it : val) {
		if(it.isBool()) {
			Nan::Set(res, i, Nan::New(it.asBool()));
		} else if(it.isInt()) {
			Nan::Set(res, i, Nan::New(it.asInt()));
		} else if(it.isDouble()) {
			Nan::Set(res, i, Nan::New(it.asDouble()));
		} else if(it.isString()) {
			Nan::Set(res, i, Nan::New<v8::String>(
								it.asString()
							).ToLocalChecked()
			);
			// Nan::Set(res, i, Nan::New(val.size()));
		} else if(it.isArray()) {
			Nan::Set(res, i, valueToArray(it));
			//Nan::Set(res, i, Nan::New(valueToArray(it)));
			//Nan::Set(res, i, Nan::New(val.size()));
		} else if(it.isObject()) {
			Nan::Set(res, i, valueToObject(it));
			// Nan::Set(res, i, Nan::New(val.size()));
		}
		i++;	
	}
	return res;
	//*/

	/*
	for(int i=0; i<val.size(); ++i) {
		if(val[i].isBool()) {
			Nan::Set(res, i, Nan::New(val.get(i, false).asBool()));
		} else if(val[i].isInt()) {
			Nan::Set(res, i, Nan::New(val.get(i, 0).asInt()));
		} else if(val[i].isDouble()) {
			Nan::Set(res, i, Nan::New(val.get(i, 0.).asDouble()));
		} else if(val[i].isString()) {
			Nan::Set(res, i, Nan::New<v8::String>(
								val.get(i, "").asString()
							).ToLocalChecked()
			);
		} else if(val[i].isArray()) {
			Nan::Set(res, i, valueToArray(val[i]));
		} else if(val[i].isObject()) {
			Nan::Set(res, i, valueToObject(val[i]));
		}
		//Nan::Set(res, i, Nan::New(val.size()));
	}
	return res;
	//*/
}

Json::Value objectToValue(v8::Local<v8::Object> & obj) {
	//TODO
	Json::Value res;
	return res;
}

Json::Value arrayToValue(v8::Local<v8::Array> & arr) {
	//TODO
	Json::Value res;
	return res;
}

