#include "Libraries/ADPCM.h"

// Define constants for ADPCM
static int indexTable[16] =
{
	-1, -1, -1, -1, 2, 4, 6, 8,
	-1, -1, -1, -1, 2, 4, 6, 8
};

static int stepTable[89] =
{
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31,
    34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143,
    157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544,
    598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845,
    8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

// Initialize ADPCM state variable
void ADPCM_Init(ADPCMEncoderState *state)
{
	state->predictedSample = 0;
	state->stepIndex = 0;
}

// ADPCM Compression function for single audio sample
static int8_t ADPCM_EncodeSample(ADPCMEncoderState *state, int16_t sample)
{
    int diff = sample - state->predictedSample;
    int step = stepTable[state->stepIndex];
    int code = 0;

    // Quantize the difference
    if (diff < 0)
    {
        code = 8;
        diff = -diff;
    }

    if (diff >= step) 			{ code |= 4; diff -= step; }
    if (diff >= (step >> 1)) 	{ code |= 2; diff -= (step >> 1); }
    if (diff >= (step >> 2)) 	{ code |= 1; }

    // Update the predicted sample
    int delta = step >> 3;
    if (code & 4) 				delta += step;
    if (code & 2) 				delta += step >> 1;
    if (code & 1) 				delta += step >> 2;

    if (code & 8)
    {
        state->predictedSample -= delta;
    }
    else
    {
        state->predictedSample += delta;
    }

    // Clamp predicted sample to 16-bit signed range
    if (state->predictedSample > 32767) 		state->predictedSample = 32767;
    else if (state->predictedSample < -32768) 	state->predictedSample = -32768;

    // Update the step index
    state->stepIndex += indexTable[code];
    if (state->stepIndex < 0) 			state->stepIndex = 0;
    else if (state->stepIndex > 88) 	state->stepIndex = 88;

    return (int8_t)(code & 0x0F);  // Return 4-bit compressed code
}

void ADPCM_EncodeBlock(ADPCMEncoderState *state, int16_t *audioData, int8_t *adpcmCode, int16_t sampleCount)
{
    for (int16_t i = 0; i < sampleCount; i += 2)
    {
        int8_t adpcmSample1 = ADPCM_EncodeSample(state, audioData[i]);
        int8_t adpcmSample2 = ADPCM_EncodeSample(state, audioData[i + 1]);
        adpcmCode[i / 2] = (adpcmSample1 << 4) | (adpcmSample2 & 0x0F);
    }
}

static int16_t ADPCM_DecodeSample(ADPCMEncoderState *state, uint8_t code)
{
    int step = stepTable[state->stepIndex];
    int delta = step >> 3;

    if (code & 4) delta += step;
    if (code & 2) delta += (step >> 1);
    if (code & 1) delta += (step >> 2);

    if (code & 8) state->predictedSample -= delta;
    else          state->predictedSample += delta;

    if (state->predictedSample > 32767)      state->predictedSample = 32767;
    else if (state->predictedSample < -32768) state->predictedSample = -32768;

    state->stepIndex += indexTable[code];
    if (state->stepIndex < 0) state->stepIndex = 0;
    else if (state->stepIndex > 88) state->stepIndex = 88;

    return (int16_t)state->predictedSample;
}

void ADPCM_DecodeBlock(ADPCMEncoderState *state, const uint8_t *adpcmCode, int16_t *decodedData, int sampleCount)
{
	for (int i = 0; i < sampleCount; i++)
	{
		decodedData[i] = ADPCM_DecodeSample(adpcmCode[i], state);
	}
}










