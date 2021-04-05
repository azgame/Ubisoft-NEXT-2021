#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H

#include "../Core/Component.h"
#include "../Core/Observer.h"

#include "MapUnit.h"

#include "EnemyFactory.h"
#include "HomeBaseComponent.h"

#include <queue>

class TowerComponent;

class EnemyComponent : public IMapUnit, public Observer
{
	friend class EnemyFactory;
public:

	EnemyComponent() : moveCooldown(300) {}

	void Update(float deltaTime) override;
	void OnDetach() override { m_home = nullptr; }

	void UpdateMapPosition(Vector2 Position) override;
	void UpdatePosition() override;
	void Move(Vector2 Destination) override;

	void SetPath(std::queue<Vector2>* pPath) { path = *pPath; }
	void SetSpawnLocation(Vector2 Position) { spawnLocation = Position; }
	void SetHome(HomeBaseComponent* pHome) { m_home = pHome; }

	void TakeDamage(int Damage, TowerComponent* pTower);
	void Die();

	void OnNotify() override
	{
		// Try to make way to path
		UpdateRoute();
	}

private:

	int moveCounter;
	float moveCooldown;

	void UpdateRoute();

	std::queue<Vector2> path;
	Vector2 nextStop;
	Vector2 spawnLocation;
	HomeBaseComponent* m_home;
	EnemyType enemy;
};

#endif