#pragma once
#include <glm/gtx/hash.hpp>
#include "base.hpp"
#include "chunk_loader/noize_loader.hpp"
#include "actor.hpp"
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

	int64_t _center[3] = {0, 0, 0};

	std::vector<GActor *> _chunkMeshes = {};
	// std::vector<Chunk *> _chunks = {};
	std::unordered_map<glm::ivec3, Chunk *> _chunks = {};

public:
	ChunkAtlass(const uint32_t visible_radius = 2, const uint32_t chunk_side = 16, uint32_t chunk_size = 32)
	{
		_loader = std::make_unique<WhiteNoiseLoader>(0xDEADBEEF, chunk_side);

		_visible_radius = visible_radius;
		_chunk_side = chunk_side;
		_chunk_size = chunk_size;
	};

	const std::unordered_map<glm::ivec3, Chunk *> *GetChunks() { return &_chunks; }

	Chunk *GetChunkInPoint(glm::dvec3 pos)
	{
		pos = floor(pos / static_cast<double>(_chunk_size)) * static_cast<double>(_chunk_size);
		auto ch = _chunks.find(pos);
		if (ch != _chunks.end())
			return ch->second;
		return nullptr;
	}

	void RecalculateAll(float c_x, float c_y, float c_z);
	void RecalculateDelta(float c_x, float c_y, float c_z);
};