#include "chunk_atlass.hpp"
#include "iostream"

void ChunkAtlass::RecalculateAll(float c_x, float c_y, float c_z)
{
	_chunks.clear();

	uint32_t side_size = 1 + _visible_radius * 2;
	uint32_t buff_size = side_size * side_size * side_size;
	_chunks.reserve(buff_size);

	_center[0] = floor(c_x / _chunk_size) * _chunk_size;
	_center[1] = floor(c_y / _chunk_size) * _chunk_size;
	_center[2] = floor(c_z / _chunk_size) * _chunk_size;

	for (int ind = 0; ind < buff_size; ind++)
	{
		int64_t x = ind % side_size;
		int64_t z = (ind / side_size) % side_size;
		int64_t y = (ind / (side_size * side_size)) % side_size;

		int64_t offset = _visible_radius * _chunk_size; // + (_chunk_side / 2);
		x = _center[0] - offset + x * _chunk_size;
		y = _center[1] - offset + (y * _chunk_size);
		z = _center[2] - offset + z * _chunk_size;

		Chunk c = _loader->LoadChunk(x, y - 6360.f * 1e3, z);

		c.dirty = true;
		c.pool_index = ind;
		_chunks[glm::ivec3{x, y, z}] = c;
		// _chunks.push_back(c);
	}
}

void ChunkAtlass::RecalculateDelta(glm::dvec3 pos)
{
	glm::ivec3 new_ = ChunkGlobalId(pos);

	// delta
	glm::ivec3 d_ = glm::ivec3{
		(new_.x - _center.x) / _chunk_size,
		(new_.y - _center.y) / _chunk_size,
		(new_.z - _center.z) / _chunk_size,
	};

	glm::ivec2 h[] = {
		d_.x > 0
			? glm::ivec2{-_visible_radius + d_.x, _visible_radius}
			: glm::ivec2{-_visible_radius, _visible_radius + d_.x},
		d_.y > 0
			? glm::ivec2{-_visible_radius + d_.y, _visible_radius}
			: glm::ivec2{-_visible_radius, _visible_radius + d_.y},
		d_.z > 0
			? glm::ivec2{-_visible_radius + d_.z, _visible_radius}
			: glm::ivec2{-_visible_radius, _visible_radius + d_.z},
	};

	for (int i = -_visible_radius; i <= _visible_radius; i++)
	{
		for (int j = -_visible_radius; j <= _visible_radius; j++)
		{
			for (int k = -_visible_radius; k <= _visible_radius; k++)
			{
				if (
					i < h[0].x || i > h[0].y ||
					j < h[1].x || j > h[1].y ||
					k < h[2].x || k > h[2].y)
				{
					glm::ivec3 c_to_d = glm::ivec3{i * _chunk_size, j * _chunk_size, k * _chunk_size};
					auto ch = _chunks.find(_center + c_to_d);
					if (ch != _chunks.end())
					{
						// Куда поставить новый чанк
						glm::ivec3 c_to_c = new_ + c_to_d * -1;

						Chunk c = _loader->LoadChunk(c_to_c.x, c_to_c.y - 6360.f * 1e3, c_to_c.z);

						// инициализируем новый чанк
						c.pool_index = ch->second.pool_index;
						c.dirty = true;

						// освобождаем старый чанк
						_chunks.erase(ch);

						_chunks[c_to_c] = c;

						// Перерисуем чанки на старой границе
						ch = _chunks.find(c_to_c - d_ * _chunk_size);
						if (ch != _chunks.end())
						{
							ch->second.dirty = true;
						}
					}
				}
			}
		}
	}

	_center = new_;
}
