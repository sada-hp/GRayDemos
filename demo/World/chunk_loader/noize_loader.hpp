#pragma once
#include "chunk_loader.hpp"
#include <functional>
#include "noise/MurmurHash3.h"

struct NoiseParams {
	/// <summary>
	/// Глобальное положение вокселя ро оси X
	/// </summary>
	int64_t x;

	/// <summary>
	/// Глобальное положение вокселя ро оси Y
	/// </summary>
	int64_t y;

	/// <summary>
	/// Глобальное положение вокселя ро оси Z
	/// </summary>
	int64_t z;

	/// <summary>
	/// Результат генерации
	/// </summary>
	uint32_t out;
};

class WhiteNoiseLoader : public IChunkLoader {
private:
	uint32_t _seed;
	uint32_t _chunkSide;

public:
	WhiteNoiseLoader(uint32_t seed, uint32_t chunkSide) :
		_chunkSide(chunkSide),
		_seed(seed) {
	};
	~WhiteNoiseLoader() {};

	/// <summary>
	/// Сгенерируем белый шум
	/// </summary>
	Chunk* LoadChunk(int64_t x, int64_t y, int64_t z) override;

	Chunk* LoadCustomFuncChunk(int64_t x, int64_t y, int64_t z, std::function<Voxel(NoiseParams)> func) ;
};