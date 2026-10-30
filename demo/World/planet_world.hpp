#pragma once
#include "Engine/camera.hpp"
#include "Engine/world.hpp"
#include <entt/entt.hpp>

#include "Materials/mesh_material.hpp"
#include "chunks.hpp"
#include "chunk_atlass.hpp"

class GPlanetWorld : public IWorld
{
private:
	constexpr static uint32_t debug_subdivisions = 512;

	std::shared_ptr<GMeshMaterial> _debugMaterial;
	std::shared_ptr<GMeshMaterial> _debugMaterialWire;
	std::shared_ptr<GActor> _debugChunkArea;
	std::shared_ptr<GActor> _debugSphere;
	std::shared_ptr<GActor> _debugCube;

public:
	ChunkAtlass _atlass;
	ChunkMeshfactory _meshFactory;

public:
	GPlanetWorld(std::shared_ptr<RenderScope> Scope);
	virtual ~GPlanetWorld();

	// uint32_t NumChunks() const { return _chunks.size(); }
	void updateChunks(const GCamera &Camera);

	void _initDebug();
	void _initVoxels(const glm::vec3&);
	void _redrawDirtyChunks();
	// void _redrawNewPosition(std::shared_ptr<RenderScope> Scope);
};