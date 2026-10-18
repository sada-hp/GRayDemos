#include "planet_world.hpp"
#include "Factories/VkImageFactory.hpp"
#include "Factories/VkMeshFactory.hpp"

GPlanetWorld::GPlanetWorld()
{
	_atlass = ChunkAtlass(3, CHUNK_ROW, CHUNK_SIZE);
	_meshFactory.Size = VOXEL_SIZE;
}

GPlanetWorld::~GPlanetWorld()
{
	_chunks.clear();
	_debugSphere = nullptr;
}

void GPlanetWorld::GetDrawableObjects(std::vector<std::pair<const GDrawable *, const EllipsoidMatrix *>> &objects) const
{
	auto view = _registry.view<EllipsoidMatrix, GDrawable>();
	for (const auto &[ent, matrix, drawable] : view.each())
		objects.emplace_back(&drawable, &matrix);
}

std::shared_ptr<GActor> GPlanetWorld::NewActor()
{
	return std::make_shared<GActor>(_registry);
}

glm::vec3 prevPos = {-INFINITY, -INFINITY, -INFINITY};

void GPlanetWorld::updateChunks(std::shared_ptr<RenderScope> Scope, const GCamera &Camera)
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
	glm::dvec3 camPos = Camera.GetWorldMatrix().GetPosition();

	bool b_x = camPos.x > prevPos.x + (CHUNK_SIZE) || camPos.x < prevPos.x;
	bool b_y = camPos.y > prevPos.y + (CHUNK_SIZE) || camPos.y < prevPos.y;
	bool b_z = camPos.z > prevPos.z + (CHUNK_SIZE) || camPos.z < prevPos.z;

	if (b_x || b_y || b_z)
	{
		prevPos.x = (floor(camPos.x / (CHUNK_SIZE)) * (CHUNK_SIZE));
		prevPos.y = (floor(camPos.y / (CHUNK_SIZE)) * (CHUNK_SIZE));
		prevPos.z = (floor(camPos.z / (CHUNK_SIZE)) * (CHUNK_SIZE));

		_initVoxels(Scope, prevPos, Camera);
	}
#endif
}

void GPlanetWorld::_initVoxels(std::shared_ptr<RenderScope> Scope, const glm::vec3 position, const GCamera &Camera)
{
	_chunks.clear();
	_atlass.RecalculateAll(position.x, position.y, position.z);

	if (!_debugMaterial || !_debugMaterialWire)
	{
		MaterialDescriptor debugMaterial;
		debugMaterial.SubmeshTextures.push_back(TexturePack{.Albedo = GVkImageFactory::SolidColor(Scope, COLOR(128, 255, 0))});

		MaterialDescriptor debugMaterialWire;
		debugMaterialWire.SubmeshTextures.push_back(TexturePack{.Albedo = GVkImageFactory::SolidColor(Scope, COLOR(0, 255, 0))});
		debugMaterialWire.PolygonMode = VK_POLYGON_MODE_LINE;

		_debugMaterial = GMeshMaterial::Create(Scope, debugMaterial);
		_debugMaterialWire = GMeshMaterial::Create(Scope, debugMaterialWire);
	}

	for (auto c : *_atlass.GetChunks())
	{
		auto mesh = _meshFactory.ChunkToMesh(Scope, c, earth_radius);

		std::shared_ptr<GActor> chunk = NewActor();
		chunk->AddComponent<GDrawable>(mesh, _debugMaterial);
		chunk->AddComponent<Chunk *>(c);
		chunk->Translate(glm::vec3{0, -6360.f * 1e3, 0});
		chunk->Translate(glm::vec3{c->x, c->y, c->z});

		std::shared_ptr<GActor> debug_chunk = NewActor();
		debug_chunk->AddComponent<GDrawable>(GVkMeshFactory::Cube(Scope, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE), _debugMaterialWire);
		debug_chunk->Translate(glm::vec3{0, -6360.f * 1e3, 0});
		debug_chunk->Translate(glm::vec3{c->x, c->y, c->z} + glm::vec3{static_cast<int64_t>(CHUNK_SIZE / 2)});

		_chunks.push_back(chunk);
		// _chunks.push_back(debug_chunk);
		c->dirty = false;
	}
}

void GPlanetWorld::_redrawDirtyChunks(std::shared_ptr<RenderScope> Scope)
{
	// Подразумеваем, что номера чанков здесь в соответсвуют 
	// номкрам мешей в массиве для отрисовки
	auto chunkList = *_atlass.GetChunks();

	for (int i = 0; i < chunkList.size(); i++)
	{
		Chunk *c = chunkList[i];
		if (!c->dirty)
			continue;

		auto mesh = _meshFactory.ChunkToMesh(Scope, c, earth_radius);

		std::shared_ptr<GActor> chunk = NewActor();
		if (mesh)
		{
			chunk->AddComponent<GDrawable>(mesh, _debugMaterial);
			chunk->Translate(glm::vec3{0, -6360.f * 1e3, 0});
			chunk->Translate(glm::vec3{c->x, c->y, c->z} + glm::vec3{static_cast<int64_t>(CHUNK_SIZE / 2)});
		}

		// auto ca = _chunks.at(i);
		_chunks[i] = chunk;
		c->dirty = false;
	}
}

void GPlanetWorld::_initDebug(std::shared_ptr<RenderScope> Scope)
{
	auto mesh = GVkMeshFactory::QuadrilateralSphere(Scope, earth_radius, debug_subdivisions);

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