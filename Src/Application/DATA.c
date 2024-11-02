#include "Application/DATA.h"

void ReadAudioData(uint16_t page, int16_t *buffer, uint16_t size)
{
	uint16_t halfWordSize = 2 * size;
	uint8_t byteData[halfWordSize];
	W25Q_ReadData(page, 0, byteData, halfWordSize);
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
    W25Q_WriteData(page, 0, byteSize, byteData);
}

void ReadPCMCode(uint16_t page, int8_t *buffer, uint16_t size)
{
    uint8_t compressedData[size/2];
    W25Q_ReadData(page, 0, compressedData, size/2);
    for (int i = 0; i < size; i++)
    {
    	int8_t highNibble = (compressedData[i] >> 4) & 0x0F;
        int8_t lowNibble  = compressedData[i] & 0x0F;
        buffer[2 * i] = highNibble;
        buffer[2 * i + 1] = lowNibble;
    }
}

void WritePCMCode(uint16_t page, int8_t *buffer, uint16_t size)
{
	W25Q_WriteData(page, 0, size, (uint8_t *)buffer);
}
