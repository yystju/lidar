#include "raspi.h"

#include <wiringPi.h>

#ifdef __cplusplus
extern "C" {
#endif
    
int raspi_blink(int n, RaspiBlinkCallback callback, void * param) {
  int rc = 0;
  int i;
  
  wiringPiSetup();
  
  pinMode(GPIO_PIN, OUTPUT);
  
  if(n > 0) {
    for (i = 0; i < n; ++i) {
      digitalWrite(GPIO_PIN, HIGH);
      delay(250);
      
      if(callback) {
        rc = callback(param);
      }
      
      digitalWrite(GPIO_PIN, LOW);
      delay(750);
    }
  } else {
    while(1) {
      digitalWrite(GPIO_PIN, HIGH);
      delay(250);
      
      if(callback) {
        rc = callback(param);
      }
      
      digitalWrite(GPIO_PIN, LOW);
      delay(750);
    }
  }

  return rc;
}
    
#ifdef __cplusplus
}
#endif
