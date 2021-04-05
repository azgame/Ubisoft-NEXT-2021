//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------

//------------------------------------------------------------------------

#include "Core/SceneObjects.h" 
#include "Core/GameObject.h"

#include "Core/InputComponent.h"
#include "Core/SpriteComponent.h"
#include "Game/EnemyComponent.h"
#include "Game/TowerComponent.h"
#include "Game/PlayerComponent.h"
#include "Game/SpawnComponent.h"
#include "Game/HomeBaseComponent.h"

#include "Game/Map.h"
#include "Game/Pathfinding.h"

Map* map;

GameObject* spawner;
GameObject* homeBase;

constexpr uint16_t mapCellSize = 48;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	map = new Map(mapCellSize);

	GameObject* player = new GameObject();
	player->AddComponents<InputComponent, PlayerComponent>();
	player->GetComponent<PlayerComponent>()->Initialize(map);
	player->GetComponent<PlayerComponent>()->AddObserver(&Pathfinding::GetInstance());
	
	homeBase = new GameObject();
	homeBase->AddComponent<SpriteComponent>();
	homeBase->GetComponent<SpriteComponent>()->LoadSprite(".\\TestData\\Ships.bmp", 2, 12);
	homeBase->GetComponent<SpriteComponent>()->SetFrame(6);
	homeBase->AddComponent<HomeBaseComponent>();
	homeBase->GetComponent<HomeBaseComponent>()->Initialize(map, Vector2(map->GetWidth() - 1, map->GetHeight() / 2), 1, 1, false);
	homeBase->GetComponent<HomeBaseComponent>()->SetHealth(200);
	homeBase->GetComponent<HomeBaseComponent>()->AddObserver(player->GetComponent<PlayerComponent>());
	homeBase->GetComponent<HomeBaseComponent>()->UpdatePosition();

	spawner = new GameObject();
	spawner->AddComponent<SpawnComponent>();
	spawner->GetComponent<SpawnComponent>()->Initialize(map, Vector2(0, 2), 1, 1, false);
	spawner->GetComponent<SpawnComponent>()->SetHomeBase(homeBase->GetComponent<HomeBaseComponent>());
	spawner->GetComponent<SpawnComponent>()->AddObserver(&Pathfinding::GetInstance());
	spawner->GetComponent<SpawnComponent>()->AddObserver(homeBase->GetComponent<HomeBaseComponent>());

	Pathfinding::GetInstance().Initialize(map, spawner->GetComponent<SpawnComponent>(), homeBase->GetComponent<HomeBaseComponent>());

	SceneObjects::GetInstance().InitObjects();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	SceneObjects::GetInstance().CleanupObjects();
	SceneObjects::GetInstance().UpdateObjects(deltaTime);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	//if (!Pathfinding::GetInstance().IsPathEmpty())
		//Pathfinding::GetInstance().DrawPath();

	SceneObjects::GetInstance().DrawObjects();
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	SAFE_DELETE(map);

	SceneObjects::GetInstance().Shutdown();
}