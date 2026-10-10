#pragma once
#include "base.hpp"
#include "chunk_loader/noize_loader.hpp"
#include <vector>
#include <memory>


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

		// Один слой буферный, его загружаем, но не отображаем
		uint32_t side_size = _visible_radius ;
		uint32_t buff_size = side_size * side_size * side_size;
		_chunks.reserve(buff_size);

		for (int ind = 0; ind < buff_size; ind++) {
			int64_t x = ind % side_size;
			int64_t y = (ind / side_size) % side_size;
			int64_t z = (ind / (side_size * side_size)) % side_size;
			_chunks.push_back(_loader->LoadChunk(x, y, z));
		}
	}
};