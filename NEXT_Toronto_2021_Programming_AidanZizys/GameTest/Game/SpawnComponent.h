#ifndef SPAWNCOMPONENT_H
#define SPAWNCOMPONENT_H

#include "MapUnit.h"

#include "EnemyFactory.h"
#include "HomeBaseComponent.h"

#include "../Core/Observer.h"

class SpawnComponent : public IMapUnit, public Subject
{
public:

	SpawnComponent() : 
		originalSpawnCooldown(100), spawnCooldown(100), spawnCounter(0), spawnCount(0), waveEnd(20),
		intermissionEnd(false), waveCount(1) {}
		// arbitrary spawning rate values for testing, replace with either difficulty, level, scaling, whatever

	void Update(float deltaTime) override;
	void Render() override;
	void OnDetach() override;

	void Initialize(Map* pMap, Vector2 Position, int Width, int Height, bool Exclusive) override;

	void UpdateMapPosition(Vector2 Position) override;
	void UpdatePosition() override {};
	void Move(Vector2 Destination) override {};

	void SetHomeBase(HomeBaseComponent* Home)
	{
		m_home = Home;
	}

private:

	int spawnCount;
	int waveEnd;
	int spawnCooldown;
	int originalSpawnCooldown;
	int spawnCounter;
	int waveCount;
	bool intermissionEnd;

	HomeBaseComponent* m_home;

	// EnemyFactory
	EnemyFactory* m_pEnemyFactory;
};

#endif