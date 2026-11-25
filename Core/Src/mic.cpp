/*
 * mic.cpp
 *
 * Mikrofon SPH0645LM4H-B ueber I2S2 + DMA (Aufgabe 1.5)
 *
 * Ablauf:
 *  - Der DMA schreibt die I2S-Daten selbststaendig in inputBuffer (Circular Mode).
 *  - Ist die erste Haelfte voll  -> HAL_I2S_RxHalfCpltCallback -> halfReady = true
 *  - Ist die zweite Haelfte voll -> HAL_I2S_RxCpltCallback     -> fullReady = true
 *  - Waehrend der DMA die eine Haelfte fuellt, wird die andere in micReadBlock() verarbeitet.
 */

#include "mic.h"
#include "main.h"

extern "C" I2S_HandleTypeDef hi2s2;   // wird in main.c von CubeMX angelegt

namespace {

  // HAL zaehlt bei 24-bit-Daten in 32-bit-Einheiten.
  // Der DMA arbeitet aber mit 16-bit (Half Word) -> der Puffer braucht doppelt so viele uint16_t.
  constexpr uint16_t I2S_BUF_SIZE  = 1000;              // Groesse fuer HAL_I2S_Receive_DMA
  constexpr uint16_t BUF_HALFWORDS = I2S_BUF_SIZE * 2;  // 2000 x uint16_t im Speicher
  constexpr uint16_t HALF          = BUF_HALFWORDS / 2; // 1000 x uint16_t pro Haelfte

  // Pro Sample: 2 x uint16_t (= 32 bit). Stereo: jedes zweite 32-bit-Wort ist leer (rechter Kanal).
  // -> 4 x uint16_t pro echtem Sample -> 1000 / 4 = 250 Samples pro Haelfte ("Why divide by 4?")
  constexpr uint16_t HALFWORDS_PER_SAMPLE = 4;

  // Kanal-Offset: SEL = GND -> Mikrofon sendet auf dem linken Kanal (zuerst im Frame).
  // Falls nur Nullen ankommen: auf 2 aendern (dann wird das andere 32-bit-Wort genommen).
  constexpr uint16_t CHANNEL_OFFSET = 0;

  uint16_t inputBuffer[BUF_HALFWORDS];

  volatile bool     halfReady = false;   // erste Haelfte bereit
  volatile bool     fullReady = false;   // zweite Haelfte bereit
  volatile uint32_t overruns  = 0;       // Haelfte ueberschrieben, bevor sie gelesen wurde

  uint8_t nextHalf = 0;                  // welche Haelfte als naechstes gelesen wird (0 oder 1)

  // Zwei aufeinanderfolgende 16-bit-Werte -> ein 18-bit-Sample
  inline int32_t mergeSample(uint16_t high, uint16_t low)
  {
    int32_t s = static_cast<int32_t>((static_cast<uint32_t>(high) << 16) | low);
    return s >> 14;   // untere 14 bit verwerfen, Vorzeichen (MSB) bleibt erhalten
  }

  // Auf den naechsten Callback warten, damit eine Aufnahme mit frischen Daten beginnt
  void syncToNextHalf()
  {
    halfReady = false;
    fullReady = false;
    overruns  = 0;
    while (!halfReady && !fullReady) { /* warten (max. 15,6 ms) */ }
    nextHalf = halfReady ? 0 : 1;
  }
}

/* ---------- DMA-Callbacks: kurz halten, nur Flags setzen, kein printf! ---------- */

extern "C" void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef* hi2s)
{
  if (hi2s->Instance != SPI2) return;
  if (halfReady) overruns++;   // alte Daten wurden noch nicht gelesen
  halfReady = true;
}

extern "C" void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef* hi2s)
{
  if (hi2s->Instance != SPI2) return;
  if (fullReady) overruns++;
  fullReady = true;
}

/* ---------- oeffentliche Funktionen ---------- */

void micStart(void)
{
  halfReady = false;
  fullReady = false;
  overruns  = 0;
  nextHalf  = 0;
  HAL_I2S_Receive_DMA(&hi2s2, inputBuffer, I2S_BUF_SIZE);
}

uint16_t micReadBlock(int32_t* out)
{
  const uint16_t* src;

  // Immer abwechselnd erste und zweite Haelfte lesen (siehe "Moegliche Fehlerquellen")
  if (nextHalf == 0)
  {
    if (!halfReady) return 0;
    halfReady = false;
    src = &inputBuffer[0];
    nextHalf = 1;
  }
  else
  {
    if (!fullReady) return 0;
    fullReady = false;
    src = &inputBuffer[HALF];
    nextHalf = 0;
  }

  uint16_t n = 0;
  for (uint16_t i = CHANNEL_OFFSET; i + 1 < HALF; i += HALFWORDS_PER_SAMPLE)
  {
    out[n++] = mergeSample(src[i], src[i + 1]);
  }
  return n;   // = MIC_BLOCK_SAMPLES (250)
}

uint32_t micRecord(int32_t* dest, uint32_t numSamples)
{
  int32_t  block[MIC_BLOCK_SAMPLES];
  uint32_t count = 0;

  syncToNextHalf();
  uint32_t start = HAL_GetTick();

  while (count < numSamples)
  {
    uint16_t n = micReadBlock(block);
    for (uint16_t i = 0; i < n && count < numSamples; i++)
    {
      dest[count++] = block[i];
    }
  }

  return HAL_GetTick() - start;   // erwartet: ca. 1000 ms fuer 16000 Samples
}

uint32_t micOverruns(void)
{
  return overruns;
}
