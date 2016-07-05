#include "xsens_api_wrapper.h"
#include "helper.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

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
    ~A() {debug("YES!!!!\n");}
    
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

int test1(int x) {
    unique_ptr<A> a(new A("GO"));
    
    vector<int> src(10), dest(10);
    
    for(int i = 0; i < 10; ++i) {
        src[i] = i;
    }
    
    auto f = [x] (int & n) -> bool {return n != 0 && n % x == 0;};
    
    copy_if(src.begin(), src.end(), dest.begin(), f);
    
    cout << "dest : ";
    
    for(auto i : dest) {
        if( i == 0 ) {
            break;
        }
        cout << i << ", ";
    }
    
    cout << endl;
    
    debug("[xsens_wrapper_test] %s\n", (*a).c_str());
}

int xsens_wrapper_test(void) {
    return test1(3);
}

#ifdef __cplusplus
}
#endif
