#pragma once
#include "base.hpp"
#include "Factories/VkMeshFactory.hpp"

class ChunkMeshfactory {
private:
	int ChunkSideBytes = 4;
	int ChunkSide = 16;
	int ChunkSize = ChunkSide * ChunkSide * ChunkSide;
	float VoxelSideSize = 1.f;

public:
	float Size = 2.f;

	void FillChunk(Chunk*) const;
	std::shared_ptr<IMesh> ChunkToMesh(std::shared_ptr<RenderScope> Scope, Chunk*, double earth_radius) const;
};

