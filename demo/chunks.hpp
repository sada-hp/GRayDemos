#pragma once
#include "Factories/VkMeshFactory.hpp"

class Voxel {
public:
	int type;
};

class Chunk {
public:
	std::vector<Voxel> VoxelArray;
};

class ChunkAtlass {
public:
	std::vector<Chunk> ChunkArray;
};

class DemoOperators {
private:
	int ChunkSideBytes = 4;
	int ChunkSide = 16;
	int ChunkSize = ChunkSide * ChunkSide * ChunkSide;
	float VoxelSideSize = 1.f;

public:
	void FillChunk(std::shared_ptr<Chunk>) const;
	std::shared_ptr<IMesh> ChunkToMesh(std::shared_ptr<RenderScope> Scope, std::shared_ptr<Chunk>) const;

	std::shared_ptr<ChunkAtlass> CreateExampleAtlass();
	void DrawVoxel(Voxel*);
};