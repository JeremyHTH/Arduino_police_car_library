#include <police_car_library.h>

#define STOP_TONE 0
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 493
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784

void setup() {

	library_set_up(QUEUE_MODE, 1, 1, 1);

	set_up_timer(1, 1);
}

void loop() 
{
  	// put your main code here, to run repeatedly:
	add_tone(C4, 300);
	add_tone(D4, 300);
	add_tone(E4, 300);
	add_tone(F4, 300);
	add_tone(G4, 300);
	add_tone(A4, 300);
	add_tone(B4, 300);
	add_tone(C5, 300);

	add_led(1, 0, 0, 0, 0, 0, 0, 1, 500);
	add_led(0, 1, 0, 0, 0, 0, 1, 0, 500);
	add_led(0, 0, 1, 0, 0, 1, 0, 0, 500);
	add_led(0, 0, 0, 1, 1, 0, 0, 0, 500);
	add_led(0, 0, 0, 1, 1, 0, 0, 0, 500);
	add_led(0, 0, 1, 0, 0, 1, 0, 0, 500);
	add_led(0, 1, 0, 0, 0, 0, 1, 0, 500);
	add_led(1, 0, 0, 0, 0, 0, 0, 1, 500);
	delay(10000);
}
