#pragma once
#include "base.hpp"
#include "Factories/VkMeshFactory.hpp"

class ChunkMeshfactory {
private:
	int ChunkSideBytes = CHUNK_ROW_BYTES;
	int ChunkSide = CHUNK_ROW;
	int ChunkSize = ChunkSide * ChunkSide * ChunkSide;
	float Size = VOXEL_SIZE;

public:

	void FillChunk(Chunk*) const;
	std::shared_ptr<IMesh> ChunkToMesh(std::shared_ptr<RenderScope> Scope, Chunk*, double earth_radius) const;
};

