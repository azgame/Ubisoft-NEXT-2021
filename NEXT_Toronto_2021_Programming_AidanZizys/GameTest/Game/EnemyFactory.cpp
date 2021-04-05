#include "stdafx.h"
#include "EnemyFactory.h"

#include "../Core/SpriteComponent.h"

#include "Map.h"

#include "EnemyComponent.h"

std::mutex EnemyFactory::EnemyRegistry::sm_mutex;
std::unordered_map<int, EnemyType*> EnemyFactory::EnemyRegistry::enemies;

EnemyFactory::EnemyRegistry EnemyFactory::registry;


GameObject* EnemyFactory::BuildEnemy(int EnemyID)
{
	if (registry.HasEnemy(EnemyID))
	{
		auto enemy = registry.enemies[EnemyID];
		GameObject* newObject = new GameObject();
		newObject->AddComponent<EnemyComponent>();
		newObject->GetComponent<EnemyComponent>()->enemy = *enemy;
		newObject->GetComponent<EnemyComponent>()->Initialize(m_pMap, Vector2(0, 0), 1, 1, false);
		newObject->AddComponent<SpriteComponent>();

		std::string fileName;
		int columns, rows;
		enemy->GetRenderInfo(fileName, columns, rows);
		newObject->GetComponent<SpriteComponent>()->LoadSprite(fileName.c_str(), columns, rows);
		newObject->GetComponent<SpriteComponent>()->SetColor(enemy->colour[0], enemy->colour[1], enemy->colour[2]);
		newObject->GetComponent<SpriteComponent>()->SetFrame(0);
		EnemyAnimations animation = enemy->GetAnimation(ANIM_FORWARDS);
		newObject->GetComponent<SpriteComponent>()->CreateAnimation(animation.id, animation.speed, animation.frames);
		animation = enemy->GetAnimation(ANIM_BACKWARDS);
		newObject->GetComponent<SpriteComponent>()->CreateAnimation(animation.id, animation.speed, animation.frames);
		animation = enemy->GetAnimation(ANIM_LEFT);
		newObject->GetComponent<SpriteComponent>()->CreateAnimation(animation.id, animation.speed, animation.frames);
		animation = enemy->GetAnimation(ANIM_RIGHT);
		newObject->GetComponent<SpriteComponent>()->CreateAnimation(animation.id, animation.speed, animation.frames);

		return newObject;
	}

	return nullptr;
}

void EnemyFactory::Shutdown()
{
	for (auto keyval : registry.enemies)
	{
		SAFE_DELETE(keyval.second);
	}

	registry.enemies.clear();
}

EnemyType::EnemyType(EnemyType* Parent, int Health, float Speed, int Gold, char* FileName, int Columns, int Rows, float r, float g, float b) :
	parent(Parent), m_health(Health), m_speed(Speed), m_goldOnDeath(Gold), fileName(FileName), columns(Columns), rows(Rows)
{
	if (parent != nullptr)
	{
		if (m_health == 0) m_health = parent->GetHealth();
		if (m_speed == 0) m_speed = parent->GetSpeed();
	}

	colour[0] = r;
	colour[1] = g;
	colour[2] = b;
}

void EnemyFactory::EnemyRegistry::InitializeAvailableEnemies(Map* pMap)
{
	// With more time I'd load these in from a json file or something similar
	EnemyType* trollType = new EnemyType(nullptr, 30, 5, 2, ".\\TestData\\Test.bmp", 8, 4, 1.0, 0.6, 0.1);
	float speed = 1.0f / 15.0f;
	trollType->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	trollType->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	trollType->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	trollType->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	
	registry.enemies[EnemyIDs::BASIC_TROLL] = trollType;

	EnemyType* orcType = new EnemyType(nullptr, 45, 2, 3, ".\\TestData\\Test.bmp", 8, 4, 0.2, 1.0, 0.2);
	orcType->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	orcType->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	orcType->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	orcType->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });

	registry.enemies[EnemyIDs::BASIC_ORC] = orcType;

	EnemyType* goblinType = new EnemyType(nullptr, 15, 10, 1, ".\\TestData\\Test.bmp", 8, 4, 0.5, 0.2, 0.8);
	goblinType->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	goblinType->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	goblinType->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	goblinType->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });

	registry.enemies[EnemyIDs::BASIC_GOBLIN] = goblinType;
}