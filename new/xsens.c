#include "xsens.h"
#include "xsens_api_wrapper.h"
#include "helper.h"

#include <stdlib.h>
#include <string.h>

void readXsensData(XsensDataProcessor processor) {
    debug("[readXsensData]>>\n");
    
    XSENS xsens = xsens_wrapper_init("/dev/ttyAMA0", 115200);
    
    XsensData * pData = (XsensData *)malloc(sizeof(XsensData));
    
    memset(pData, 0, sizeof(XsensData));
    
    while(xsens_wrapper_read(xsens, pData)) {
        if(processor) {
            processor(pData);
        }
    }
    
    free(pData);
    
    xsens_wrapper_dispose(xsens);
    
    debug("[readXsensData]<<\n");
}
