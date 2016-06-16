#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>


#include "receive.h"
#include "serial.h"
#include "helper.h"

using namespace std;

namespace Hello {
    template <class T>
    T& test(T& a, T* b) {
        return a != 10 ? a : *b;
    }
    
    template <class T, int N>
    size_t test(T t) {
        return N * sizeof(T);
    }
    
    struct S {
        int i;
        double x;
        char * p;
    };
    
    union V {
        unsigned int l;
        
        struct {
            unsigned char b0;
            unsigned char b1;
            unsigned char b2;
            unsigned char b3;
            // unsigned char b4;
            // unsigned char b5;
            // unsigned char b6;
            // unsigned char b7;
        } a;
        
        char text[32];
    };
    
    class A {
    public:
        A(void): x(0), y(0) {}
        A(int x, int y): x(x), y(y) {}
        A(const A& a) : x(a.x), y(a.y) {cout << "**COPY**" << endl;}
        A(const A&& a) : x(a.x), y(a.y) {cout << "**MOV**" << endl; this->x = 0; this->y = 0;}
        ~A() {}
        operator int() { return x + y; }
        A& operator *(A&& a) {
            cout << "*MOV OPERATOR*" << endl;
            this->x = a.x; a.x = 0;
            this->y = a.y; a.y = 0;
            return *this;
        }
        A& operator *(int z) {
            this->x = this->x * z;
            this->y = this->y * z;
            return *this;
        }
    private:
        int x;
        int y;
    };
}

int main(int argc, char * argv[]) {
    const char * file_name = "/dev/ttyAMA0";
    if(file_exits(file_name)) {
        receive_serial_data(file_name);
    }
    
    // double n = 0.005d;
    
    // cout << Hello::test<double, 5>(n) << endl;
    // cout << Hello::test<double, 10>(n) << endl;
    
    // int * p = new (nothrow) int[1024];
    
    // if(p != nullptr) {
    //     cout << "yes." << endl;
    // }
    
    // delete[] p;
    
    // Hello::S * pS = nullptr;
    
    // char *q = new (nothrow) char[sizeof(Hello::S)];
    
    // pS = (Hello::S *) q;
    
    // pS->i = 100;
    // pS->x = 100.0d;
    // pS->p = "Hello";
    
    // cout << pS ->p << endl;
    
    // delete[] q;
    
    // cout << "sizeof(Hello::V) : " << sizeof(Hello::V) << endl;
    
    // Hello::V v;
    
    // v.a.b0 = 0x00;
    // v.a.b1 = 0x00;
    // v.a.b2 = 0x00;
    // v.a.b3 = 0xFF;
    // // v.a.b4 = 0x00;
    // // v.a.b5 = 0x00;
    // // v.a.b6 = 0x00;
    // // v.a.b7 = 0x01;
    
    // cout << "V : " << v.l << endl;
    
    // cout << " >> " << typeid(v).name() << endl;
    
    // if(typeid(v) == typeid(Hello::V)) {
    //     cout << " == " << endl;
    // }
    
    // Hello::A a(5, 10);
    
    // int z = (int)a;
    
    // cout << "z : " << z << endl;
    
    // Hello::A b = Hello::A();
    
    // cout << "b : " << b << endl;
    
    // receive_sync_data(argc, argv);
    
    // ofstream o("./test.bin", ios::binary);
    
    // double data[11];
    
    // for(int i = 0; i < 11; ++i) {
    //     data[i] = (double)i;
    // }
    
    // const char * p = (char *)data;
    
    // o.write(p, sizeof(data));
    
    // o.close();
    
    // cout << "sizeof(p) : " << sizeof(p) << endl;
    
    // ifstream i("./test.bin", ios::binary);
    
    // double x;
    
    // while(!i.eof()) {
    //     i.read((char *)&x, sizeof(double));
        
    //     cout << x << endl;
    // }
    
    // cout << u8"你好" << endl;
    
    
    
    
    // string str;
    
    // while(!cin.eof()) {
    //     cin >> str;
        
    //     cout << str << endl;
    // }
    
    // i.close();
    
    
    
    
    // ifstream in("./test.bin");
    // unsigned char c;
    
    // if(in) {
    //     while(!in.eof()) {
    //         in >> c;
            
    //         cout << (unsigned int)c << endl;
    //     }
        
    //     in.close();
    // }
    
    //receive_test(argc, argv);
    
    //string str;
    
    // ifstream f("./Makefile");
    // ofstream o("./test.bin");
    
    // while(getline(f, str)) {
    //     cout << str << endl;
    //     unsigned char checksum = 0x00;
        
    //     for(char c : str) {
    //         checksum ^= ((unsigned char)c);
    //     }
        
    //     o << checksum;
    // }
    
    // f.close();
    // o.close();
    
    // ifstream in("./test.bin");
    // unsigned char c;
    
    // if(in) {
    //     while(!in.eof()) {
    //         in >> c;
            
    //         cout << (unsigned int)c << endl;
    //     }
        
    //     in.close();
    // }
    
    // stringstream ss;
    
    // ss << "Hello";
    // ss << "ABCD";
    // ss << "EFGH";
    
    // str = ss.str();
    
    // cout << str << endl;
	
    return 0;
}