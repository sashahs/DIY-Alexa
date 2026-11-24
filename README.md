# DIY Alexa – Group 3

## Pin Configuration

**LED Bar (DC10EWA)**

The LED pin mapping has been reversed to simplify the wiring.

| LED | STM32 GPIO |
|---|---|
| 1 | PG0 |
| 2 | PG1 |
| 3 | PF9 |
| 4 | PF7 |
| 5 | PF8 |
| 6 | PE3 |
| 7 | PE6 |
| 8 | PE5 |
| 9 | PE4 |
| 10 | PE2 |

**RF Transmitter (FS1000A)**

- RF_SEND: PF13 (D7)
- VCC: 5V
- GND: GND
- Driver circuit: BC237 transistor and two 4.7 kΩ resistors

**Pins occupied by the board**

- USART3 (ST-LINK virtual COM, 115200 baud): PD8, PD9
- On-board LEDs / button: PB0, PB7, PB14, PC13

## Implemented Features

### Aufgabe 1.1 & 1.2 – C++ Project and LED Array

Files: `leds.h`, `leds.cpp`

Functions:
- `ledsInit()`
- `setLeds(n)`
- `ledCount()`
- `volumeToLeds(volume, maxVolume)`

Status: LED array tested successfully.

### Aufgabe 1.3 – Timer-Based Delay

Files: `timing.h`, `timing.cpp`

- TIM2 configured with a 1 MHz counter frequency (PSC = 83)
- Implemented `delay350Microseconds(uint32_t n)`
- Each unit corresponds to 350 µs
- Timer started using `HAL_TIM_Base_Start(&htim2)`

Status: Basic LED blinking test successful.

### Aufgabe 1.4 – RF Socket Control

Files: `rf.h`, `rf.cpp`

Functions:
- `transmit(highPulses, lowPulses)` – Generates timed RF pulses
- `sendSequence(bitSequence)` – Sends a 12-bit command, repeated 10 times

Commands:
- ON: `0b111111000010`
- OFF: `0b111111000001`

The RF control signal is generated through PF13 using the BC237 driver circuit.

**Status: Implementation completed, hardware debugging required.**

The socket's red indicator turns on during testing but does not turn off when the OFF command is transmitted.

The STM32 program runs, but reliable RF switching has not yet been verified.

**Next steps:** Investigate the RF transmitter signal, timing and hardware connections.

## Development Environment

- Board: NUCLEO-F429ZI
- STM32CubeMX / STM32CubeIDE
- C++ application code with STM32 HAL
