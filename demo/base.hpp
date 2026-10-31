#pragma once
#include "Engine/actor.hpp"
#include <vector>
#include <memory>

constexpr double VOXEL_SIZE = 32.0;
constexpr double  VOXEL_RADIUS = 22.62741699796952f;
constexpr int CHUNK_ROW_BYTES = 5;
constexpr int CHUNK_ROW = (1 << CHUNK_ROW_BYTES);
constexpr int CHUNK_TOWER = 3;
constexpr double CHUNK_SIZE = static_cast<double>(VOXEL_SIZE * CHUNK_ROW);
constexpr int CHUNK_SQARE = static_cast<int>(CHUNK_ROW * CHUNK_ROW);
constexpr int CHUNK_CUBE = static_cast<int>(CHUNK_ROW * CHUNK_ROW * CHUNK_ROW);

struct Voxel {
public:
	uint16_t type;
};

struct Chunk
{
public:
	std::shared_ptr<GActor> Mesh;
	std::vector<Voxel> VoxelArray;
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