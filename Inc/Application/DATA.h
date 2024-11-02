#ifndef __DATA_H__
#define __DATA_H__

#include <stdint.h>
#include "Libraries/W25Qxx.h"

// Functions to Handle 16-bit Audio samples
void ReadAudioData(uint16_t page, int16_t *buffer, uint16_t size);
void WriteAudioData(uint16_t page, int16_t *buffer, uint16_t size);
// Functions to Handle 4-bit ADPCM codes
void ReadPCMCode(uint16_t page, int8_t *buffer, uint16_t size);
void WritePCMCode(uint16_t page, int8_t *buffer, uint16_t size);

#endif
