#pragma once
#include "chunks.hpp"

void ChunkMeshfactory::FillChunk(Chunk* chunk) const {
	chunk->VoxelArray = std::vector<Voxel>(ChunkSize);

	for (int i = 0; i < ChunkSize; i++) {
		int x = i & (ChunkSide - 1);
		int y = (i >> (ChunkSideBytes * 2));
		int z = (i >> ChunkSideBytes) & (ChunkSide - 1);

		if (x * y * z > 256 && x != 12) {
			chunk->VoxelArray[i].type = 0;
		}
		else if (x >= 2 && x <= 6 && (z < 6 - abs(x - 4) && z > 2 - abs(x - 4))) {
			chunk->VoxelArray[i].type = 0;
		}
		else {
			chunk->VoxelArray[i].type = 1;
		}
	}
}

std::shared_ptr<IMesh> ChunkMeshfactory::ChunkToMesh(std::shared_ptr<RenderScope> Scope, Chunk* chunk, double earth_radius) const
{
	const glm::ivec3 dirs[] = {
		{1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1}
	};
	const glm::dvec3 vertexPoints[][4] = {
		{{1,0,0}, {1,1,0}, /*{1,1,1},*/ {1,1,1}, {1,0,1}, /*{1,0,0},*/},
		{{0,0,0}, {0,0,1}, /*{0,1,1},*/ {0,1,1}, {0,1,0}, /*{0,0,0},*/},
		{{0,1,0}, {0,1,1}, /*{1,1,1},*/ {1,1,1}, {1,1,0}, /*{0,1,0},*/},
		{{0,0,0}, {1,0,0}, /*{1,0,1},*/ {1,0,1}, {0,0,1}, /*{0,0,0},*/},
		{{0,0,1}, {1,0,1}, /*{1,1,1},*/ {1,1,1}, {0,1,1}, /*{0,0,1},*/},
		{{0,0,0}, {0,1,0}, /*{1,1,0},*/ {1,1,0}, {1,0,0}, /*{0,0,0},*/},
	};

	auto vertices = std::vector<CompressedVertex>();
	auto indices = std::vector<uint16_t>();
	
	uint16_t indicesTmp[6] = {0};
	uint16_t vertexIndex = 0;
	int vertexOffset = 0;

	for (int chunk_it = 0; chunk_it < ChunkSize; chunk_it++) {
		int x = chunk_it & (ChunkSide - 1);
		int y = (chunk_it >> (ChunkSideBytes * 2));
		int z = (chunk_it >> ChunkSideBytes) & (ChunkSide - 1);
		Voxel* cur = &(chunk->VoxelArray[x + y * ChunkSide + z * ChunkSide * ChunkSide]);

		if (cur->type == 0) continue;
		for (int side_it = 0; side_it < 6; side_it++) {
			glm::ivec3 dir = dirs[side_it];
			glm::ivec3 dir_p = dir + glm::ivec3{x,y,z};

			Voxel vx = { 0 };
			/// TODO: Проверка соседних чанков
			if (dir_p.x >= ChunkSide || dir_p.x < 0) vx.type = 0;
			else if (dir_p.z >= ChunkSide || dir_p.z < 0) vx.type = 0;
			else if (dir_p.y >= ChunkSide || dir_p.y < 0) vx.type = 0;
			else {
				vx = (chunk->VoxelArray.data()[dir_p.x + dir_p.y * ChunkSide + dir_p.z * ChunkSide * ChunkSide]);
			}

			if (vx.type != 0) continue;

			for (int vert_it = 0; vert_it < 4; vert_it++) {
				CompressedVertex vertex{ };
				vertex.position = static_cast<double>(Size) * (glm::dvec3{x,y,z} + vertexPoints[side_it][vert_it]);
				vertex.normal = CompressedVertex::packNormal(dir);
				vertices.push_back(vertex);
			}

			indicesTmp[0] = vertexIndex + 0;
			indicesTmp[1] = vertexIndex + 1;
			indicesTmp[2] = vertexIndex + 2;
			indicesTmp[3] = vertexIndex + 2;
			indicesTmp[4] = vertexIndex + 3;
			indicesTmp[5] = vertexIndex + 0;
			indices.insert(indices.end(), std::begin(indicesTmp), std::end(indicesTmp));
			vertexIndex += 4;
		}
	}

	if (vertices.size() == 0)
		return nullptr;

	return std::shared_ptr<IMesh>(new GVkMesh<CompressedVertex>(Scope, vertices, indices));
}

