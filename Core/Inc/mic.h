/*
 * mic.h
 *
 * Mikrofon SPH0645LM4H-B ueber I2S2 + DMA (Aufgabe 1.5)
 *
 * Pins: PB10 = I2S2_CK (BCLK), PB12 = I2S2_WS (LRCL), PC3 = I2S2_SD (DOUT), SEL -> GND
 * Abtastrate: 16 kHz, ein Sample = 18 bit (vorzeichenbehaftet), gespeichert als int32_t
 */

#ifndef MIC_H_
#define MIC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIC_SAMPLE_RATE     16000u   // Samples pro Sekunde
#define MIC_BLOCK_SAMPLES   250u     // Samples pro DMA-Haelfte (= 15,6 ms Audio)

void micStart(void); // DMA starten (einmal nach den MX_..._Init Aufrufen)

uint16_t micReadBlock(int32_t* out); // neuer Block da? -> 250 Samples nach out kopieren, Rueckgabe 250
									// noch kein neuer Block -> Rueckgabe 0

uint32_t micRecord(int32_t* dest, uint32_t numSamples); // blockierend numSamples aufnehmen,
                                                        // Rueckgabe: benoetigte Zeit in ms (Aufgabe 1.7: HAL_GetTick)

uint32_t micOverruns(void); // wie oft Daten verloren gingen (Verarbeitung zu langsam)

#ifdef __cplusplus
}
#endif

#endif /* MIC_H_ */
