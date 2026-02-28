# Arduino Police Car Library

`police_car_library` is an AVR-focused Arduino library for simple police-car effects:

- Timed buzzer tone sequences
- Timed 8-LED pattern sequences
- Basic dual-motor movement helper
- Timer interrupt callback mode for custom behavior

## Supported Targets

- `architectures=avr` (from `library.properties`)
- Designed around AVR timer registers

## Default Pin Mapping

These globals are defined in the library and can be overridden in your sketch before use:

- `LED_PIN[8] = {10, 9, 8, 7, 6, 5, 4, 2}`
- `Pin_Motor_1_speed = 3`
- `Pin_Motor_1_Dir = 12`
- `Pin_Motor_2_speed = 11`
- `Pin_Motor_2_Dir = 13`
- `Pin_Buzzer = A0`

## Core Modes

- `REPEAT_MODE` (`0`): loops through queued tone/LED items.
- `QUEUE_MODE` (`1`): consumes items once in FIFO order.
- `CUSTOM_FUNCTION_MODE` (`2`): runs your callback on each timer tick.

## Quick Start

1. Include the library and call `library_set_up(...)`.
2. Configure all used pins with `pinMode(...)`.
3. Add tone and/or LED sequence items with `add_tone(...)` and `add_led(...)`.
4. Start timer interrupts with `set_up_timer(1, duration_ms)`.

```cpp
#include <police_car_library.h>

void setup() {
  library_set_up(REPEAT_MODE, 1, 1, 0);

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);

  add_tone(440, 300);
  add_tone(0, 100); // silence

  add_led(1,0,0,0,0,0,0,1,200);
  add_led(0,1,0,0,0,0,1,0,200);

  set_up_timer(1, 5); // 5 ms tick on Timer1
}

void loop() {}
```

## API

### Initialization

```cpp
void library_set_up(
  uint8_t timer_purpose_mode = REPEAT_MODE,
  uint8_t use_buzzer = 1,
  uint8_t use_led = 1,
  uint8_t debug_mode = 0
);
```

- Allocates internal buffers (capacity `70` each for tones and LEDs).
- Sets mode and enables/disables buzzer and LED processing.
- If `debug_mode` is enabled, prints startup info at `9600` baud.

### Timer Setup

```cpp
void set_up_timer(int index, int duration_ms);
```

- `index == 1`: configures `Timer1` compare interrupt.
- `index == 2`: configures `Timer2` registers.
- Runtime processing currently happens in `ISR(TIMER1_COMPA_vect)`.

### Mode Switching

```cpp
bool set_repeat_mode(void);
bool set_queue_mode(void);
bool set_custom_function_mode(custom_timer_function_callback cb);
```

- `set_custom_function_mode` returns `false` if `cb == nullptr`.

### Add Sequence Items

```cpp
bool add_tone(uint16_t frequency, uint16_t duration_ms);
```

- `frequency == 0` means silence (`noTone`).
- Returns `false` only when the tone buffer is full.
- Returns `true` without enqueueing when `duration_ms == 0` or buzzer is disabled.

```cpp
bool add_led(
  bool LED_1, bool LED_2, bool LED_3, bool LED_4,
  bool LED_5, bool LED_6, bool LED_7, bool LED_8,
  uint16_t duration_ms
);
```

- `LED_1..LED_8` map to `LED_PIN[0..7]`.
- Returns `false` only when the LED buffer is full.
- Returns `true` without enqueueing when `duration_ms == 0` or LED output is disabled.

### Motor Helper

```cpp
void Move(int left_speed, int right_speed, int duration_ms);
```

- Uses PWM magnitude (`abs(speed)`) and direction pins for each motor.
- Blocks for `duration_ms` using `delay(...)`.

## Included Examples

- `examples/Repeating_Mode/Repeating_Mode.ino`
- `examples/Queue_Mode/Queue_Mode.ino`
- `examples/Custom_Function_Mode/Custom_Function_Mode.ino`

## Notes and Limitations

- The library directly configures AVR timer registers; avoid conflicts with other timer-heavy libraries.
- `CUSTOM_FUNCTION_MODE` callbacks run inside an ISR. Keep callbacks short and non-blocking.
- The library does not configure pin modes for you.
- Buffer memory is allocated with `malloc` in `library_set_up`; there is no explicit free path.
