#include "raspi.h"

#include <wiringPi.h>

#ifdef __cplusplus
extern "C" {
#endif
    
void blink(int n) {
  wiringPiSetup();
  
  pinMode(0, OUTPUT);
  
  for (int i = 0; i < n; ++i) {
    digitalWrite(0, HIGH);
    delay(500);
    digitalWrite(0, LOW);
    delay(500);
  }
}
    
#ifdef __cplusplus
}
#endif