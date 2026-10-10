#pragma once
#include "base.hpp"

/// <summary>
/// Загружает чанки из сторонних источников
/// </summary>
class IChunkLoader {
protected:
	uint32_t _chunkSide = 16;

public:
	//virtual ~IChunkLoader() = default;

	/// <summary>
	/// Загрузка чанка из внешней среды
	/// </summary>
	virtual Chunk* LoadChunk(int64_t x, int64_t y, int64_t z) = 0;
};