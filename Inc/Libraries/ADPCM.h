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
void ADPCM_EncodeBlock(ADPCMEncoderState *state, int16_t *audioData, int8_t *adpcmCode, int16_t sampleCount);
void ADPCM_DecodeBlock(ADPCMEncoderState *state, const uint8_t *adpcmCode, int16_t *decodedData, int sampleCount);

#endif
