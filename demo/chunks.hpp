#pragma once
#include "Factories/VkMeshFactory.hpp"
#include "dummy.hpp"

class DemoOperators {
private:
	int ChunkSideBytes = 4;
	int ChunkSide = 16;
	int ChunkSize = ChunkSide * ChunkSide * ChunkSide;
	float VoxelSideSize = 1.f;

public:
	float Size = 10.f;

	void FillChunk(Chunk*) const;
	std::shared_ptr<IMesh> ChunkToMesh(std::shared_ptr<RenderScope> Scope, Chunk*) const;

	std::shared_ptr<ChunkAtlass> CreateExampleAtlass();
	void DrawVoxel(Voxel*);
};