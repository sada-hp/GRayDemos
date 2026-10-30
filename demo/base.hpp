#pragma once
#include "Engine/actor.hpp"
#include <vector>
#include <memory>

#define VOXEL_SIZE 32.0
#define VOXEL_RADIUS 22.62741699796952f
#define CHUNK_ROW_BYTES 4
#define CHUNK_ROW (1 << CHUNK_ROW_BYTES)
#define CHUNK_TOWER 3
#define CHUNK_SIZE static_cast<double>(VOXEL_SIZE * CHUNK_ROW)
#define CHUNK_SQARE static_cast<int>(CHUNK_ROW * CHUNK_ROW)
#define CHUNK_CUBE static_cast<int>(CHUNK_ROW * CHUNK_ROW * CHUNK_ROW)

struct Voxel {
public:
	uint32_t type;
};

struct Chunk
{
public:
	std::shared_ptr<GActor> Mesh;
	std::vector<Voxel> VoxelArray;
	// int64_t x, y, z;
	bool dirty;
	~Chunk(){
		VoxelArray.clear();
	}
};


struct PlanetChunk
{
public:
	std::vector<Voxel> VoxelArray;
	// double 
	uint32_t Layer = 0;
	uint32_t Height = 0;
	uint32_t Number = 0;


	~PlanetChunk(){
		VoxelArray.clear();
	}
};