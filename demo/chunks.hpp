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
};

class ChunkAtlass {
private:
	/// <summary>
	/// Размер чанка
	/// </summary>
	uint32_t _chunk_side;

	/// <summary>
	/// Радиус отображаемых чанков
	/// </summary>
	uint32_t _visible_radius;

	std::unique_ptr<IChunkLoader> _loader;

	int64_t _center[2] = { 0, 0 };
	std::vector<Chunk*> _chunks = {};

public:
	const std::vector<Chunk*>& GetChunks() { return _chunks; }

	ChunkAtlass(const size_t visible_radius = 2, const uint32_t chunk_side = 16) {
		_loader = std::make_unique<WhiteNoiseLoader>(0xDEADBEEF, chunk_side);

		_visible_radius = visible_radius;
		_chunk_side = chunk_side;

		Recalculate();
	};

	void Recalculate() {
		for (auto chunk : _chunks) {
			delete chunk;
		}

		uint32_t side_size = _visible_radius;
		uint32_t buff_size = side_size * side_size * side_size;
		_chunks.reserve(buff_size);
		for (int ind = 0; ind < buff_size; ind++) {
			int64_t x = ind % side_size;
			int64_t y = (ind / side_size) % side_size;
			int64_t z = (ind / (side_size * side_size)) % side_size;

			_chunks.push_back(((WhiteNoiseLoader*)_loader.get())->LoadCustomFuncChunk(x, y, z, [](NoiseParams params) {
				Voxel v = { 0 };
				const int y_v = 16;
				auto r_x =
					abs(32.f - static_cast<float>(params.x % 64)) / 32.f;
				auto r_y =
					abs(16.f - static_cast<float>(params.y % 32)) / 16.f;
				auto val = static_cast<float>(params.out) / static_cast<float>(UINT32_MAX);
					
				v.type = val >= (r_x * r_y) ? 0 : 1;
				return v;
				}));
		}
	}
};