#include "simplex_noise_loader.hpp"

Chunk SimplexNoiseLoader::LoadChunk(int64_t x, int64_t y, int64_t z)
{
    Chunk c = {};
    float scale = .02f;
    const int g_low = -0;
    const int g_high = 256;
    const int g_diff = abs(g_low - g_high);

    c.VoxelArray.resize(_chunkSide * _chunkSide * _chunkSide);
    glm::ivec3 v_pos = {};
    for (v_pos.x = 0; v_pos.x < _chunkSide; v_pos.x++)
    {
        for (v_pos.z = 0; v_pos.z < _chunkSide; v_pos.z++)
        {
            float noise = 
                _noise.fractal(3, (x / VOXEL_SIZE + v_pos.x), (z / VOXEL_SIZE + v_pos.z));
                // _noise.noise((x / VOXEL_SIZE + v_pos.x) * scale, (z / VOXEL_SIZE + v_pos.z) * scale);

            int height = noise * g_diff;

            for (v_pos.y = 0; v_pos.y < _chunkSide; v_pos.y++)
            {
                Voxel v = {0};

                int vh = y + v_pos.y * VOXEL_SIZE;

                if (vh < g_low)
                {
                    v.type = 1;
                }
                else if (vh > g_high)
                {
                    v.type = 0;
                }
                else
                {
                    v.type = vh <= height ? 1 : 0;
                }
                c.VoxelArray[v_pos.x + v_pos.y * _chunkSide + v_pos.z * _chunkSide * _chunkSide] = v;
            }
        }
    }

    return c;
}