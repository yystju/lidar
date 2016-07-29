#include "Hello.h"

namespace M {
	using v8::Context;
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::FunctionTemplate;
	using v8::Isolate;
	using v8::Local;
	using v8::Number;
	using v8::Object;
	using v8::Persistent;
	using v8::String;
	using v8::Value;

	Persistent<Function> Hello::constructor;

	Hello::Hello(double d) : _d(d) {
	}

	Hello::~Hello() {
	}

	void Hello::Init(Local<Object> exports) {
		Isolate* isolate = exports->GetIsolate();

		// Prepare constructor template
		Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
		tpl->SetClassName(String::NewFromUtf8(isolate, "Hello"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype
		NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

		constructor.Reset(isolate, tpl->GetFunction());
		exports->Set(String::NewFromUtf8(isolate, "Hello"), tpl->GetFunction());
	}

	void Hello::New(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		if (args.IsConstructCall()) {
			double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
			Hello* obj = new Hello(value);
			obj->Wrap(args.This());
			args.GetReturnValue().Set(args.This());
		}
		else {
			const int argc = 1;
			Local<Value> argv[argc] = { args[0] };
			Local<Context> context = isolate->GetCurrentContext();
			Local<Function> cons = Local<Function>::New(isolate, constructor);
			Local<Object> result =
				cons->NewInstance(context, argc, argv).ToLocalChecked();
			args.GetReturnValue().Set(result);
		}
	}

	void Hello::PlusOne(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		Hello* obj = ObjectWrap::Unwrap<Hello>(args.Holder());
		obj->_d += 1;
		args.GetReturnValue().Set(Number::New(isolate, obj->_d));
	}
}
