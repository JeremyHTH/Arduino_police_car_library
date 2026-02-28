#include <police_car_library.h>


void setup() {

	library_set_up(REPEAT_MODE, 1, 1, 1);

	add_led(1, 0, 0, 0, 0, 0, 0, 1, 500);
	add_led(0, 1, 0, 0, 0, 0, 1, 0, 500);
	add_led(0, 0, 1, 0, 0, 1, 0, 0, 500);
	add_led(0, 0, 0, 1, 1, 0, 0, 0, 500);
	add_led(0, 0, 0, 1, 1, 0, 0, 0, 500);
	add_led(0, 0, 1, 0, 0, 1, 0, 0, 500);
	add_led(0, 1, 0, 0, 0, 0, 1, 0, 500);
	add_led(1, 0, 0, 0, 0, 0, 0, 1, 500);


	set_up_timer(1, 1);
}

void loop() 
{
  	// put your main code here, to run repeatedly:
	move(200, 200, 500);
	move(-200, 200, 500);
	move(200, -200, 500);
	move(-200, -200, 500);
}
