#pragma once
#include "base.hpp"
#include "chunk_loader/noize_loader.hpp"
#include <vector>
#include <memory>

class ChunkAtlass
{

private:
	/// количество вокселей на одной стороне
	uint32_t _chunk_side;

	/// Размер одной стороны
	uint32_t _chunk_size;

	/// Радиус отображаемых чанков
	uint32_t _visible_radius;

	std::unique_ptr<IChunkLoader> _loader;

	/// @brief Global position for float smoothing
	int64_t _center[3] = {0, 0, 0};

	std::vector<Chunk *> _chunks = {};

public:
	ChunkAtlass(const uint32_t visible_radius = 2, const uint32_t chunk_side = 16, uint32_t chunk_size = 32)
	{
		_loader = std::make_unique<WhiteNoiseLoader>(0xDEADBEEF, chunk_side);

		_visible_radius = visible_radius;
		_chunk_side = chunk_side;
		_chunk_size = chunk_size;
	};

	const std::vector<Chunk *> *GetChunks() { return &_chunks; }

	Chunk *GetChunkInPoint(float x, float y, float z)
	{
		for (auto c : _chunks)
		{
			bool b_x = x < c->x + _chunk_size && x >= c->x;
			bool b_y = y < c->y + _chunk_size && y >= c->y;
			bool b_z = z < c->z + _chunk_size && z >= c->z;
			if (b_x && b_y && b_z)
			{
				return c;
			}
		}
		return nullptr;
	}

	void RecalculateAll(float c_x, float c_y, float c_z);
};