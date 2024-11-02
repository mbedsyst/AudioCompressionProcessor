#include "Application/main.h"

#define AUDIO_FILE_SIZE		(960000)				// File size in bytes
#define AUDIO_BLOCK_SIZE	(AUDIO_FILE_SIZE/4096)

ADPCMEncoderState state;

int16_t SoundData[2048] = {0};
int8_t EncodedData[1024] = {0};
int16_t DecodedData[2048] = {0};

int main()
{
	W25Q_Init();
	LED_Init();
	UART2_Init();

	ADPCM_Init(&state);

	for(int32_t i = 0; i <AUDIO_BLOCK_SIZE; i++)
	{
		ReadAudioData(i, SoundData, 2048);
		ADPCM_EncodeBlock(&state, SoundData, EncodedData, 2048);
		WritePCMCode(i + 8192, EncodedData, 1024);
	}

	ADPCM_Init(&state);

	for(int32_t i = 0; i <AUDIO_BLOCK_SIZE; i++)
	{
		ReadPCMCode(i + 8192, EncodedData, 1024);
		ADPCM_DecodeBlock(&state, (uint8_t *)EncodedData, DecodedData, 2048);
		WriteAudioData(i + 16384, DecodedData, 2048);
	}

	while(1)
	{

	}
}









