#pragma once
#include "Engine/camera.hpp"
#include "Engine/world.hpp"
#include <entt/entt.hpp>
#include "actor.hpp"

#include "Materials/mesh_material.hpp"
#include "chunks.hpp"
#include "dummy.hpp"

class GPlanetWorld : public IWorld
{
	constexpr static uint32_t debug_subdivisions = 512;
	constexpr static double earth_radius = 6360.f * 1e3;

	std::vector<std::shared_ptr<GActor>> _chunks;
	entt::registry _registry;

	std::shared_ptr<GMeshMaterial> _debugMaterial;
	std::shared_ptr<GActor> _debugChunkArea;
	std::shared_ptr<GActor> _debugSphere;
	ChunkAtlass _atlass;
	ChunkMeshfactory _meshFactory;

public:
	GPlanetWorld();
	virtual ~GPlanetWorld();

	void GetDrawableObjects(std::vector<std::pair<const GDrawable *, const EllipsoidMatrix *>> &objects) const override;
	std::shared_ptr<GActor> NewActor();

	uint32_t NumChunks() const { return _chunks.size(); }
	void updateChunks(std::shared_ptr<RenderScope> Scope, const GCamera &Camera);

	void _initDebug(std::shared_ptr<RenderScope> Scope);
	void _initVoxels(std::shared_ptr<RenderScope> Scope, const glm::vec3, const GCamera &Camera);
};