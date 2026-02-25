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
	
	pinMode(2, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(A0, OUTPUT);

	add_tone(G4, 300);
	add_tone(G4, 300);
	add_tone(A4, 600);
	add_tone(G4, 600);
	add_tone(C5, 600);
	add_tone(B4, 900);

	add_tone(STOP_TONE, 200);

	add_tone(G4, 300);
	add_tone(G4, 300);
	add_tone(A4, 600);
	add_tone(G4, 600);
	add_tone(D5, 600);
	add_tone(C5, 900);

	add_tone(STOP_TONE, 200);

	add_tone(G4, 300);
	add_tone(G4, 300);
	add_tone(G5, 600);
	add_tone(E5, 600);
	add_tone(C5, 600);
	add_tone(B4, 600);
	add_tone(A4, 900);

	add_tone(STOP_TONE, 200);

	add_tone(F5, 300);
	add_tone(F5, 300);
	add_tone(E5, 600);
	add_tone(C5, 600);
	add_tone(D5, 600);
	add_tone(C5, 1200);

	add_tone(STOP_TONE, 3000);  

	add_led(1, 0, 0, 0, 0, 0, 0, 1, 500);
	add_led(0, 1, 0, 0, 0, 0, 1, 0, 500);
	add_led(0, 0, 1, 0, 0, 1, 0, 0, 500);
	add_led(0, 0, 0, 1, 1, 0, 0, 0, 500);
	add_led(0, 0, 0, 1, 1, 0, 0, 0, 500);
	add_led(0, 0, 1, 0, 0, 1, 0, 0, 500);
	add_led(0, 1, 0, 0, 0, 0, 1, 0, 500);
	add_led(1, 0, 0, 0, 0, 0, 0, 1, 500);


	set_up_timer(1, 5);
}

void loop() 
{
  	// put your main code here, to run repeatedly:

}
