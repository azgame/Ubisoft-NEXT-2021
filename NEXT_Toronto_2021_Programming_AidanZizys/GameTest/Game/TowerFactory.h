#ifndef TOWERFACTORY_H
#define TOWERFACTORY_H

#include "../Core/GameObject.h"
#include "../Core/SpriteComponent.h"

#include "TowerComponent.h"

#include <unordered_map>
#include <mutex>

class Map;

namespace TowerIDs
{
	constexpr int BASIC_TOWER = 0;
	constexpr int FAST_TOWER = 1;
	constexpr int BIG_TOWER = 2;
	constexpr int COUNT = 3;
}

class TowerFactory
{
public:

	TowerFactory(Map* pMap) { registry.InitializeAvailableTowers(pMap); }
	GameObject* BuildTower(int TowerID);
	~TowerFactory() { Shutdown(); }

private:

	void Shutdown();

	struct Tower
	{
		Tower(TowerComponent* Tower, SpriteComponent* Sprite)
		{
			tower = Tower;
			sprite = Sprite;
		}

		~Tower()
		{
			SAFE_DELETE(tower);
			SAFE_DELETE(sprite);
		}

		TowerComponent* tower;
		SpriteComponent* sprite;
	};

	struct TowerRegistry
	{
		void InitializeAvailableTowers(Map* pMap);

		bool HasTower(int ID)
		{
			std::lock_guard<std::mutex> lg(sm_mutex);

			return towers.find(ID) != towers.end();
		}

		Tower* GetTower(int id)
		{
			std::lock_guard<std::mutex> lg(sm_mutex);

			return towers[id];
		}

		static std::unordered_map<int, Tower*> towers;
		static std::mutex sm_mutex;
	};

	static TowerRegistry registry;
};


#endif