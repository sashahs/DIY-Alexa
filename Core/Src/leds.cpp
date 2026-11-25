/*
 * leds.cpp
 *
 *
 */



#include "leds.h"
#include "main.h"

namespace {
  struct LedPin {
    GPIO_TypeDef* port;
    uint16_t      pin;
  };

  const LedPin LED_PINS[] = {

		    { GPIOG, GPIO_PIN_0 },  // LED 1
		    { GPIOG, GPIO_PIN_1 },  // LED 2
		    { GPIOF, GPIO_PIN_9 },  // LED 3
		    { GPIOF, GPIO_PIN_7 },  // LED 4
		    { GPIOF, GPIO_PIN_8 },  // LED 5
		    { GPIOE, GPIO_PIN_3 },  // LED 6
		    { GPIOE, GPIO_PIN_6 },  // LED 7
		    { GPIOE, GPIO_PIN_5 },  // LED 8
		    { GPIOE, GPIO_PIN_4 },  // LED 9
		    { GPIOE, GPIO_PIN_2 }   // LED 10


  };

  constexpr uint8_t NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
}

void ledsInit(void)
{
  setLeds(0);
}

void setLeds(uint8_t count)
{
  if (count > NUM_LEDS)
  {
    count = NUM_LEDS;                     // limit to 10
  }

  for (uint8_t i = 0; i < NUM_LEDS; i++)
  {
    if (i < count)
    {
      HAL_GPIO_WritePin(LED_PINS[i].port, LED_PINS[i].pin, GPIO_PIN_SET);     // LED on
    }
    else
    {
      HAL_GPIO_WritePin(LED_PINS[i].port, LED_PINS[i].pin, GPIO_PIN_RESET);   // LED off
    }
  }
}


uint8_t ledCount(void)
{
  return NUM_LEDS;
}


uint8_t volumeToLeds(uint32_t volume, uint32_t maxVolume)
{
  if (maxVolume == 0) return 0;
  if (volume >= maxVolume) return NUM_LEDS;
  return static_cast<uint8_t>((volume * NUM_LEDS) / maxVolume);   // linear: 0 to 10
}
