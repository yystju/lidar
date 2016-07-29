#ifndef HELLO_H
#define HELLO_H

#include <node.h>
#include <node_object_wrap.h>

namespace M {
	class Hello : public node::ObjectWrap {
	public:
		static void Init(v8::Local<v8::Object> exports);

	private:
		explicit Hello(double d = 0);
		~Hello();

		static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
		static v8::Persistent<v8::Function> constructor;
		double _d;
	};
}

#endif
