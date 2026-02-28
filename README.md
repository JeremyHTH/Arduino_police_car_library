# Arduino Police Car Library

`police_car_library` is an AVR-focused Arduino library for police-car style effects:

- Timed buzzer tone sequences
- Timed 8-LED pattern sequences
- Basic dual-motor movement helper
- Timer callback mode for custom behavior

## Supported Targets

- `architectures=avr` (from `library.properties`)
- Uses AVR timer registers directly (`Timer1` / `Timer2` setup)

## Default Pin Mapping

These globals are provided by the library. You can override them in your sketch **before** calling `library_set_up(...)`.

- `LED_PIN[8] = {10, 9, 8, 7, 6, 5, 4, 2}`
- `PIN_MOTOR_1_SPEED = 3`
- `PIN_MOTOR_1_DIR = 12`
- `PIN_MOTOR_2_SPEED = 11`
- `PIN_MOTOR_2_DIR = 13`
- `PIN_BUZZER = A0`

## Modes

- `REPEAT_MODE` (`0`): cycles through queued tone/LED items repeatedly.
- `QUEUE_MODE` (`1`): plays items once in FIFO order.
- `CUSTOM_FUNCTION_MODE` (`2`): calls your callback every timer tick.

## Quick Start

```cpp
#include <police_car_library.h>

void setup() {
  library_set_up(REPEAT_MODE, 1, 1, 0);

  add_tone(440, 300);
  add_tone(0, 100); // silence

  add_led(1, 0, 0, 0, 0, 0, 0, 1, 200);
  add_led(0, 1, 0, 0, 0, 0, 1, 0, 200);

  set_up_timer(1, 1); // 1 ms timer tick
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

- Allocates tone and LED buffers (`CAPACITY = 70` each).
- Sets mode and enables/disables buzzer and LED processing.
- Configures LED, motor, and buzzer pins as `OUTPUT`.
- If `debug_mode` is enabled, prints startup data to `Serial` at `9600`.

### Timer Setup

```cpp
void set_up_timer(int index, int duration_ms);
```

- `index == 1`: configures `Timer1` compare interrupt (`ISR(TIMER1_COMPA_vect)` drives the library runtime).
- `index == 2`: configures `Timer2` compare register and interrupt enable bits.
- In current code, playback/callback processing is implemented in the `Timer1` ISR.

### Mode Switching

```cpp
bool set_repeat_mode(void);
bool set_queue_mode(void);
bool set_custom_function_mode(custom_timer_function_callback cb);
```

- `set_custom_function_mode` returns `false` if `cb == nullptr`.
- Switching to repeat/queue mode clears any previously registered custom callback.

### Add Sequence Items

```cpp
bool add_tone(uint16_t frequency, uint16_t duration_ms);
```

- `frequency == 0` means silence (`noTone`).
- Returns `false` only when the tone buffer is full.
- Returns `true` without enqueueing if `duration_ms == 0` or buzzer is disabled.
- In `QUEUE_MODE`, when queue is empty, the first added tone starts immediately.

```cpp
bool add_led(
  bool LED_1, bool LED_2, bool LED_3, bool LED_4,
  bool LED_5, bool LED_6, bool LED_7, bool LED_8,
  uint16_t duration_ms
);
```

- `LED_1..LED_8` map to `LED_PIN[0..7]`.
- Returns `false` only when the LED buffer is full.
- Returns `true` without enqueueing if `duration_ms == 0` or LED output is disabled.
- In `QUEUE_MODE`, when queue is empty, the first LED pattern is applied immediately.

### Motor Helper

```cpp
void move(int left_speed, int right_speed, int duration_ms);
```

- Uses PWM magnitude (`abs(speed)`) and direction pins for each motor.
- Blocks for `duration_ms` using `delay(...)`.

## Included Examples

- `police_car_library/examples/repeating_mode/repeating_mode.ino`
- `police_car_library/examples/queue_mode/queue_mode.ino`
- `police_car_library/examples/custom_function_mode/custom_function_mode.ino`
- `police_car_library/examples/led_and_move/led_and_move.ino`

## Notes and Limitations

- The library directly touches AVR timer registers; avoid conflicts with other timer-heavy libraries.
- `CUSTOM_FUNCTION_MODE` callbacks run inside an ISR. Keep callbacks short and non-blocking (avoid long `Serial` prints, `delay`, etc.).
- Buffers are allocated with `malloc` in `library_set_up`; no explicit free path is provided.
- `library_set_up` is intended for setup-time initialization (not repeated re-initialization).
