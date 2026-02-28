#include <police_car_library.h>



void Blinking_LED() 
{
    static int LED_PIN_LIST[8] = {10, 9, 8, 7, 6, 5, 4, 2};
    static int state = 0;

    for (int i = 0; i < 8; i++)
    {
        digitalWrite(LED_PIN_LIST[i], state);
    }

    state = !state;
}

volatile int counter = 0;
void custom_timer_function_2() 
{
    counter++;
    Serial.println(counter);
}


void setup() 
{
    library_set_up(CUSTOM_FUNCTION_MODE, 1, 1, 1);
    set_custom_function_mode(Blinking_LED);

    set_up_timer(1, 500);

}


void loop() 
{
  	// put your main code here, to run repeatedly:

}
