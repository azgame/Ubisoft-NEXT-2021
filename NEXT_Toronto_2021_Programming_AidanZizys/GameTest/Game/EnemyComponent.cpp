#include "stdafx.h"
#include "EnemyComponent.h"

#include "Map.h"

#include "Pathfinding.h"
#include "TowerComponent.h"

#include "../Core/SpriteComponent.h"

#include <math.h>

void EnemyComponent::Update(float deltaTime)
{
	moveCounter++;

	if (moveCounter > moveCooldown / enemy.GetSpeed())
	{
		moveCounter = 0;
		if (!path.empty())
			UpdateMapPosition(path.front()); path.pop();
		UpdatePosition();
	}

	m_gameObject->GetComponent<SpriteComponent>()->SetAnimation(ANIM_RIGHT);
}

void EnemyComponent::UpdateMapPosition(Vector2 Position)
{
	m_pMap->RemoveUnitFromMap(this);

	m_mapPosition = Position;

	if (m_mapPosition == Pathfinding::GetInstance().GetGoal())
	{
		m_home->TakeDamage(enemy.GetHealth());
		Die();
		return;
	}

	if (m_pMap->CanAddToMap(this))
	{
		m_isOnMap = true;
		m_pMap->AddUnitToMap(this);
	}
	else
	{
		m_isOnMap = false;
	}
}

void EnemyComponent::UpdatePosition()
{
	float cellWidth = APP_VIRTUAL_WIDTH / m_pMap->GetWidth();
	float cellHeight = APP_VIRTUAL_HEIGHT / m_pMap->GetHeight();

	Vector2 snappedPosition;

	snappedPosition.x = (cellWidth * (m_mapPosition.x)) + m_width * (cellWidth / 2);
	snappedPosition.y = (cellHeight * (m_mapPosition.y)) + m_height * (cellHeight / 2);

	m_gameObject->SetPosition(snappedPosition);
}

void EnemyComponent::Move(Vector2 Destination)
{
}

void EnemyComponent::TakeDamage(int Damage, TowerComponent* pTower)
{
	enemy.TakeDamage(Damage);
	if (enemy.GetHealth() <= 0)
	{
		pTower->KilledEnemy(enemy.GetGold());
		Die();
	}
}

void EnemyComponent::Die()
{
	Pathfinding::GetInstance().RemoveObserver(this);
	m_pMap->RemoveUnitFromMap(this);
	m_gameObject->MarkForDeletion();
}

void EnemyComponent::UpdateRoute()
{
	if (!Pathfinding::GetInstance().TryMakePath(m_mapPosition, Pathfinding::GetInstance().GetGoal(), path))
	{
		UpdateMapPosition(spawnLocation);
		UpdatePosition();
		path = *Pathfinding::GetInstance().GetPath();
	}
}
