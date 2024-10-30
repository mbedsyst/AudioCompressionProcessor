#include "Application/main.h"

#define AUDIO_FILE_SIZE		(960512)	// File size in bytes
#define AUDIO_BLOCK_SIZE	(AUDIO_FILE_SIZE/W25Q_PageSize)

int main()
{
	W25Q_Init();
	ADPCM_Init();

	for(uint32_t i = 0; i <AUDIO_FILE_SIZE; i++)
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

