#include "raspi.h"

#include <wiringPi.h>

#ifdef __cplusplus
extern "C" {
#endif
    
void raspi_blink(int n, RaspiBlinkCallback callback, void * param) {
  wiringPiSetup();
  
  pinMode(GPIO_PIN, OUTPUT);
  
  if(n > 0) {
    for (int i = 0; i < n; ++i) {
      digitalWrite(GPIO_PIN, HIGH);
      delay(250);
      
      if(callback) {
        callback(param);
      }
      
      digitalWrite(GPIO_PIN, LOW);
      delay(750);
    }
  } else {
    while(1) {
      digitalWrite(GPIO_PIN, HIGH);
      delay(250);
      
      if(callback) {
        callback(param);
      }
      
      digitalWrite(GPIO_PIN, LOW);
      delay(750);
    }
  }
}
    
#ifdef __cplusplus
}
#endif