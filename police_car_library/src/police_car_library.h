#pragma once

#include <Arduino.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ===== Constants (mirrors your .cpp) =====
#define REPEAT_MODE 0
#define QUEUE_MODE  1
#define CUSTOM_FUNCTION_MODE 2
#define CAPACITY    70



// ===== Types =====
typedef void (*custom_timer_function_callback)();


typedef struct tone_item {
  uint16_t frequency;
  uint16_t duration_ms;
} tone_item;

typedef struct led_item {
  uint16_t led_sequence;
  uint16_t duration_ms;
} led_item;

extern int LED_PIN[8];
extern int Pin_Motor_1_speed;
extern int Pin_Motor_1_Dir;
extern int Pin_Motor_2_speed;
extern int Pin_Motor_2_Dir;
extern int Pin_Buzzer;

// ===== API Functions =====
void library_set_up(uint8_t timer_purpose_mode = REPEAT_MODE, uint8_t use_buzzer = 1, uint8_t use_led = 1);
void set_up_timer(int index, int duration_ms);

bool set_repeat_mode(void);
bool set_queue_mode(void);

bool add_tone(uint16_t frequency, uint16_t duration_ms);

bool add_led(bool LED_1, bool LED_2, bool LED_3, bool LED_4,
             bool LED_5, bool LED_6, bool LED_7, bool LED_8,
             uint16_t duration_ms);

void Move(int left_speed, int right_speed, int duration_ms);

#ifdef __cplusplus
}
#endif
