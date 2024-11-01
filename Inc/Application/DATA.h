#ifndef __DATA_H__
#define __DATA_H__

#include <stdint.h>
#include "Libraries/W25Qxx.h"

void ReadAudioData(uint16_t page, int16_t *buffer, uint16_t size);
void WriteAudioData(uint16_t page, int16_t *buffer, uint16_t size);

#endif
