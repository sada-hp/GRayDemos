#include "chunks.hpp"
#include <iostream>

void DemoOperators::FillChunk(std::shared_ptr<Chunk> chunk) const {
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

std::shared_ptr<IMesh> DemoOperators::ChunkToMesh(std::shared_ptr<RenderScope> Scope, std::shared_ptr<Chunk> chunk) const
{
	const glm::vec3 dirs[] = {
		{1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}, {0,0,-1}
	};
	const glm::vec3 vertexPoints[] = {
		{1,0,0}, {1,1,0}, {1,1,1}, {1,1,1}, {1,0,1}, {1,0,0},
		{0,0,0}, {0,0,1}, {0,1,1}, {0,1,1}, {0,1,0}, {0,0,0},
		{0,1,0}, {0,1,1}, {1,1,1}, {1,1,1}, {1,1,0}, {0,1,0},
		{0,0,0}, {1,0,0}, {1,0,1}, {1,0,1}, {0,0,1}, {0,0,0},
		{0,0,1}, {1,0,1}, {1,1,1}, {1,1,1}, {0,1,1}, {0,0,1},
		{0,0,0}, {0,1,0}, {1,1,0}, {1,1,0}, {1,0,0}, {0,0,0},
	};

	auto vertices = std::vector<MeshVertex>();
	int vertexOffset = 0;


	for (int chunk_it = 0; chunk_it < ChunkSize; chunk_it++) {
		int x = chunk_it & (ChunkSide - 1);
		int y = (chunk_it >> (ChunkSideBytes * 2));
		int z = (chunk_it >> ChunkSideBytes) & (ChunkSide - 1);
		Voxel* cur = &(chunk->VoxelArray[x + y * ChunkSide + z * ChunkSide * ChunkSide]);

		if (cur->type == 0) continue;
		for (int side_it = 0; side_it < 6; side_it++) {
			glm::vec3 dir = dirs[side_it];
			int _x = x + dir.x;
			int _z = z + dir.z;
			int _y = y + dir.y;

			Voxel ch = { 0 };
			/// TODO: Проверка соседних чанков
			if (_x >= ChunkSide || _x < 0) ch.type = 0;
			else if (_z >= ChunkSide || _z < 0) ch.type = 0;
			else if (_y >= ChunkSide || _y < 0) ch.type = 0;
			else {
				ch = (chunk->VoxelArray[_x + _y * ChunkSide + _z * ChunkSide * ChunkSide]);
			}

			if (ch.type != 0) continue;

			for (int vert_it = 0; vert_it < 6; vert_it++) {
				MeshVertex vertex { };
				vertex.position = {
					x + vertexPoints[side_it * 6 + vert_it].x,
					y + vertexPoints[side_it * 6 + vert_it].y,
					z + vertexPoints[side_it * 6 + vert_it].z
				};
				vertex.normal = dir;
				vertices.push_back(vertex);
			}
		}
	}

	return std::shared_ptr<IMesh>(new GVkMesh<MeshVertex>(Scope, vertices));
}

