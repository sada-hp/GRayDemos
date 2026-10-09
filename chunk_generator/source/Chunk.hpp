#pragma once
#include <vector>
#include <memory>
#include "noise/MurmurHash3.h"

struct Voxel {
public:
	uint32_t type;
};

struct Chunk
{
public:
	std::vector<Voxel> VoxelArray;
};

class ChunkMethods {
private:
	uint16_t ChunkSide = 16;
public:
	inline Voxel* GetVoxel(Chunk* chunk, uint32_t x, uint32_t y, uint32_t z) {
		return GetVoxel(chunk, x, y, z, ChunkSide);
	}
	Voxel* GetVoxel(Chunk* chunk, uint32_t x, uint32_t y, uint32_t z, uint32_t side) {
		if (!chunk) return nullptr;
		return &chunk->VoxelArray[0];
	}
};

class ChunkGenerator {
private:
	uint64_t _seed;
	uint32_t _chunkSide = 16;
public:
	ChunkGenerator(uint64_t seed) {
		_seed = seed;
	}

	Chunk* GetChunk(uint32_t x, uint32_t y, uint32_t z) {
		uint32_t out = _seed;
		Chunk* c = new Chunk();
		c->VoxelArray.reserve(_chunkSide * _chunkSide * _chunkSide);

		for (int i = 0; i < _chunkSide * _chunkSide * _chunkSide; i++) {
			MurmurHash3_x86_32(&out, _seed, 1, &out);
			Voxel v = Voxel();
			v.type = out & 0x1;
			c->VoxelArray.push_back(v);
		}

		return c;
	}
};