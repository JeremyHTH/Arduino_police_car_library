#include "police_car_library.h"

#define REPEAT_MODE 0
#define QUEUE_MODE 1
#define CAPACITY 70

#define ENABLE_BUZZER 1
#define ENABLE_LED 1



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

void set_up_timer(int index, int duration_ms)
{
  cli();
  duration_ms_timer_1 = duration_ms;

  if (index == 1) {
    // -------- TIMER1 --------
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    // Prescaler = 256
    uint32_t ticks = (F_CPU / 256) * duration_ms / 1000;
    Serial.println("entered 1");
    Serial.println(ticks);

    if (ticks > 65535) ticks = 65535;

    OCR1A = ticks;

    TCCR1B |= (1 << WGM12);             // CTC
    TCCR1B |= (1 << CS12);              // prescaler  
    TIMSK1 |= (1 << OCIE1A);
  }

  else if (index == 2) {
    // -------- TIMER2 --------
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;

    // Prescaler 64 (simple)
    uint32_t ticks = (F_CPU / 64) * duration_ms / 1000;
    Serial.println("entered 2");
    Serial.println(ticks);

    if (ticks > 255) ticks = 255;

    OCR2A = ticks;

    TCCR2A |= (1 << WGM21); // CTC
    TCCR2B |= (1 << CS22) ; // 1024
    TIMSK2 |= (1 << OCIE2A);
  }

  sei();
}

bool set_repeat_mode()
{
  mode = REPEAT_MODE;
  return true;
}

bool set_queue_mode()
{
  mode = QUEUE_MODE;
  return true;
}

bool add_tone(uint16_t frequency, uint16_t duration_ms)
{
  if (duration_ms == 0)
  {
    return true;
  }

  if (tone_count >= CAPACITY)
  {
    return false;
  }

  cli();
  tone_buffer[tone_tail] = {frequency, duration_ms};
  tone_tail = (uint8_t)((tone_tail + 1) % CAPACITY);
  tone_count++;
  sei();

  return true;
}

bool add_led(bool LED_1, bool LED_2, bool LED_3, bool LED_4, bool LED_5 , bool LED_6, bool LED_7, bool LED_8, uint16_t duration_ms)
{
  if (duration_ms == 0)
  {
    return true;
  }

  if (led_count >= CAPACITY)
  {
    return false;
  }


  cli();
  led_buffer[led_tail] = {(0 | (LED_1? 1 : 0) << 0 | (LED_2? 1 : 0)  << 1 | (LED_3? 1 : 0)  << 2 | (LED_4? 1 : 0)  << 3 | (LED_5? 1 : 0)  << 4 | (LED_6? 1 : 0) << 5 | (LED_7? 1 : 0 ) << 6 | (LED_8? 1 : 0 ) << 7), duration_ms};
  led_tail = (uint8_t)((led_tail + 1) % CAPACITY);
  led_count++;
  sei();

  return true;
}


ISR(TIMER1_COMPA_vect) 
{
  // Serial.print(buffer[current_playing_index].frequency);
  // Serial.print(" ");
  // Serial.print(buffer[current_playing_index].duration_ms); 
  // Serial.print(" ");
  // Serial.print(millis());
  // Serial.print(" ");
  // Serial.print(last_starting_time);
  // Serial.println();
  led_number_of_tick++;
  tone_number_of_tick++;

#if ENABLE_BUZZER
  if (tone_count > 0 && tone_number_of_tick >= tone_buffer[tone_current_playing_index].duration_ms / duration_ms_timer_1)
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

    if (tone_buffer[tone_current_playing_index].frequency == 0)
    {
      noTone(Pin_Buzzer);
    }
    else
    {
      tone(Pin_Buzzer, tone_buffer[tone_current_playing_index].frequency);
      
    }

    tone_number_of_tick = 0;
  }
#endif

#if ENABLE_LED
  if (led_count > 0 && led_number_of_tick >= led_buffer[led_current_playing_index].duration_ms / duration_ms_timer_1)
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

    for (int i = 0; i < 8; i++)
    {
      digitalWrite(LED_PIN[i], led_buffer[led_current_playing_index].led_sequence & 1 << i);
      // digitalWrite(LED_PIN[i], !digitalRead(LED_PIN[i]));
    }
    led_number_of_tick = 0;
  }
#endif  
  

  
}

void library_set_up()
{
  //In the class
  Serial.begin(9600);
  Serial.println("==============================");
  Serial.println("Started");
  tone_buffer = (tone_item*)malloc(sizeof(tone_item) * CAPACITY); 
  led_buffer = (led_item*)malloc(sizeof(led_item) * CAPACITY); 
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
