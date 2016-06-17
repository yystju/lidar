#include "xsens_api_wrapper.h"
#include "helper.h"

#include <iostream>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

class A {
public:
    A(): _str() {}
    A(const char * s): _str(s) {}
    A(const string& s): _str(s) {}
    A(const A& a): _str(a._str) {}
    ~A() {}
    
    string& str();
    const char * c_str();
private:
    string _str;
};

string& A::str() {
    return this->_str;
}

const char * A::c_str() {
    return this->_str.c_str();
}

int xsens_wrapper_test(void) {
    A * a = new A("GO");
    
    debug("[xsens_wrapper_test] %s\n", static_cast<const char *>(a->c_str()));
    
    delete a;
}

#ifdef __cplusplus
}
#endif
