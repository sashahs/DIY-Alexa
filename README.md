
# DIY Alexa – Gruppe 3

**Pins occupied by task:**
- LED bar (DC7G3HWA): PE2, PE4, PE5, PE6, PE3, PF8, PF7, PF9, PG1, PG0

**Pins occupied by board:**
- Terminal (USART3, ST-LINK virtual COM, 115200 baud): PD8, PD9
- On-board LEDs / button: PB0, PB7, PB14, PC13

**Added files / functions:**
- `leds.h / leds.cpp`: `ledsInit()`, `setLeds(n)`, `ledCount()`, `volumeToLeds(volume, max)`
