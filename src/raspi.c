#include "raspi.h"

#include <wiringPi.h>

#ifdef __cplusplus
extern "C" {
#endif
    
int raspi_blink(int n, RaspiBlinkCallback callback, void * param) {
  int rc = 0;
  int i = 0;
  
  wiringPiSetup();
  
  pinMode(GPIO_PIN, OUTPUT);
  
  while(1) {
    ++i;
    
    if(n >= 0 && i > n) {
      break;
    }
    
    digitalWrite(GPIO_PIN, HIGH);
    delay(250);
    
    if(callback) {
      rc = callback(param);
    }
    
    digitalWrite(GPIO_PIN, LOW);
    delay(750);
  }

  return rc;
}
    
#ifdef __cplusplus
}
#endif
