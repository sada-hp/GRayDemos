#include "chunk_atlass.hpp"
#include "iostream"

void ChunkAtlass::RecalculateAll(float c_x, float c_y, float c_z)
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
		int64_t offset = (_visible_radius / 2) * _chunk_size; // + (_chunk_side / 2);
		x = _center[0] + floor(c_x / _chunk_size) * _chunk_size - offset + x * _chunk_size;
		y = _center[1] + floor(c_y / _chunk_size) * _chunk_size - offset + y * _chunk_size;
		z = _center[2] + floor(c_z / _chunk_size) * _chunk_size - offset + z * _chunk_size;

		Chunk *c = ((WhiteNoiseLoader *)_loader.get())->LoadCustomFuncChunk(x, y, z, [](NoiseParams params)
																			{
				Voxel v = { 0 };
				// const int y_v = 16;
				// auto r_x =
				// 	abs(4.f - static_cast<float>(params.x % 8)) / 4.f;
				// auto r_y =
				// 	abs(16.f - static_cast<float>(params.y % 32)) / 16.f;
				auto val = static_cast<float>(params.out) / static_cast<float>(UINT32_MAX);
				if (std::sqrtf(params.y * params.y + params.z * params.z + params.x * params.x) <= 6360.f * 1e3)
					v.type = 1 ;
				else
					v.type = val >= .01f ? 0 : 1;
				// std:: cout << params.x << " "<< params.y << " "<< params.z << std::endl;
				// std:: cout << params.out << std::endl;
					
				// v.type = val >= (r_x * r_y) ? 0 : 1;
				// int64_t a = params.y % 16;
				// v.type = (a == 0 || a == 15) ? 1 : 0; 
				// v.type = (a == 0 || a == 15) ? 1 : 0; 
				return v; });

		c->dirty = true;
		_chunks.push_back(c);
	}
}
