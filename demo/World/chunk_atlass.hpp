#pragma once
#include "chunk_loader/noize_loader.hpp"
#include <glm/gtx/hash.hpp>
#include "Engine/actor.hpp"
#include "base.hpp"
#include <vector>
#include <memory>

class ChunkAtlass
{
private:
	/// количество вокселей на одной стороне
	int32_t _chunk_side;

	/// Размер одной стороны
	int32_t _chunk_size;

	/// Радиус отображаемых чанков
	int32_t _visible_radius;

	std::unique_ptr<IChunkLoader> _loader;

	glm::ivec3 _center = glm::ivec3{0};

	std::vector<GActor *> _chunkMeshes = {};
	// std::vector<Chunk *> _chunks = {};
	std::unordered_map<glm::ivec3, Chunk *> _chunks = {};

public:
	ChunkAtlass(const int32_t visible_radius = 2, const int32_t chunk_side = 16, int32_t chunk_size = 32)
	{
		_loader = std::make_unique<WhiteNoiseLoader>(0xDEADBEEF, chunk_side);

		_visible_radius = visible_radius;
		_chunk_side = chunk_side;
		_chunk_size = chunk_size;
	};

	const std::unordered_map<glm::ivec3, Chunk *> *GetChunks() const { return &_chunks; }

	Chunk *GetChunkInPoint(glm::dvec3 pos)
	{
		pos = floor(pos / static_cast<double>(_chunk_size)) * static_cast<double>(_chunk_size);
		auto ch = _chunks.find(pos);
		if (ch != _chunks.end())
			return ch->second;
		return nullptr;
	}

	void RecalculateAll(float c_x, float c_y, float c_z);
	void RecalculateDelta(glm::dvec3 pos);

	static glm::ivec3 ChunkGlobalId(const glm::dvec3 &pos)
	{
		int x = static_cast<int>(floor(pos.x / (CHUNK_SIZE)) * (CHUNK_SIZE));
		int y = static_cast<int>(floor(pos.y / (CHUNK_SIZE)) * (CHUNK_SIZE));
		int z = static_cast<int>(floor(pos.z / (CHUNK_SIZE)) * (CHUNK_SIZE));
		return glm::ivec3{x, y, z};
	}
};
