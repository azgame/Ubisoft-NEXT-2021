#ifndef TOWERCOMPONENT_H
#define TOWERCOMPONENT_H

#include "../Core/Component.h"
#include "MapUnit.h"

class PlayerComponent;
class EnemyComponent;

class Bullet : public Component
{
public:

	virtual void OnAttach(GameObject* pParent) override { m_gameObject = pParent; };
	virtual void OnDetach() override {};
	virtual void OnStart() override {};
	virtual void Update(float deltaTime) override {};
	virtual void Render() override {};

	void SetTravelSpeed(float Speed) { travelSpeed = Speed; }
	void SetMap(Map* pMap) { m_pMap = pMap; }
	void SetTarget(EnemyComponent* Enemy) { m_target = Enemy; }
	void SetStartingLocation(Vector2 Start) { m_startLocation = Start; }
	void SetTargetLocation(Vector2 Target) { m_targetLocation = Target; }

	void ReachedTarget();

private:

	float travelSpeed;
	EnemyComponent* m_target;
	Vector2 m_startLocation;
	Vector2 m_targetLocation;
	Map* m_pMap;
};

class TowerComponent : public IMapUnit
{
public:

	TowerComponent() : m_attackCounter(0), m_attackCooldown(APP_MAX_FRAME_RATE) { m_isOnMap = false; }

	void Update(float deltaTime) override;
	void Render() override;

	void UpdateMapPosition(Vector2 Position) override;
	void UpdatePosition() override;
	void Move(Vector2 Direction) override;

	void SetPlayer(PlayerComponent* pPlayer) { m_player = pPlayer; }
	void SetName(std::string Name) { m_name = Name; }
	void SetRadius(int Radius) { m_damageRadius = Radius; }
	void SetDamage(int Damage) { m_damage = Damage; }
	void SetAtkSpeed(float Speed) { m_attackSpeed = Speed; }
	void SetAtkColour(float r, float g, float b) 
		{ lazerColour[0] = r; lazerColour[1] = b; lazerColour[2] = b; }
	void SetCost(int Cost) { m_cost = Cost; }
	int GetCost() { return m_cost; }

	void KilledEnemy(int Gold);
	bool TryPlaceTower();

	TowerComponent* Clone();

private:

	void FindTarget();
	bool CanFire();
	void Fire();

	std::vector<IMapUnit*> GetLine(int x1, int y1, int x2, int y2);
	void GetRadius(std::vector<IMapUnit*>& Units);

	EnemyComponent* m_currentTarget;
	Vector2 m_targetLocation;

	PlayerComponent* m_player;
	int m_cost;

	int m_attackCounter;
	int m_attackCooldown;

	bool m_hasFired;
	float m_attackSpeed;
	int m_damage;
	int m_damageRadius;

	float lazerColour[3];
};

#endif