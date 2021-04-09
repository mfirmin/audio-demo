#pragma once

#include <stdint.h>
#include <vector>

typedef struct WAV {
    std::vector<char> data;

    uint8_t channels;
    uint32_t sampleRate;
    uint8_t bitsPerSample;
} WAV;
