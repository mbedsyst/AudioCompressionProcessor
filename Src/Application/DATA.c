#include "Application/DATA.h"


void ReadAudioData(uint16_t page, int16_t *buffer, uint16_t size)
{
	uint16_t halfWordSize = 2 * size;
	uint8_t byteData[halfWordSize];
	W25Q_ReadData(page, offset, byteData, halfWordSize);
	for(int i = 0; i < size; i++)
	{
		buffer[i] = (int16_t)(byteData[2 * i] | (byteData[2 * i + 1] << 8));
	}
}

void WriteAudioData(uint16_t page, int16_t *buffer, uint16_t size)
{
    uint32_t byteSize = 2 * size;
    uint8_t byteData[byteSize];
    for (size_t i = 0; i < size; i++)
    {
        byteData[2 * i] = (uint8_t)(buffer[i] & 0xFF);
        byteData[2 * i + 1] = (uint8_t)((buffer[i] >> 8) & 0xFF);
    }
    W25Q_WriteData(startPage, offset, byteSize, byteData);
}

vod ReadPCMCode(uint16_t page, uint8_t *buffer, uint16_t size)
{

}

vod WritePCMCode(uint16_t page, uint8_t *buffer, uint16_t size)
{

}
