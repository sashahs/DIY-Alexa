/*
 * rf.h
 *
 *  Created on: 24 Kas 2026
 *      Author: Tunahan
 */

#ifndef INC_RF_H_
#define INC_RF_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void transmit(uint32_t highPulses, uint32_t lowPulses);
void sendSequence(uint16_t bitSequence);


#ifdef __cplusplus
}

#endif




#endif /* INC_RF_H_ */
