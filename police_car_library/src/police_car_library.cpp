#include "police_car_library.h"


int LED_PIN[8] = {10, 9, 8, 7, 6, 5, 4, 2};
int Pin_Motor_1_speed = 3;
int Pin_Motor_1_Dir = 12;
int Pin_Motor_2_speed = 11;
int Pin_Motor_2_Dir = 13;
int Pin_Buzzer = A0; 


tone_item* tone_buffer = nullptr;
led_item* led_buffer = nullptr; 

volatile uint8_t mode = REPEAT_MODE;

volatile uint8_t tone_head = 0;   
volatile uint8_t tone_tail = 0;   
volatile uint8_t tone_count = 0;
volatile uint8_t tone_current_playing_index = 0;
volatile unsigned long tone_number_of_tick = 0;

volatile uint8_t led_head = 0;   
volatile uint8_t led_tail = 0;   
volatile uint8_t led_count = 0;
volatile uint8_t led_current_playing_index = 0;
volatile unsigned long led_number_of_tick = 0;

volatile int duration_ms_timer_1 = 1;
volatile uint8_t enable_buzzer = 1;
volatile uint8_t enable_led = 1;
volatile uint8_t enable_debug = 1;

custom_timer_function_callback custom_function = nullptr;

void set_up_timer(int index, int duration_ms)
{
	cli();
	duration_ms_timer_1 = duration_ms;

	if (index == 1) 
	{
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1  = 0;

		// Prescaler = 256
		uint32_t ticks = (F_CPU / 256) * duration_ms / 1000;
		if (enable_debug)
		{
			Serial.print("duration_ms: ");
			Serial.print(duration_ms);
			Serial.print(" ticks: ");
			Serial.println(ticks);
		}

		if (ticks > 65535) ticks = 65535;

		OCR1A = ticks;

		TCCR1B |= (1 << WGM12);             
		TCCR1B |= (1 << CS12);             
		TIMSK1 |= (1 << OCIE1A);
	}

	else if (index == 2) 
	{
		TCCR2A = 0;
		TCCR2B = 0;
		TCNT2  = 0;

		uint32_t ticks = (F_CPU / 64) * duration_ms / 1000;
		if (enable_debug)
		{
			Serial.print("duration_ms: ");
			Serial.print(duration_ms);
			Serial.print(" ticks: ");
			Serial.println(ticks);
		}

		if (ticks > 255) ticks = 255;

		OCR2A = ticks;

		TCCR2A |= (1 << WGM21); 
		TCCR2B |= (1 << CS22) ; 
		TIMSK2 |= (1 << OCIE2A);
	}

	sei();
}

bool set_repeat_mode()
{
	if (custom_function != nullptr)
	{
		custom_function = nullptr;
	}
	mode = REPEAT_MODE;
	return true;
}

bool set_queue_mode()
{
	if (custom_function != nullptr)
	{
		custom_function = nullptr;
	}
	mode = QUEUE_MODE;
	return true;
}

bool set_custom_function_mode(custom_timer_function_callback current_custom_function)
{
	if (current_custom_function == nullptr)
	{
		return false;
	}

	mode = CUSTOM_FUNCTION_MODE;
	custom_function = current_custom_function;
	return true;
}

bool add_tone(uint16_t frequency, uint16_t duration_ms)
{
	if (duration_ms == 0 || !enable_buzzer)
	{
		return true;
	}

	if (tone_count >= CAPACITY)
	{
		return false;
	}

	bool start_now = false;
	cli();
	if (mode == QUEUE_MODE && tone_count == 0)
	{
		tone_current_playing_index = tone_head;
		tone_number_of_tick = 0;
		start_now = true;
	}
	tone_buffer[tone_tail] = {frequency, duration_ms};
	tone_tail = (uint8_t)((tone_tail + 1) % CAPACITY);
	tone_count++;
	sei();

	if (start_now)
	{
		if (frequency == 0)
		{
			noTone(Pin_Buzzer);
		}
		else
		{
			tone(Pin_Buzzer, frequency);
		}
	}

	return true;
}

bool add_led(bool LED_1, bool LED_2, bool LED_3, bool LED_4, bool LED_5 , bool LED_6, bool LED_7, bool LED_8, uint16_t duration_ms)
{
	if (duration_ms == 0 || !enable_led)
	{
		return true;
	}

	if (led_count >= CAPACITY)
	{
		return false;
	}

	uint8_t led_sequence = (uint8_t)(0 | (LED_1 ? 1 : 0) << 0 | (LED_2 ? 1 : 0) << 1 | (LED_3 ? 1 : 0) << 2 | (LED_4 ? 1 : 0) << 3 | (LED_5 ? 1 : 0) << 4 | (LED_6 ? 1 : 0) << 5 | (LED_7 ? 1 : 0) << 6 | (LED_8 ? 1 : 0) << 7);
	bool start_now = false;

	cli();
	if (mode == QUEUE_MODE && led_count == 0)
	{
		led_current_playing_index = led_head;
		led_number_of_tick = 0;
		start_now = true;
	}
	led_buffer[led_tail] = {led_sequence, duration_ms};
	led_tail = (uint8_t)((led_tail + 1) % CAPACITY);
	led_count++;
	sei();

	if (start_now)
	{
		for (int i = 0; i < 8; i++)
		{
			digitalWrite(LED_PIN[i], led_sequence & (1 << i));
		}
	}

	return true;
}


