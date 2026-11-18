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
    { GPIOE, GPIO_PIN_2 },   // A  – bar graph pin 1
    { GPIOE, GPIO_PIN_4 },   // B  – pin 2
    { GPIOE, GPIO_PIN_5 },   // C  – pin 3
    { GPIOE, GPIO_PIN_6 },   // D  – pin 4
    { GPIOE, GPIO_PIN_3 },   // E  – pin 5
    { GPIOF, GPIO_PIN_8 },   // F  – pin 6
    { GPIOF, GPIO_PIN_7 },   // G  – pin 7
    { GPIOF, GPIO_PIN_9 },   // H  – pin 8 (red)
    { GPIOG, GPIO_PIN_1 },   // I  – pin 9 (red)
    { GPIOG, GPIO_PIN_0 },   // J  – pin 10 (red)
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
