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

// Initialize ADPCM state
void ADPCM_Init(ADPCMEncoderState *state)
{
    state->predictedSample = 0;
    state->stepIndex = 0;
}

// ADPCM Compression function
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

void ADPCM_EncodeBlock(ADPCMEncoderState *state, int16_t *pcm_buffer, uint8_t *adpcm_buffer, int16_t sample_count)
{
    for (int16_t i = 0; i < sample_count; i += 2)
    {
        // Compress two samples at a time and pack into one byte
        int8_t adpcm_sample1 = ADPCM_EncodeSample(state, pcm_buffer[i]);
        int8_t adpcm_sample2 = ADPCM_EncodeSample(state, pcm_buffer[i + 1]);

        // Pack two 4-bit samples into one byte
        adpcm_buffer[i / 2] = (adpcm_sample1 << 4) | (adpcm_sample2 & 0x0F);
    }
}

static int16_t ADPCM_DecodeSample(uint8_t code, int *predictedSample, int *stepIndex)
{

    int step = stepTable[*stepIndex];  // Get current step size
    int delta = step >> 3;             // Start with 1/8 of step

    // Adjust delta based on the code
    if (code & 4) 		delta += step;
    if (code & 2) 		delta += (step >> 1);
    if (code & 1) 		delta += (step >> 2);

    // Apply the delta to the predicted sample
    if (code & 8) 		*predictedSample -= delta;  // Sign bit: 1 = negative
    else 				*predictedSample += delta;           // Sign bit: 0 = positive

    // Clamp the predicted sample to 16-bit signed range
    if (*predictedSample > 32767) 			*predictedSample = 32767;
    else if (*predictedSample < -32768) 	*predictedSample = -32768;

    // Update step index with the indexTable based on code
    *stepIndex += indexTable[code];
    if (*stepIndex < 0) 			*stepIndex = 0;
    else if (*stepIndex > 88) 		*stepIndex = 88;

    return (int16_t)*predictedSample;
}

void ADPCM_DecodeBlock(const uint8_t *compressedData, int16_t *decodedData, int dataSize, int *predictedSample, int *stepIndex)
{
    for (int i = 0; i < dataSize; i++)
    {
        decodedData[i] = ADPCM_DecodeSample(compressedData[i], predictedSample, stepIndex);
    }
}
