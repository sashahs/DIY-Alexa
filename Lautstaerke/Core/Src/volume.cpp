#include "volume.h"
#include <cstdlib>

// Lautstaerke berechnen
int32_t Volume::calculateVolume(const int32_t* pcmData, int length)
{
    if (pcmData == nullptr || length <= 0)
    {
        return 0;
    }

    int64_t sum = 0;

    // Mittelwert berechnen
    for (int i = 0; i < length; i++)
    {
        sum += pcmData[i];
    }

    // Offset bestimmen
    int32_t offset = sum / length;

    int64_t volume = 0;

    // Absolute Abweichungen vom Offset berechnen
    for (int i = 0; i < length; i++)
    {
        volume += std::abs(pcmData[i] - offset);
    }

    return volume / length;
}


// Anzahl der leuchtenden LEDs bestimmen
int Volume::calculateLEDCount(int32_t volume)
{
    int count = volume / LED_STEP;

    if (count > LED_COUNT)
    {
        count = LED_COUNT;
    }

    if (count < 0)
    {
        count = 0;
    }

    return count;
}


// TODO: Schwellwertueberschreitung erkennen

// TODO: LED-Array ansteuern

// TODO: Steckdose ein- und ausschalten
