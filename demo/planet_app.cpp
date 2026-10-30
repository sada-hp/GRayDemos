#include "planet_app.hpp"
#include "UI/imgui_ui.hpp"

#define IsNotReleased(key) keyStates[key] != EAction::Release
#define IsJustPressed(key) keyStates[key] == EAction::Press

using namespace GEnums;

GPlanetApplication::GPlanetApplication()
	: IGApplication(SWindowParameters{1024, 720, "Procedural planet demo"})
	, Scope(Window.GetRenderer().GetScope())
	, world(Window.GetRenderer().GetScope())
{
	engineListener.SetUserPointer(this);
	Window.SetUpEvents(engineListener);
	Window.GetRenderer().AddRenderStage<GUI>(Window);

	engineListener.SubscribeKeyPressEvent(this, &GPlanetApplication::_keyPress);
	engineListener.SubscribeScrollEvent(this, &GPlanetApplication::_wheelScroll);
	engineListener.SubscribeMouseMoveEvent(this, &GPlanetApplication::_mouseMove);
	engineListener.SubscribeMousePressEvent(this, &GPlanetApplication::_mousePress);

	// world._initDebug();

	_debugCube = world.NewActor();
	_debugCube->AddComponent<GDrawable>(GVkMeshFactory::Cube(Scope, VOXEL_SIZE / 2, VOXEL_SIZE / 2, VOXEL_SIZE / 2), GMeshMaterial::Create(Scope));
	_debugCube->Translate(-glm::dvec3(100, 0, 400));
	Camera.GetWorldMatrix().Move(glm::dvec3(VOXEL_SIZE / 2));
}

GPlanetApplication::~GPlanetApplication()
{
}

void GPlanetApplication::_wheelScroll(GEvents::ScrollDelta Event, void *Data)
{
	camera_speed += Event.y * 100.0;
}

void GPlanetApplication::_mouseMove(GEvents::MousePosition Event, void *Data)
{
	if (mouseStates[GEnums::EMouse::Left] == GEnums::EAction::Press && !GUI::IsFocused())
	{
		const double Mult = 0.3;
		Camera.GetWorldMatrix().Rotate(Mult * Event.delta_y, Mult * -Event.delta_x, 0.0);
	}
}

void GPlanetApplication::_mousePress(GEvents::MousePress Event, void *Data)
{
	mouseStates[Event.key] = Event.action;
}

void GPlanetApplication::_keyPress(GEvents::KeyPress Event, void *Data)
{
	keyStates[Event.key] = Event.action;
}

glm::dvec3 CheckVoxelIntersection(glm::dvec3 voxel_pos, glm::dvec3 camera_pos, glm::dvec3 dir, double &tBest)
{
	tBest = std::numeric_limits<double>::infinity();
	int axis = -1;
	bool positive = false;

	glm::dvec3 l = voxel_pos;
	glm::dvec3 h = voxel_pos + VOXEL_SIZE;

	for (int i = 0; i < 3; i++)
	{
		if (dir[i] < 1e-9 && dir[i] > -1e-9)
			continue;

		bool pos = dir[i] > 0.0;
		double t = ((pos ? h[i] : l[i]) - camera_pos[i]) / dir[i]; // t >= 0, т.к. точка внутри

		if (t < tBest)
		{
			tBest = t;
			axis = i;
			positive = pos;
		}
	}
	static const glm::dvec3 faces[3][2] = {
		{glm::dvec3{-1, 0, 0}, glm::dvec3{1, 0, 0}},
		{glm::dvec3{0, -1, 0}, glm::dvec3{0, 1, 0}},
		{glm::dvec3{0, 0, -1}, glm::dvec3{0, 0, 1}},
	};

	return faces[axis][positive];
}

void GPlanetApplication::_destroyVoxel()
{
	glm::dvec3 pos = Camera.GetWorldMatrix().GetPosition();
	glm::dvec3 fwd = Camera.GetWorldMatrix().GetForward();

	// глобальная позиция вокселя
	glm::dvec3 globalVoxelPos = glm::floor((pos / VOXEL_SIZE)) * VOXEL_SIZE;
	// глобальная позиция чанка
	glm::dvec3 globalChunkPos = glm::floor((pos / CHUNK_SIZE)) * CHUNK_SIZE;
	// локальная позиция чанка
	glm::dvec3 voxelPosToCheck = glm::floor((globalVoxelPos - globalChunkPos) / VOXEL_SIZE);

	const int maxReach = 10;
	double t = 0;
	int reach = 1;

	while (reach <= maxReach)
	{
		// Сторона с которой столкнулся смотрящий из камеры вектор
		auto fc = CheckVoxelIntersection(globalVoxelPos, pos + fwd * t, fwd, t);
		globalVoxelPos += fc * VOXEL_SIZE;

		// Чанк, в котором находится воксель
		auto c = world._atlass.GetChunkInPoint(globalVoxelPos);
		if (c)
		{
			voxelPosToCheck += fc;
			glm::dvec3 pvp = voxelPosToCheck;
			for (int fc_i = 0; fc_i < 3; fc_i++)
			{
				if (voxelPosToCheck[fc_i] < 0)
					voxelPosToCheck[fc_i] += CHUNK_ROW;
				if (voxelPosToCheck[fc_i] >= CHUNK_ROW)
					voxelPosToCheck[fc_i] -= CHUNK_ROW;
			}
			int i = voxelPosToCheck.x + voxelPosToCheck.y * CHUNK_ROW + voxelPosToCheck.z * CHUNK_SQARE;

			if (c->VoxelArray[i].type == 0)
			{
				reach++;
				continue;
			}

			c->dirty = true;
			c->VoxelArray[i].type = 0;

			world._redrawDirtyChunks();
			break;
		}
		else
		{
			break;
		}
	}
}

void GPlanetApplication::_updateCamera(float Delta)
{
	glm::vec3 translation(0.0);

	if (IsNotReleased(EKey::R))
	{
		camera_speed = DEFAULT_CAMERA_SPEED;
		// world._redrawDirtyChunks(Scope);
		auto prevPos = Camera.GetWorldMatrix().GetPosition();
		world._initVoxels(prevPos);
	}

	if (mouseStates[EMouse::Middle] != EAction::Release)
	{
		_destroyVoxel();
	}

	if (IsNotReleased(EKey::W))
		translation.z += camera_speed * Delta;

	if (IsNotReleased(EKey::S))
		translation.z -= camera_speed * Delta;

	if (IsNotReleased(EKey::A))
		translation.x += camera_speed * Delta;

	if (IsNotReleased(EKey::D))
		translation.x -= camera_speed * Delta;

	if (IsNotReleased(EKey::E) || IsNotReleased(EKey::Space))
		translation.y += camera_speed * Delta;

	if (IsNotReleased(EKey::Q) || IsNotReleased(EKey::LeftControl))
		translation.y -= camera_speed * Delta;

	if (translation.x != 0.0 || translation.y != 0.0 || translation.z != 0.0)
	{
		Camera.GetWorldMatrix().Move(translation);
	}
}

void GPlanetApplication::Update(float Delta)
{
	Window.SetTitle(("Procedural planet demo " + std::format("{:.1f}", 1.0 / Delta)).c_str());
	_updateCamera(Delta);

	world.updateChunks(Camera);
}