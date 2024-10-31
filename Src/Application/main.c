#include "Application/main.h"

#define AUDIO_FILE_SIZE		(960512)	// File size in bytes
#define AUDIO_BLOCK_SIZE	(AUDIO_FILE_SIZE/4096)

ADPCMEncoderState state;

int16_t SoundData[2048] = {0};
int16_t EncodedData[512] = {0};
int16_t DecodedData[2048] = {0};

int main()
{
	W25Q_Init();
	LED_Init();
	UART2_Init();

	ADPCM_Init(&state);

	for(uint32_t i = 0; i <AUDIO_BLOCK_SIZE; i++)
	{
		// Read original data from Flash
		// Encode Audio
		// Write encoded audio to Flash
	}

	for(uint32_t i = 0; i <AUDIO_FILE_SIZE; i++)
	{
		// Read encoded audio from Flash
		// Decode Audio
		// Write decoded audio to Flash
	}

	while(1)
	{

	}
}

