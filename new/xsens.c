#include "xsens.h"
#include "xsens_api_wrapper.h"
#include "helper.h"

#include <stdlib.h>
#include <string.h>

void readXsensData(XsensDataProcessor processor) {
    debug("[readXsensData]>>\n");
    
    xsens_wrapper_test();
    
    XsensData * pData = (XsensData *)malloc(sizeof(XsensData));
    
    memset(pData, 0, sizeof(XsensData));
    
    if(processor) {
        processor(pData);
    }
    
    free(pData);
    
    debug("[readXsensData]<<\n");
}
