#include <node.h>

namespace n {
	using namespace v8;
	
	void TestCall(const FunctionCallbackInfo<Value>& args) {
		Isolate * isolate = args.GetIsolate();
		
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "This is test."));
	}
	
	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "test", TestCall);
	}
	
	NODE_MODULE(test, init);
}
