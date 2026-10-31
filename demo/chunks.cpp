#pragma once
#include "chunks.hpp"

void ChunkMeshfactory::FillChunk(Chunk *chunk) const
{
	chunk->VoxelArray = std::vector<Voxel>(ChunkSize);

	for (int i = 0; i < ChunkSize; i++)
	{
		int x = i & (ChunkSide - 1);
		int y = (i >> (ChunkSideBytes * 2));
		int z = (i >> ChunkSideBytes) & (ChunkSide - 1);

		if (x * y * z > 256 && x != 12)
		{
			chunk->VoxelArray[i].type = 0;
		}
		else if (x >= 2 && x <= 6 && (z < 6 - abs(x - 4) && z > 2 - abs(x - 4)))
		{
			chunk->VoxelArray[i].type = 0;
		}
		else
		{
			chunk->VoxelArray[i].type = 1;
		}
	}
}

enum Sides
{
	X = 0,
	Y = 1,
	Z = 2,
};
enum Dirs
{
	PosX = 0,
	NegX = 1,
	PosY = 2,
	NegY = 3,
	PosZ = 4,
	NegZ = 5,
};

const glm::ivec3 dirs[] = {
	{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

std::shared_ptr<IMesh> ChunkMeshfactory::ChunkToMesh(std::shared_ptr<RenderScope> Scope, Chunk *chunk, ChunkAtlass &atlass, glm::ivec3 pos, double earth_radius) const
{
	const glm::vec3 vertexPoints[][4] = {
		{{1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}},
		{{0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}},
		{{0, 1, 0}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}},
		{{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}},
		{{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}},
		{{0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}},
	};

	Chunk *neighbours[6];
	Chunk *n_chunk;
	for (int c = 0; c < 6; c++)
	{
		neighbours[c] = atlass.GetChunkInPoint(pos + dirs[c] * static_cast<int>(Size * ChunkSide));
	}

	auto vertices = std::vector<CompressedVertex>();
	auto indices = std::vector<uint16_t>();

	uint16_t indicesTmp[6] = {0};
	uint16_t vertexIndex = 0;
	int vertexOffset = 0;

	for (int voxel_it = 0; voxel_it < ChunkSize; voxel_it++)
	{
		int x = voxel_it & (ChunkSide - 1);
		int y = (voxel_it >> (ChunkSideBytes * 2));
		int z = (voxel_it >> ChunkSideBytes) & (ChunkSide - 1);
		Voxel *cur = &(chunk->VoxelArray[x + y * ChunkSide + z * ChunkSide * ChunkSide]);

		if (cur->type == 0)
			continue;

		for (int side_it = 0; side_it < 6; side_it++)
		{
			glm::ivec3 dir = dirs[side_it];
			glm::ivec3 dir_p = dir + glm::ivec3{x, y, z};

			// Соседний воксель
			Voxel vx = {1};

			const int sides[] = {Sides::X, Sides::X, Sides::Y, Sides::Y, Sides::Z, Sides::Z};
			int side = sides[side_it];

			n_chunk = chunk;
			if (dir_p[side] >= ChunkSide || dir_p[side] < 0)
			{
				dir_p[side] = dir_p[side] < 0 ? ChunkSide - 1 : 0;
				n_chunk = neighbours[side_it];
			}

			if (n_chunk){
				vx = (n_chunk->VoxelArray.data()[dir_p.x + dir_p.y * ChunkSide + dir_p.z * ChunkSide * ChunkSide]);
			}

			if (vx.type != 0)
				continue;

			for (int vert_it = 0; vert_it < 4; vert_it++)
			{
				CompressedVertex vertex{};
				vertex.position = Size * (glm::vec3{x, y, z} + vertexPoints[side_it][vert_it]);
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
