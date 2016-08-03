#include <iostream>
#include <node.h>
#include "Hello.h"

namespace M {
	using namespace v8;
	using node::AtExit;
	
	void TestCall(const FunctionCallbackInfo<Value>& args) {
		Isolate * isolate = args.GetIsolate();
		
		if (args.Length() < 2) {
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}
		
		if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}
		
		double value = args[0]->NumberValue() + args[1]->NumberValue();
		Local<Number> num = Number::New(isolate, value);
		
		args.GetReturnValue().Set(num);
		
		//args.GetReturnValue().Set(String::NewFromUtf8(isolate, "This is test."));
	}
	
	void RunCallback(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		Local<Function> cb = Local<Function>::Cast(args[0]);
		const unsigned argc = 2;
		Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world"), Number::New(isolate, 20.5) };
		cb->Call(Null(isolate), argc, argv);
	}
	
	void CreateObject(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		
		Local<Object> obj = Object::New(isolate);
		obj->Set(String::NewFromUtf8(isolate, "msg"), args[0]->ToString());
		
		args.GetReturnValue().Set(obj);
	}
	
	static void at_exit_cb(void* arg) {
		Isolate* isolate = static_cast<Isolate*>(arg);

		std::cout << "[at_exit_cb]" << std::endl;
	}

	void init(Local<Object> exports, Local<Object> module) {
		//NODE_SET_METHOD(module, "exports", RunCallback);
		
		NODE_SET_METHOD(exports, "call", RunCallback);
		NODE_SET_METHOD(exports, "test", TestCall);
		
		NODE_SET_METHOD(exports, "newInstance", CreateObject);

		Hello::Init(exports);

		AtExit(at_exit_cb, exports->GetIsolate());
	}
	
	NODE_MODULE(module1, init);
}
