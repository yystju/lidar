#include "xsens_api_wrapper.h"

#include <xsens/xsportinfoarray.h>
#include <xsens/xsdatapacket.h>
#include <xsens/xstime.h>
#include <xcommunication/legacydatapacket.h>
#include <xcommunication/int_xsdatapacket.h>
#include <xcommunication/enumerateusbdevices.h>

#include "deviceclass.h"
#include "helper.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

/*
#ifdef __GNUC__
#include "conio.h" // for non ANSI _kbhit() and _getch()
#else
#include <conio.h>
#endif
*/

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

struct Xsens {
    Xsens(const char * fileName, const int baud) : device(), portInfo(fileName, XsBaud::numericToRate(baud)) {}
    ~Xsens() {}
    DeviceClass device;
    XsPortInfo portInfo;
};

XSENS xsens_wrapper_init(const char *fileName, const int baud) {
    debug("[xsens_wrapper_init]>>\n");
    
    Xsens * p = new Xsens(fileName, baud);
    
    if (!(p->device).openPort(p->portInfo)) {
        delete p;
        p = nullptr;
    }
    
    if(p) {
        (p->device).gotoConfig();
        
        (p->portInfo).setDeviceId((p->device).getDeviceId());
        
        debug("Device: %s opened.", (p->device).getProductCode().toStdString().c_str());
        
        (p->device).gotoMeasurement();
    }
    
    debug("[xsens_wrapper_init]<<\n");
    
    return p;
}

int xsens_wrapper_read(XSENS xsens, XsensData * pData) {
    Xsens * p = (Xsens *) xsens;
    
    XsByteArray data;
	XsMessageArray msgs;
	
	(p->device).readDataToBuffer(data);
	(p->device).processBufferedData(data, msgs);
	
	for (XsMessageArray::iterator it = msgs.begin(); it != msgs.end(); ++it) {
		XsDataPacket packet;
		if ((*it).getMessageId() == XMID_MtData) {
			LegacyDataPacket lpacket(1, false);
			lpacket.setMessage((*it));
			lpacket.setXbusSystem(false);
			lpacket.setDeviceId((p->portInfo).deviceId(), 0);
			lpacket.setDataFormat(XOM_Orientation, XOS_OrientationMode_Quaternion,0);	//lint !e534
			XsDataPacket_assignFromLegacyDataPacket(&packet, &lpacket, 0);
		}
		else if ((*it).getMessageId() == XMID_MtData2) {
			packet.setMessage((*it));
			packet.setDeviceId((p->portInfo).deviceId());
		}

        //Quaternion
		XsQuaternion quaternion = packet.orientationQuaternion();
		
		pData->quaternion_w = (double)quaternion.m_w;
		pData->quaternion_x = (double)quaternion.m_x;
		pData->quaternion_y = (double)quaternion.m_y;
		pData->quaternion_z = (double)quaternion.m_z;
		
// 		std::cout << "\r"
// 				  << "W:" << std::setw(5) << std::fixed << std::setprecision(2) << quaternion.m_w
// 				  << ",X:" << std::setw(5) << std::fixed << std::setprecision(2) << quaternion.m_x
// 				  << ",Y:" << std::setw(5) << std::fixed << std::setprecision(2) << quaternion.m_y
// 				  << ",Z:" << std::setw(5) << std::fixed << std::setprecision(2) << quaternion.m_z
// 		;

        //Euler
		XsEuler euler = packet.orientationEuler();
		
		pData->euler_roll = (double)euler.m_roll;
		pData->euler_pitch = (double)euler.m_pitch;
		pData->euler_yaw = (double)euler.m_yaw;
		
// 		std::cout << ",Roll:" << std::setw(7) << std::fixed << std::setprecision(2) << euler.m_roll
// 				  << ",Pitch:" << std::setw(7) << std::fixed << std::setprecision(2) << euler.m_pitch
// 				  << ",Yaw:" << std::setw(7) << std::fixed << std::setprecision(2) << euler.m_yaw
// 		;

//        std::cout << std::endl << std::flush;
		
		
		//Time
		XsUtcTime now = packet.utcTime();
		//XsUtcTime now = XsUtcTime::currentTime(); //For test only...
		
// 		std::cout << int(now.m_year - 1900) << "-" << int(now.m_month) << "-" << int(now.m_day) << " " << int(now.m_hour) << ":" << int(now.m_minute) << ":" << int(now.m_second) << std::endl;
		
		pData->year = int(now.m_year - 1900);
		
		pData->month = int(now.m_month);
		pData->day = int(now.m_day);
		pData->hour = int(now.m_hour);
		pData->minute = int(now.m_minute);
		pData->second = int(now.m_second);
		
		//Velocity
		XsVector velocity = packet.velocity();

// 		std::cout << "V[";

// 		for (XsSize i = 0; i < velocity.size(); ++i) {
// 			XsReal value = velocity[i];
// 			if (i != 0) {
// 				std::cout << ",";
// 			}

// 			std::cout << value;
// 		}
		
// 		std::cout << "]" << std::endl;
	}
	msgs.clear();
	XsTime::msleep(0);
    return 1;
}

void xsens_wrapper_dispose(XSENS xsens) {
    
    delete ((Xsens *)xsens);
}

#ifdef __cplusplus
}
#endif
