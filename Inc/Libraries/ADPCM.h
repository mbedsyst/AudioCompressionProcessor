#ifndef __ADPCM_H__
#define __ADPCM_H__

#include <stdint.h>

// ADPCM Encoder State
typedef struct
{
    int16_t predictedSample;    // Predicted sample
    int8_t  stepIndex;          // Step index
} ADPCMEncoderState;

void ADPCM_Init(ADPCMEncoderState *state);
void ADPCM_EncodeBlock(ADPCMEncoderState *state, int16_t *pcm_buffer, uint8_t *adpcm_buffer, int16_t sample_count);
void ADPCM_DecodeBlock(const uint8_t *compressedData, int16_t *decodedData, int dataSize, int *predictedSample, int *stepIndex);

#endif
