#pragma once
#include "base.hpp"
#include "chunk_loader/noize_loader.hpp"
#include <vector>
#include <memory>

class ChunkAtlass
{

private:
	/// Размер чанка
	uint32_t _chunk_side;

	/// Радиус отображаемых чанков
	uint32_t _visible_radius;

	std::unique_ptr<IChunkLoader> _loader;

	/// @brief Global position for float smoothing
	int64_t _center[3] = {0, 0, 0};

	std::vector<Chunk *> _chunks = {};

public:
	ChunkAtlass(const size_t visible_radius = 2, const uint32_t chunk_side = 16)
	{
		_loader = std::make_unique<WhiteNoiseLoader>(0xDEADBEEF, chunk_side);

		_visible_radius = visible_radius;
		_chunk_side = chunk_side;
	};
	const std::vector<Chunk *> &GetChunks() { return _chunks; }
	void Recalculate(float c_x, float c_y, float c_z);
};