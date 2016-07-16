#ifndef _XSENS_API_WRAPPER_H_
#define _XSENS_API_WRAPPER_H_

#include "xsens.h"

#ifdef __cplusplus
extern "C" {
#endif

int xsens_wrapper_test(void);

typedef void* XSENS;

XSENS xsens_wrapper_init(const char *fileName, const int baud);

int xsens_wrapper_read(XSENS xsens, XsensData * pData);

void xsens_wrapper_dispose(XSENS xsens);

#ifdef __cplusplus
}
#endif

#endif