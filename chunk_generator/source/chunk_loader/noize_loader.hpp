#pragma once
#include "chunk_loader.hpp"
#include <functional>
#include "noise/MurmurHash3.h"

struct NoiseParams {
	/// <summary>
	/// Глобальное положение вокселя ро оси X
	/// </summary>
	int64_t x;

	/// <summary>
	/// Глобальное положение вокселя ро оси Y
	/// </summary>
	int64_t y;

	/// <summary>
	/// Глобальное положение вокселя ро оси Z
	/// </summary>
	int64_t z;

	/// <summary>
	/// Результат генерации
	/// </summary>
	uint32_t out;
};

class WhiteNoiseLoader : public IChunkLoader {
private:
	uint32_t _seed;
	uint32_t _chunkSide;

public:
	WhiteNoiseLoader(uint32_t seed, uint32_t chunkSide) :
		_chunkSide(chunkSide),
		_seed(seed) {
	};
	~WhiteNoiseLoader() {};

	/// <summary>
	/// Сгенерируем белый шум
	/// </summary>
	Chunk* LoadChunk(int64_t x, int64_t y, int64_t z) override {
		return LoadCustomFuncChunk(x, y, z, [](NoiseParams params) {
			Voxel v = { 0 };
			v.type = (params.out >> 24) < 0b1111 ? 1 : 0;
			return v;
		});
	};

	Chunk* LoadCustomFuncChunk(int64_t x, int64_t y, int64_t z, std::function<Voxel(NoiseParams)> func) {
		NoiseParams params = { x,y,z, _seed, };
		Chunk* c = new Chunk();

		c->x = x;
		c->y = y;
		c->z = z;
		c->VoxelArray.reserve(_chunkSide * _chunkSide * _chunkSide);

		for (int i = 0; i < _chunkSide * _chunkSide * _chunkSide; i++) {
			params.x = c->x * _chunkSide + (i & (_chunkSide - 1));
			params.z = c->z * _chunkSide + (i >> (4 * 2));
			params.y = c->y * _chunkSide + ((i >> 4) & (_chunkSide - 1));

			MurmurHash3_x86_32(&params, sizeof(params), _seed, &params.out);

			Voxel v = func(params);

			//if (params.x == 2 || (x > 2 && x < 6)) {
			//	int t = abs(y - 8);
			//	if (t < 2)
			//		v.type = 0;
			//	if (t < 4)
			//		v.type = 1;
			//}
			//else {
			//	v.type = (params.out >> 24) < 0b1111 ? 1 : 0;
			//}
			c->VoxelArray.push_back(v);
		}

		return c;
	};
};