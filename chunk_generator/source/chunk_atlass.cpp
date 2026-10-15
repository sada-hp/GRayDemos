#include "chunk_atlass.hpp"

// void SetChunkRightPoint()

void ChunkAtlass::Recalculate(float c_x, float c_y, float c_z)
{
	for (auto chunk : _chunks)
	{
		delete chunk;
	}
	_chunks.clear();

	uint32_t side_size = _visible_radius;
	uint32_t buff_size = side_size * side_size * 3;
	_chunks.reserve(buff_size);
	for (int ind = 0; ind < buff_size; ind++)
	{
		int64_t x = ind % side_size;
		int64_t z = (ind / side_size) % side_size;
		int64_t y = (ind / (side_size * side_size)) % 3;
		int64_t offset = (_visible_radius / 2) * _chunk_side; // + (_chunk_side / 2);
		x = _center[0] + floor(c_x / 16) * 16 - offset + x * _chunk_side;
		y = _center[1] + floor(c_y / 16) * 16 - offset + y * _chunk_side;
		z = _center[2] + floor(c_z / 16) * 16 - offset + z * _chunk_side;

		Chunk *c = ((WhiteNoiseLoader *)_loader.get())->LoadCustomFuncChunk(x, y, z, [](NoiseParams params)
																			{
				Voxel v = { 0 };
				const int y_v = 16;
				auto r_x =
					abs(4.f - static_cast<float>(params.x % 8)) / 4.f;
				auto r_y =
					abs(16.f - static_cast<float>(params.y % 32)) / 16.f;
				auto val = static_cast<float>(params.out) / static_cast<float>(UINT32_MAX);
					
				v.type = val >= (r_x * r_y) ? 0 : 1;
				// int64_t a = params.y % 16;
				// v.type = (a == 0 || a == 15) ? 1 : 0; 
				return v; });
		// c->Atlas = this;
		_chunks.push_back(c);
	}
}
