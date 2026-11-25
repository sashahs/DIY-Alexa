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

**Status: Code compiles, not yet tested on hardware.**

**Next steps:** Timing test on the board, then test with the microphone (values, wav file).
