#pragma once
#include "chunk_loader.hpp"
#include "noise/SimplexNoise.h"

class SimplexNoiseLoader : public IChunkLoader
{
private:
    int _chunkSide;
    SimplexNoise _noise;

public:
    SimplexNoiseLoader(int chunkSide) : _chunkSide(chunkSide)
    {
        _noise = SimplexNoise(.00003f);
    };
    ~SimplexNoiseLoader() {};

    /// <summary>
    /// Сгенерируем белый шум
    /// </summary>
    Chunk LoadChunk(int64_t x, int64_t y, int64_t z) override;
};