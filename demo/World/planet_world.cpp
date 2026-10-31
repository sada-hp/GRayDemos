#include "planet_world.hpp"
#include "Factories/VkImageFactory.hpp"
#include "Factories/VkMeshFactory.hpp"

glm::ivec3 prevPos = {0, 6360.f * 1e3, 0};

GPlanetWorld::GPlanetWorld(std::shared_ptr<RenderScope> InScope)
	: IWorld(InScope)
{
	_atlass = ChunkAtlass(10, CHUNK_ROW, CHUNK_SIZE);
	// _meshFactory.Size = VOXEL_SIZE;
}

GPlanetWorld::~GPlanetWorld()
{
	// _chunks.clear();
	_debugSphere = nullptr;
}

void GPlanetWorld::updateChunks(const GCamera &Camera)
{
	// return;
#if 0
	// TODO
	int face = 0;
	double dxy = 1.f / static_cast<float>(debug_subdivisions);

	glm::dvec3 center = Camera.GetWorldMatrix().GetPosition();
	glm::dvec3 dir = glm::normalize(center);

	double s = dir.x / dir.y;
	double t = dir.z / dir.y;

	uint32_t x = floor((s * 0.5 + 0.5) / dxy);
	uint32_t y = floor((t * 0.5 + 0.5) / dxy);

	MeshVertex v0{.position = earth_radius * glm::normalize(glm::dvec3(x * dxy - 0.5, 0.5, y * dxy - 0.5))};
	MeshVertex v1{.position = earth_radius * glm::normalize(glm::dvec3((x + 1) * dxy - 0.5, 0.5, y * dxy - 0.5))};
	MeshVertex v2{.position = earth_radius * glm::normalize(glm::dvec3(x * dxy - 0.5, 0.5, (y + 1) * dxy - 0.5))};
	MeshVertex v3{.position = earth_radius * glm::normalize(glm::dvec3((x + 1) * dxy - 0.5, 0.5, (y + 1) * dxy - 0.5))};

	v0.normal = glm::normalize(v0.position);
	v1.normal = glm::normalize(v1.position);
	v2.normal = glm::normalize(v2.position);
	v3.normal = glm::normalize(v3.position);

	std::vector<MeshVertex> vertices = {
		v0, v1, v2, v3};

	std::vector<uint32_t> indices = {
		0, 1, 2,
		1, 2, 3};

	auto Mesh = GVkMeshFactory::Mesh(Scope, vertices, indices);
	_debugChunkArea->AddComponent<GDrawable>(Mesh, _debugMaterial);
#else
		
	glm::ivec3 camPos = ChunkAtlass::ChunkGlobalId(Camera.GetWorldMatrix().GetPosition());

	if (camPos != prevPos)
	{
		prevPos = camPos;
		// _initVoxels(Scope, prevPos);
		_atlass.RecalculateDelta(prevPos);
		_redrawDirtyChunks();
	}
#endif
}

void GPlanetWorld::_initVoxels(const glm::vec3& position)
{
	// _chunks.clear();
	_atlass.RecalculateAll(position.x, position.y, position.z);

	if (!_debugMaterial || !_debugMaterialWire)
	{
		MaterialDescriptor debugMaterial;
		debugMaterial.SubmeshTextures.push_back(TexturePack{.Albedo = GVkImageFactory::SolidColor(Scope, COLOR(128, 255, 0))});
		debugMaterial.Compressed = true;
		// debugMaterial.PolygonMode = VK_POLYGON_MODE_LINE;
		// debugMaterial.CullMode = VK_CULL_MODE_NONE;

		MaterialDescriptor debugMaterialWire;
		debugMaterialWire.SubmeshTextures.push_back(TexturePack{.Albedo = GVkImageFactory::SolidColor(Scope, COLOR(0, 255, 0))});
		debugMaterialWire.PolygonMode = VK_POLYGON_MODE_LINE;
		debugMaterial.Compressed = true;

		_debugMaterial = GMeshMaterial::Create(Scope, debugMaterial);
		_debugMaterialWire = GMeshMaterial::Create(Scope, debugMaterialWire);
	}

	for (auto [p, c] : *_atlass.GetChunks())
	{
		auto mesh = _meshFactory.ChunkToMesh(Scope, c, GEllipsoid::Rg);

		std::shared_ptr<GActor> chunk = NewActor();
		chunk->AddComponent<GDrawable>(mesh, _debugMaterial);
		chunk->AddComponent<Chunk *>(c);
		chunk->SetPosition(p);

		//std::shared_ptr<GActor> debug_chunk = NewActor();
		//debug_chunk->AddComponent<GDrawable>(GVkMeshFactory::Cube(Scope, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE), _debugMaterialWire);
		//debug_chunk->Translate(glm::vec3{0, -GEllipsoid::Rg, 0});
		//debug_chunk->Translate(glm::vec3{p.x, p.y, p.z} + glm::vec3{static_cast<int64_t>(CHUNK_SIZE / 2)});

		// _chunks.push_back(chunk);
		// _chunks.push_back(debug_chunk);
		c->Mesh = chunk;
		c->dirty = false;
	}
}

void GPlanetWorld::_redrawDirtyChunks()
{
	// Подразумеваем, что номера чанков здесь в соответсвуют
	// номкрам мешей в массиве для отрисовки
	auto chunkList = *_atlass.GetChunks();

	for (auto [p, c] : *_atlass.GetChunks())
	{
		if (!c->dirty)
			continue;

		auto mesh = _meshFactory.ChunkToMesh(Scope, c, GEllipsoid::Rg);

		if (mesh)
		{
			std::shared_ptr<GActor> chunk = NewActor();
			chunk->AddComponent<GDrawable>(mesh, _debugMaterial);
			chunk->SetPosition(p);
			c->Mesh = chunk;
		}

		c->dirty = false;
	}
}

void GPlanetWorld::_initDebug()
{
	auto mesh = GVkMeshFactory::QuadrilateralSphere(Scope, GEllipsoid::Rg, debug_subdivisions);

	MaterialDescriptor debugMaterial;
	debugMaterial.SubmeshTextures.push_back(TexturePack{.Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 255, 0))});
	debugMaterial.PolygonMode = VK_POLYGON_MODE_LINE;

	_debugSphere = NewActor();
	_debugSphere->Translate({0, -6360.f * 1e3, 0});
	_debugSphere->AddComponent<GDrawable>(mesh, GMeshMaterial::Create(Scope, debugMaterial));

	MaterialDescriptor chunkMaterial;
	chunkMaterial.SubmeshTextures.push_back(TexturePack{.Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 0, 0))});
	chunkMaterial.CullMode = VK_CULL_MODE_NONE;
	_debugMaterial = GMeshMaterial::Create(Scope, chunkMaterial);

	_debugChunkArea = NewActor();
	_debugChunkArea->Translate({0, -6360.f * 1e3, 0});
}