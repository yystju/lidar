/**
 * Stepper motor
 */
#include <stdio.h>
#include <wiringPi.h>

#define	A   7
#define B   0
#define C   2
#define D   3

#define MaskA 0x1
#define MaskB 0x2
#define MaskC 0x4
#define MaskD 0x8

#define INTERVAL 10

#ifdef DEBUG 
#	define TRACE(X, ...) fprintf(stdout, X, __VA_ARGS__);
#else
#	define TRACE(X, ...)
#endif

void init() {
	wiringPiSetup();
	pinMode(A, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(C, OUTPUT);
	pinMode(D, OUTPUT);
}

void cleanup() {
	digitalWrite(A, LOW);
	digitalWrite(B, LOW);
	digitalWrite(C, LOW);
	digitalWrite(D, LOW);
	
	pinMode(A, INPUT);
	pinMode(B, INPUT);
	pinMode(C, INPUT);
	pinMode(D, INPUT);
}

void oneStep(char data, int interval) {
	TRACE("[ONE STEP] A : %d, B : %d, C : %d, D : %d\n", (data & MaskA) > 0, (data & MaskB) > 0, (data & MaskC) > 0, (data & MaskD)> 0);
	digitalWrite(A, (data & MaskA) ? HIGH : LOW);
	digitalWrite(B, (data & MaskB) ? HIGH : LOW);
	digitalWrite(C, (data & MaskC) ? HIGH : LOW);
	digitalWrite(D, (data & MaskD) ? HIGH : LOW);
	delay(interval);
}

char forwardCode[4] = {
	0x1, 0x2, 0x4, 0x8
};

char backwardCode[4] = {
	0x2, 0x1, 0x8, 0x4
};

void stepForward() {
	int i;
	for(i = 0; i < 4; ++i) {
		oneStep(forwardCode[i], INTERVAL);
	}
}

void stepBackward() {
	int i;
	for(i = 0; i < 4; ++i) {
		oneStep(backwardCode[i], INTERVAL);
	}
}

int main (void)
{
	int i;
	init();

	while(1) {	
	for (i = 0; i < 200; ++i)
	{
		stepForward();
	}
	for (i = 0; i < 200; ++i)
	{
		stepBackward();
	}
	}

	//cleanup();
	return 0 ;
}
