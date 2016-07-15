#include "xsens.h"
#include "xsens_api_wrapper.h"
#include "helper.h"

#include <stdlib.h>
#include <string.h>

void readXsensData(const char * device_name, XsensDataProcessor processor, void * param) {
    debug("[readXsensData]>>\n");
    
    if(file_exits(device_name)) {
        XSENS xsens = xsens_wrapper_init(device_name, 115200);
        
        XsensData * pData = (XsensData *)malloc(sizeof(XsensData));
        
        memset(pData, 0, sizeof(XsensData));
        
        while(xsens_wrapper_read(xsens, pData)) {
            if(processor) {
                if(!processor(pData, param)) {
                    break;
                }
            }
        }
        
        free(pData);
        
        xsens_wrapper_dispose(xsens);
    } else {
        error("device %s doesn't exist.", device_name);
    }
    
    debug("[readXsensData]<<\n");
}
