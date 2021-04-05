#ifndef ENEMYFACTORY_H
#define ENEMYFACTORY_H

#include "../Core/GameObject.h"

#include <string>
#include <unordered_map>
#include <mutex>

class Map;
class EnemyFactory;

enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};

namespace EnemyIDs
{
	constexpr int BASIC_TROLL = 0;
	constexpr int BASIC_ORC = 1;
	constexpr int BASIC_GOBLIN = 2;
	constexpr int COUNT = 3;
}

struct EnemyAnimations
{
	EnemyAnimations() : id(0), speed(0), frames(std::vector<int>()) {}
	unsigned int id;
	float speed;
	std::vector<int> frames;
};

class EnemyType
{
	friend class EnemyFactory;
public:

	EnemyType() : EnemyType(nullptr, 0, 0, 0, "", 0, 0) {}
	EnemyType(EnemyType* Parent, int Health, float Speed, int Gold, char* FileName, int Columns, int Rows, float r = 1.0, float g = 1.0, float b = 1.0);
	~EnemyType() { parent = nullptr; }

	int GetHealth() { return m_health; }
	float GetSpeed() { return m_speed; }

	void TakeDamage(int Damage) { m_health -= Damage; }

	int GetGold() { return m_goldOnDeath; }

	inline void GetRenderInfo(std::string& FileName, int& Columns, int& Rows)
	{
		FileName = fileName;
		Columns = columns;
		Rows = rows;
	}

	inline void GetColour(float& R, float& G, float& B)
	{
		R = colour[0];
		G = colour[1];
		B = colour[2];
	}

	void CreateAnimation(unsigned int Id, float Speed, const std::vector<int>& Frames)
	{

		EnemyAnimations animation = EnemyAnimations();
		animation.id = Id;
		animation.speed = Speed;
		animation.frames = Frames;

		animations.push_back(animation);
	}

	EnemyAnimations GetAnimation(unsigned int Id)
	{
		return animations[Id];
	}

private:

	EnemyType* parent;

	int m_health;
	float m_speed;

	int m_goldOnDeath;

	char* fileName;
	int columns, rows;
	float colour[3];

	std::vector<EnemyAnimations> animations;
};

class EnemyFactory
{
public:

	EnemyFactory(Map* pMap) { registry.InitializeAvailableEnemies(pMap); m_pMap = pMap; }
	GameObject* BuildEnemy(int EnemyID);
	~EnemyFactory() { Shutdown(); }
	
private:

	void Shutdown();

	Map* m_pMap;

	struct EnemyRegistry
	{
		void InitializeAvailableEnemies(Map* pMap);

		bool HasEnemy(int ID)
		{
			std::lock_guard<std::mutex> lg(sm_mutex);

			return enemies.find(ID) != enemies.end();
		}

		EnemyType* GetEnemy(int id)
		{
			std::lock_guard<std::mutex> lg(sm_mutex);

			return enemies[id];
		}

		static std::unordered_map<int, EnemyType*> enemies;
		static std::mutex sm_mutex;
	};

	static EnemyRegistry registry;

};

#endif