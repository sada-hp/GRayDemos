#pragma once
#include "base.hpp"
#include "World/chunk_atlass.hpp"
#include "Allocators/gpu_block.hpp"

class ChunkMeshfactory {
public:
	static constexpr int ChunkSideBytes = CHUNK_ROW_BYTES;
	static constexpr int ChunkSide = CHUNK_ROW;
	static constexpr int ChunkSize = ChunkSide * ChunkSide * ChunkSide;
	static constexpr float Size = VOXEL_SIZE;

public:
	void FillChunk(Chunk*) const;
	std::shared_ptr<IMesh> ChunkToMesh(std::shared_ptr<RenderScope> Scope, std::shared_ptr<GGPuMemoryPool> vertexPool, std::shared_ptr<GGPuMemoryPool> indexPool, Chunk*, ChunkAtlass&, glm::ivec3 chunk_position) const;
};

