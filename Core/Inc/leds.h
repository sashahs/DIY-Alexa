/*
 * leds.h
 *
 *
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void    ledsInit(void);                                  // Turn all LEDs off to reset state (Helper)

void    setLeds(uint8_t count);                          // first count LEDs on (bottom to top) (Task 1.2)


uint8_t ledCount(void);                                  // = 10 (Helper)



uint8_t volumeToLeds(uint32_t volume, uint32_t maxVolume); //( for step 6 volume to LED count)

#ifdef __cplusplus
}

#endif

#endif /* INC_LEDS_H_ */
