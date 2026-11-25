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

**Microphone (SPH0645LM4H-B)**

- BCLK: PB10 (D36, I2S2_CK)
- LRCL: PB12 (D19, I2S2_WS)
- DOUT: PC3 (A2, I2S2_SD)
- SEL: GND (left channel)
- VCC: 3.3 V

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

### Aufgabe 1.5 – Microphone (I2S + DMA)

Files: `mic.h`, `mic.cpp`

Pins (SPH0645LM4H-B):
- BCLK: PB10 (I2S2_CK)
- LRCL: PB12 (I2S2_WS)
- DOUT: PC3 (I2S2_SD)
- SEL: GND (left channel), VCC: 3.3 V

Configuration:
- I2S2: Master Receive, Philips, 24 bit data on 32 bit frame, 16 kHz (real: 15.957 kHz)
- DMA1 Stream 3 (SPI2_RX): circular, half word, priority high

Functions:
- `micStart()` – starts the DMA
- `micReadBlock(out)` – returns 250 new samples per DMA half (15.6 ms), else 0
- `micRecord(dest, n)` – records n samples (16000 = 1 s), returns the duration in ms
- `micOverruns()` – number of lost DMA halves (processing too slow)

Sample format: 2 × 16 bit → 32 bit, `>> 14` → signed 18-bit sample; every second 32-bit word (right channel) is discarded.

printf: retargeted to USART3 (115200 baud) in `main.c`.

**Status: Tested on hardware.** 16000 samples recorded in 987 ms, 0 overruns. Silence gives values around -7000 (DC offset of the microphone).

**Next steps:** Listening test with the Wav-file Creator Script.

## Test Switches (`main.c`, USER CODE PD)

| Define | Test |
|---|---|
| `LED_TEST` | A1.2: LED bar counts up and down |
| `MIC_TEST` | A1.5: 1 s recording at startup, prints duration, overruns and first samples |
| `MIC_PRINT_ALL` | A1.5: prints all 16000 samples between `WAV_START` / `WAV_END` (needs `MIC_TEST 1`) |
| `RF_TEST` | A1.4: socket ON / 10 s / OFF / 10 s, prints the duration of each `sendSequence` (expected ~448 ms) |

Set to `1` to enable, `0` to disable.

## Development Environment

- Board: NUCLEO-F429ZI
- STM32CubeMX / STM32CubeIDE
- C++ application code with STM32 HAL
