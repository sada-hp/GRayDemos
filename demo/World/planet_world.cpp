#include "planet_world.hpp"
#include "Factories/VkImageFactory.hpp"
#include "Factories/VkMeshFactory.hpp"

GPlanetWorld::GPlanetWorld()
{
	_atlass = ChunkAtlass(3, 32);
	// _meshFactory.Size = 2;
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

glm::vec3 prevPos = {};

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

	bool b_x = camPos.x > prevPos.x + 32 || camPos.x < prevPos.x;
	bool b_y = camPos.y > prevPos.y + 32 || camPos.y < prevPos.y;
	bool b_z = camPos.z > prevPos.z + 32 || camPos.z < prevPos.z;

	if (b_x || b_y || b_z)
	{
		prevPos.x = (floor(camPos.x / 32) * 32) ;
		prevPos.y = (floor(camPos.y / 32) * 32) ;
		prevPos.z = (floor(camPos.z / 32) * 32) ;

		_initVoxels(Scope, prevPos, Camera);
	}
#endif
}

void GPlanetWorld::_initVoxels(std::shared_ptr<RenderScope> Scope, const glm::vec3 position, const GCamera &Camera)
{
	_chunks.clear();
	_atlass.Recalculate(position.x, position.y, position.z);

	if (!_debugMaterial)
	{
		MaterialDescriptor debugMaterial;
		debugMaterial.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(128, 255, 0)) });

		_debugMaterial = GMeshMaterial::Create(Scope, debugMaterial);
	}

	for (auto c : _atlass.GetChunks())
	{
		auto mesh = _meshFactory.ChunkToMesh(Scope, c, earth_radius);

		std::shared_ptr<GActor> chunk = NewActor();
		chunk->AddComponent<GDrawable>(mesh, _debugMaterial);

		// chunk->Translate(glm::normalize(position) * glm::vec3{0, -6360.f * 1e3, 0});
		chunk->Translate(glm::vec3{0, -6360.f * 1e3, 0});
		chunk->Translate({c->x, c->y, c->z});

		glm::dvec3 camPos = Camera.GetWorldMatrix().GetPosition();
		auto p = chunk->GetTransform().GetPosition();
		auto t = chunk->GetTransform();
		_chunks.push_back(chunk);
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