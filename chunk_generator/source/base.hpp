#pragma once
#include <vector>

struct Voxel {
public:
	uint32_t type;
};

struct Chunk
{
public:
	std::vector<Voxel> VoxelArray;
	int64_t x, y, z;
};

class ChunkMethods {
	static Voxel* GetVoxel(Chunk* chunk, uint32_t x, uint32_t y, uint32_t z, uint32_t side) {
		if (!chunk) return nullptr;
		return &chunk->VoxelArray[0];
	}
};