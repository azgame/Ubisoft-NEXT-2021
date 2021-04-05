#include "stdafx.h"
#include "TowerFactory.h"

#include "Map.h"

std::unordered_map<int, TowerFactory::Tower*> TowerFactory::TowerRegistry::towers;
std::mutex TowerFactory::TowerRegistry::sm_mutex;

TowerFactory::TowerRegistry TowerFactory::registry;

namespace TowerFunctions
{

}

GameObject* TowerFactory::BuildTower(int TowerID)
{
	if (registry.HasTower(TowerID))
	{
		auto tower = registry.towers[TowerID];
		GameObject* newObject = new GameObject();
		newObject->AddComponent<TowerComponent>(tower->tower->Clone());
		newObject->AddComponent<SpriteComponent>(tower->sprite->Clone());
		return newObject;
	}

	return nullptr;
}

void TowerFactory::Shutdown()
{
	for (auto keyval : registry.towers)
	{
		SAFE_DELETE(keyval.second)
	}

	registry.towers.clear();
}

void TowerFactory::TowerRegistry::InitializeAvailableTowers(Map* pMap)
{
	// With more time I'd load these in from a json file or something similar

	TowerComponent* basicTower = new TowerComponent();
	basicTower->Initialize(pMap, Vector2(0, 0), 1, 1, true);
	basicTower->SetName("Basic");
	basicTower->SetCost(3);
	basicTower->SetRadius(2);
	basicTower->SetAtkSpeed(0.25);
	basicTower->SetDamage(5);
	basicTower->SetAtkColour(0.0f, 0.6f, 0.8f);

	SpriteComponent* basicTowerS = new SpriteComponent();
	basicTowerS->LoadSprite(".\\TestData\\Ships.bmp", 2, 12);
	basicTowerS->SetFrame(0);
	basicTowerS->SetColor(0.0f, 0.6f, 0.8f);

	towers[TowerIDs::BASIC_TOWER] = new Tower(basicTower, basicTowerS);

	TowerComponent* fastTower = new TowerComponent();
	fastTower->Initialize(pMap, Vector2(0, 0), 1, 1, true);
	fastTower->SetName("Fast");
	fastTower->SetCost(4);
	fastTower->SetRadius(2);
	fastTower->SetAtkSpeed(0.5);
	fastTower->SetDamage(2);
	fastTower->SetAtkColour(0.1f, 0.9f, 0.5f);

	SpriteComponent* fastTowerS = new SpriteComponent();
	fastTowerS->LoadSprite(".\\TestData\\Ships.bmp", 2, 12);
	fastTowerS->SetFrame(0);
	fastTowerS->SetColor(0.1f, 0.9f, 0.5f);

	towers[TowerIDs::FAST_TOWER] = new Tower(fastTower, fastTowerS);

	TowerComponent* bigTower = new TowerComponent();
	bigTower->Initialize(pMap, Vector2(0, 0), 1, 1, true);
	bigTower->SetName("Big");
	bigTower->SetCost(6);
	bigTower->SetRadius(4);
	bigTower->SetAtkSpeed(0.1);
	bigTower->SetDamage(10);
	bigTower->SetAtkColour(0.9f, 0.3f, 0.8f);

	SpriteComponent* bigTowerS = new SpriteComponent();
	bigTowerS->LoadSprite(".\\TestData\\Ships.bmp", 2, 12);
	bigTowerS->SetFrame(0);
	bigTowerS->SetColor(0.9f, 0.3f, 0.9f);

	towers[TowerIDs::BIG_TOWER] = new Tower(bigTower, bigTowerS);
}