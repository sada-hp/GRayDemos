#include "planet_world.hpp"
#include "Factories/VkImageFactory.hpp"
#include "Factories/VkMeshFactory.hpp"

GPlanetWorld::GPlanetWorld()
{

}

GPlanetWorld::~GPlanetWorld()
{
	_chunks.clear();
	_debugSphere = nullptr;
}

void GPlanetWorld::GetDrawableObjects(std::vector<std::pair<const GDrawable*, const EllipsoidMatrix*>>& objects) const
{
	auto view = _registry.view<EllipsoidMatrix, GDrawable>();
	for (const auto& [ent, matrix, drawable] : view.each())
		objects.emplace_back(&drawable, &matrix);
}

std::shared_ptr<GActor> GPlanetWorld::NewActor()
{
	return std::make_shared<GActor>(_registry);
}

void GPlanetWorld::updateChunks(std::shared_ptr<RenderScope> Scope, const GCamera& Camera)
{
#if 1
	// TODO
	int face = 0;
	double dxy = 1.f / static_cast<float>(debug_subdivisions);

	glm::dvec3 center = Camera.GetWorldMatrix().GetPosition();
	glm::dvec3 dir = glm::normalize(center);

	double s = dir.x / dir.y;
	double t = dir.z / dir.y;

	uint32_t x = floor((s * 0.5 + 0.5) / dxy);
	uint32_t y = floor((t * 0.5 + 0.5) / dxy);

	MeshVertex v0{ .position = earth_radius * glm::normalize(glm::dvec3(x * dxy - 0.5, 0.5, y * dxy - 0.5)) };
	MeshVertex v1{ .position = earth_radius * glm::normalize(glm::dvec3((x + 1) * dxy - 0.5, 0.5, y * dxy - 0.5)) };
	MeshVertex v2{ .position = earth_radius * glm::normalize(glm::dvec3(x * dxy - 0.5, 0.5, (y + 1) * dxy - 0.5)) };
	MeshVertex v3{ .position = earth_radius * glm::normalize(glm::dvec3((x + 1) * dxy - 0.5, 0.5, (y + 1) * dxy - 0.5)) };

	v0.normal = glm::normalize(v0.position);
	v1.normal = glm::normalize(v1.position);
	v2.normal = glm::normalize(v2.position);
	v3.normal = glm::normalize(v3.position);

	std::vector<MeshVertex> vertices = {
		v0, v1, v2, v3
	};

	std::vector<uint32_t> indices = {
		0, 1, 2,
		1, 2, 3
	};

	auto Mesh = GVkMeshFactory::Mesh(Scope, vertices, indices);
	_debugChunkArea->AddComponent<GDrawable>(Mesh, _debugMaterial);
#else
	MaterialDescriptor chunkMaterial1;
	chunkMaterial1.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 0, 0)) });

	MaterialDescriptor chunkMaterial2;
	chunkMaterial2.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 255, 0)) });
	chunkMaterial2.PolygonMode = VK_POLYGON_MODE_LINE;

	DemoOperators op;
	auto chunk = std::shared_ptr<Chunk>(new Chunk());
	op.FillChunk(chunk);

	std::shared_ptr<IMaterial> ChunkMaterialSolid = GMeshMaterial::Create(Scope, chunkMaterial1);
	std::shared_ptr<IMaterial> ChunkMaterialWire = GMeshMaterial::Create(Scope, chunkMaterial2);
	std::shared_ptr<IMesh> ChunkMesh = op.ChunkToMesh(Scope, chunk);

	const int32_t radius = 10;
	for (int32_t y = -radius; y < radius; y++)
	{
		for (int32_t x = -radius; x < radius; x++)
		{
			std::shared_ptr<GActor> ChunkActor = NewActor();
			ChunkActor->AddComponent<GDrawable>(ChunkMesh, std::vector{ ChunkMaterialSolid, ChunkMaterialWire });
			ChunkActor->Translate({ DemoOperators::ChunkLength() * x, 0, DemoOperators::ChunkLength() * y });
			_chunks.push_back(ChunkActor);
		}
	}
#endif
}

void GPlanetWorld::_initDebug(std::shared_ptr<RenderScope> Scope)
{
	auto mesh = GVkMeshFactory::QuadrilateralSphere(Scope, earth_radius, debug_subdivisions);

	MaterialDescriptor debugMaterial;
	debugMaterial.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 255, 0)) });
	debugMaterial.PolygonMode = VK_POLYGON_MODE_LINE;

	_debugSphere = NewActor();
	_debugSphere->Translate({ 0, -6360.f * 1e3, 0 });
	_debugSphere->AddComponent<GDrawable>(mesh, GMeshMaterial::Create(Scope, debugMaterial));

	MaterialDescriptor chunkMaterial;
	chunkMaterial.SubmeshTextures.push_back(TexturePack{ .Albedo = GVkImageFactory::SolidColor(Scope, COLOR(255, 0, 0)) });
	chunkMaterial.CullMode = VK_CULL_MODE_NONE;
	_debugMaterial = GMeshMaterial::Create(Scope, chunkMaterial);

	_debugChunkArea = NewActor();
	_debugChunkArea->Translate({ 0, -6360.f * 1e3, 0 });
}