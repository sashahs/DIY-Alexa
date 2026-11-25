#ifndef VOLUME_H
#define VOLUME_H

#include <stdint.h>

class Volume
{
public:

    int32_t calculateVolume(const int32_t* pcmData, int length);

    int calculateLEDCount(int32_t volume);

private:

    static constexpr int LED_COUNT = 7;
    static constexpr int LED_STEP = 500;
    static constexpr int32_t THRESHOLD = 2000;

    bool socketOn = false;
    bool wasAboveThreshold = false;
};

#endif