ISR(TIMER1_COMPA_vect) 
{
	// if (enable_debug && mode != CUSTOM_FUNCTION_MODE)
	// {
	// 	Serial.print("led_current_playing_index=");
	// 	Serial.print(led_current_playing_index);
	// 	Serial.print(" led_seq=");
	// 	Serial.print(led_buffer[led_current_playing_index].led_sequence);
	// 	Serial.print(" led_duration_ms=");
	// 	Serial.print(led_buffer[led_current_playing_index].duration_ms);
	// 	Serial.print(" tone_current_playing_index=");
	// 	Serial.print(tone_current_playing_index);
	// 	Serial.print(" tone_freq_hz=");
	// 	Serial.print(tone_buffer[tone_current_playing_index].frequency);
	// 	Serial.print(" tone_duration_ms=");
	// 	Serial.println(tone_buffer[tone_current_playing_index].duration_ms);
	// }

  if (led_count > 0)
  {
    led_number_of_tick++;
  }

  if (tone_count > 0)
  {
    tone_number_of_tick++;
  }

	if (mode == CUSTOM_FUNCTION_MODE)
	{
		if (custom_function != nullptr)
		{
			custom_function();
		}
	}
	else
	{
		if (enable_buzzer && tone_count > 0 && tone_number_of_tick >= tone_buffer[tone_current_playing_index].duration_ms / duration_ms_timer_1)
		{
			if (mode == QUEUE_MODE)
			{
				tone_head = (tone_head + 1) % CAPACITY;
				tone_count--;
				tone_current_playing_index = (tone_current_playing_index + 1) % CAPACITY;
			}
			else
			{
				tone_current_playing_index = (tone_current_playing_index + 1) % tone_tail;
			}

			if (tone_buffer[tone_current_playing_index].frequency == 0 || tone_count == 0)
			{
				noTone(Pin_Buzzer);
			}
			else
			{
				tone(Pin_Buzzer, tone_buffer[tone_current_playing_index].frequency);
				
			}

			tone_number_of_tick = 0;
		}



		if (enable_led && led_count > 0 && led_number_of_tick >= led_buffer[led_current_playing_index].duration_ms / duration_ms_timer_1)
		{
		if (mode == QUEUE_MODE)
		{
			led_head = (led_head + 1) % CAPACITY;
			led_count--;
			led_current_playing_index = (led_current_playing_index + 1) % CAPACITY;
		}
		else
		{
			led_current_playing_index = (led_current_playing_index + 1) % led_tail;
		}
		
		if (led_count == 0)
		{
			for (int i = 0; i < 8; i++)
			{
				digitalWrite(LED_PIN[i], LOW);
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				digitalWrite(LED_PIN[i], led_buffer[led_current_playing_index].led_sequence & 1 << i);
			}
		}
		led_number_of_tick = 0;
		}
	}

}

void library_set_up(uint8_t timer_purpose_mode, uint8_t use_buzzer, uint8_t use_led, uint8_t debug_mode)
{

	if (debug_mode)
	{
		Serial.begin(9600);
		Serial.println("==============================");
		Serial.println("Started");
		Serial.print("use_buzzer: ");
		Serial.println(use_buzzer);
		Serial.print("use_led: ");
		Serial.println(use_led);
		Serial.print("mode: ");
		Serial.println(timer_purpose_mode);
		Serial.print("debug_mode: ");
		Serial.println(debug_mode);
	}

	tone_buffer = (tone_item*)malloc(sizeof(tone_item) * CAPACITY); 
	led_buffer = (led_item*)malloc(sizeof(led_item) * CAPACITY); 
	enable_buzzer = use_buzzer;
	enable_led = use_led; 
	mode = timer_purpose_mode;
	enable_debug = debug_mode;
}

void Move(int left_speed, int right_speed, int duration_ms)
{
	analogWrite(Pin_Motor_1_speed, abs(left_speed));
	analogWrite(Pin_Motor_2_speed, abs(right_speed));

	if (left_speed < 0)
	{
		digitalWrite(Pin_Motor_1_Dir, HIGH);
	}
	else 
	{
		digitalWrite(Pin_Motor_1_Dir, LOW);
	}

	if (right_speed < 0)
	{
		digitalWrite(Pin_Motor_2_Dir, LOW);
	}
	else 
	{
		digitalWrite(Pin_Motor_2_Dir, HIGH);
	}

	delay(duration_ms);
}
