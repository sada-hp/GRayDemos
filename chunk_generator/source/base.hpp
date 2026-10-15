#pragma once
#include <vector>
#include <memory>

struct Voxel {
public:
	uint32_t type;
};

struct Chunk
{
public:
	std::vector<Voxel> VoxelArray;
	int64_t x, y, z;
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