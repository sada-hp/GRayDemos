#include "iostream"
#include "noize_loader.hpp"

Chunk *WhiteNoiseLoader::LoadChunk(int64_t x, int64_t y, int64_t z)
{
	return LoadCustomFuncChunk(x, y, z, [](NoiseParams params)
							   {
			Voxel v = { 0 };
			v.type = (params.out >> 24) < 0b1111 ? 1 : 0;
			return v; });
}

Chunk *WhiteNoiseLoader::LoadCustomFuncChunk(int64_t x, int64_t y, int64_t z, std::function<Voxel(NoiseParams)> func)
{
	NoiseParams params = {
		x,
		y,
		z,
		_seed,
	};
	Chunk *c = new Chunk();

	// c->x = x;
	// c->y = y;
	// c->z = z;
	c->VoxelArray.reserve(_chunkSide * _chunkSide * _chunkSide);
	// std:: cout << params.x << " "<< params.y << " "<< params.z <<  " " <<  _seed << std::endl;

	for (uint32_t i = 0; i < _chunkSide * _chunkSide * _chunkSide; i++)
	{
		params.x = x + (i & (_chunkSide - 1));
		params.y = y + ((i >> 4) & (_chunkSide - 1));
		params.z = z + (i >> (4 * 2));

		MurmurHash3_x86_32(&params, sizeof(params), _seed, &params.out);

		Voxel v = func(params);
		c->VoxelArray.push_back(v);
	}

	return c;
};