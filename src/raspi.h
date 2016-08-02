#ifndef _RASPI_H_
#define _RASPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define GPIO_PIN 1

typedef int (* RaspiBlinkCallback)(void * param);

void raspi_blink(int n, RaspiBlinkCallback callback, void * param);
    
#ifdef __cplusplus
}
#endif

#endif