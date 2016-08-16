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
    
    return 0;
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
        if (packet.containsOrientation()) {
    		XsQuaternion quaternion = packet.orientationQuaternion();
    		
    		pData->quaternion_w = (double)quaternion.m_w;
    		pData->quaternion_x = (double)quaternion.m_x;
    		pData->quaternion_y = (double)quaternion.m_y;
    		pData->quaternion_z = (double)quaternion.m_z;
    		
    		debug("[xsens_wrapper_read] quaternion(w,x,y,z) : (%f, %f, %f, %f) \n", pData->quaternion_w, pData->quaternion_x, pData->quaternion_y, pData->quaternion_z);
    
            //Euler
    		XsEuler euler = packet.orientationEuler();
    		
    		pData->euler_roll = (double)euler.m_roll;
    		pData->euler_pitch = (double)euler.m_pitch;
    		pData->euler_yaw = (double)euler.m_yaw;
    		
    		debug("[xsens_wrapper_read] euler(roll, pitch, yaw) : (%f, %f, %f) \n", pData->euler_roll, pData->euler_pitch, pData->euler_yaw);
        }
		
		//Time
		debug("## contains UTC : %s\n", packet.containsUtcTime() ? "true" : "false");
		if(packet.containsUtcTime()) {
    		XsUtcTime now = packet.utcTime();
    		//XsUtcTime now = XsUtcTime::currentTime(); //For test only...
    		
    		pData->year = int(now.m_year - 1900);
    		
    		pData->month = int(now.m_month);
    		pData->day = int(now.m_day);
    		pData->hour = int(now.m_hour);
    		pData->minute = int(now.m_minute);
    		pData->second = int(now.m_second);
    		pData->nano = int(now.m_nano);
    		
    		debug("[xsens_wrapper_read] utctime(Y-M-D h:m:s.n) : (%d-%d-%d %d:%d:%d.%d) \n", pData->year, pData->month, pData->day, pData->hour, pData->minute, pData->second, pData->nano);
		}
		
		if (packet.containsVelocity()) {
			XsVector v = packet.velocity();

			if (v.size() == 3) {
				const XsReal * p = v.data();

				pData->x = p[0];
				pData->y = p[1];
				pData->z = p[2];
				
				debug("[xsens_wrapper_read] velocity(x, y, z) : (%f, %f, %f) \n", pData->x, pData->y, pData->z);
			}
		}
        
        if (packet.containsAltitude()) {
			double altitude = packet.altitude();
			debug("[xsens_wrapper_read] altitude : %f \n", altitude);
		}
		
		if (packet.containsLatitudeLongitude()) {
			XsVector v = packet.latitudeLongitude();

			if (v.size() == 2) {
				const XsReal * p = v.data();

				pData->latitude = p[0];
				pData->longitude = p[1];
				
				debug("[xsens_wrapper_read] (latitude, longitude) : (%df, %f) \n", pData->latitude, pData->longitude);
			}
		}
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
