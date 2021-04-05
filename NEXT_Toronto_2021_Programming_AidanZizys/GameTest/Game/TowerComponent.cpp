#include "stdafx.h"
#include "TowerComponent.h"

#include "../Core/SpriteComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

#include "Map.h"

#include "../Utility/MapUtility.h"

void TowerComponent::Update(float deltaTime)
{
	m_attackCounter += deltaTime;

	if (((float)m_attackCounter * m_attackSpeed) > (float)m_attackCooldown / 4.0)
	{
		m_hasFired = false;
	}

	if (m_isOnMap)
	{
		FindTarget();

		if (CanFire())
			Fire();
	}
}

void TowerComponent::Render()
{
	if (m_hasFired && m_currentTarget != nullptr)
	{
		// hit scan
		Vector2 thisPos = m_gameObject->GetPosition();
		Vector2 targetPos = m_currentTarget->GetGameObject()->GetPosition();

		App::DrawLine(thisPos.x, thisPos.y, targetPos.x, targetPos.y, lazerColour[0], lazerColour[1], lazerColour[2]);
	}
}

void TowerComponent::UpdateMapPosition(Vector2 Position)
{
	m_pMap->RemoveUnitFromMap(this);

	m_mapPosition = Position;

	if (m_mapPosition.x + m_width > m_pMap->GetWidth())
		m_mapPosition.x = m_pMap->GetWidth() - m_width;

	if (m_mapPosition.x < 1) // lazy, far left column of cells is for enemy spawning
		m_mapPosition.x = 1;

	if (m_mapPosition.y + m_height > m_pMap->GetHeight())
		m_mapPosition.y = m_pMap->GetHeight() - m_height;

	if (m_mapPosition.y < 2) // lazy, saving bottom 2 cells for UI
		m_mapPosition.y = 2;

	if (m_pMap->CanAddToMap(this))
	{
		m_gameObject->GetComponent<SpriteComponent>()->SetColor(0.2f, 1.0f, 0.2f);
	}
	else
	{
		m_gameObject->GetComponent<SpriteComponent>()->SetColor(1.0f, 0.2f, 0.2f);
	}

	UpdatePosition();
}

void TowerComponent::UpdatePosition()
{
	float cellWidth = APP_VIRTUAL_WIDTH / m_pMap->GetWidth();
	float cellHeight = APP_VIRTUAL_HEIGHT / m_pMap->GetHeight();

	Vector2 snappedPosition;

	snappedPosition.x = (cellWidth * (m_mapPosition.x)) + m_width * (cellWidth / 2);
	snappedPosition.y = (cellHeight * (m_mapPosition.y)) + m_height * (cellHeight / 2);

	m_gameObject->SetPosition(snappedPosition);
	m_gameObject->SetScale(((m_width > m_height) ? m_width : m_height) * ((cellWidth > cellHeight) ? cellWidth : cellHeight) / 64.0f);
}

void TowerComponent::Move(Vector2 Direction)
{
	m_mapPosition += Direction;
	UpdateMapPosition(m_mapPosition);
}

void TowerComponent::KilledEnemy(int Gold)
{
	m_player->KilledEnemy(Gold);
}

bool TowerComponent::TryPlaceTower()
{
	if (m_pMap->CanAddToMap(this))
	{
		m_pMap->AddUnitToMap(this);
		m_gameObject->GetComponent<SpriteComponent>()->SetColor(lazerColour[0], lazerColour[1], lazerColour[2]);
		m_isOnMap = true;
		return true;
	}

	return false;
}

TowerComponent* TowerComponent::Clone()
{
	TowerComponent* newTower = new TowerComponent();
	*newTower = *this;
	return newTower;
}

void TowerComponent::FindTarget()
{
	std::vector<IMapUnit*> unitsInRadius;

	GetRadius(unitsInRadius);

	int furthest = INT_MAX;

	if (!unitsInRadius.empty())
	{
		for (auto enemy : unitsInRadius)
		{
			int distance = MapUtility::Heuristic(m_mapPosition, enemy->GetMapPosition());
			if (distance < furthest)
			{
				m_currentTarget = dynamic_cast<EnemyComponent*>(enemy);
				furthest = distance;
			}
		}
	}
	else
	{
		m_currentTarget = nullptr;
	}
}

bool TowerComponent::CanFire()
{
	if (m_currentTarget != nullptr)
	{
		if ((float)m_attackCounter * m_attackSpeed > (float)m_attackCooldown)
		{
			m_attackCounter = 0;
			return true;
		}
	}

	return false;
}

void TowerComponent::Fire()
{
	App::PlaySound(".\\TestData\\Test.wav");
	m_hasFired = true;
	m_currentTarget->TakeDamage(m_damage, this);
}

std::vector<IMapUnit*> TowerComponent::GetLine(int x1, int y1, int x2, int y2)
{
	std::vector<IMapUnit*> line;

	// Swap first and second if second < first
	if (x2 < x1)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y2 < y1)
	{
		int temp = y1;
		y1 = y2;
		y2 = temp;
	}

	// Check if line is horizontal or vertical
	if (x1 == x2) {
		// Vertical line
		// Get starting and ending if not within bounds
		if (x1 > m_pMap->GetWidth() - 1 || x1 < 0)
			return line;
		if (y1 < 0) y1 = 0;
		if (y2 > m_pMap->GetHeight()) y2 = m_pMap->GetHeight();

		for (int i = y1; i < y2; i++)
		{
			std::vector<IMapUnit*> AllUnitsAt;
			m_pMap->GetAllUnitsOfType<EnemyComponent>(Vector2(x1, i), AllUnitsAt);
			line.insert(line.end(), AllUnitsAt.begin(), AllUnitsAt.end());
		}
	}
	else if (y1 == y2) {
		// Horizontal line
		// Get starting and ending if not within bounds
		if (y1 > m_pMap->GetHeight() - 1 || y1 < 0)
			return line;
		if (x1 < 0) x1 = 0;
		if (x2 > m_pMap->GetWidth()) x2 = m_pMap->GetWidth();

		for (int i = x1; i < x2; i++)
		{
			std::vector<IMapUnit*> AllUnitsAt;
			m_pMap->GetAllUnitsOfType<EnemyComponent>(Vector2(i, y1), AllUnitsAt);
			line.insert(line.end(), AllUnitsAt.begin(), AllUnitsAt.end());
		}
	}

	return line;
}

void TowerComponent::GetRadius(std::vector<IMapUnit*>& Units)
{
	std::vector<IMapUnit*> units = GetLine(m_mapPosition.x, m_mapPosition.y - m_damageRadius, m_mapPosition.x, m_mapPosition.y + m_damageRadius + 1);
	Units.insert(Units.end(), units.begin(), units.end());

	for (int i = m_damageRadius - 1; i >= 0; i--)
	{
		units = GetLine(m_mapPosition.x - (m_damageRadius - i), m_mapPosition.y - i, m_mapPosition.x - (m_damageRadius - i), m_mapPosition.y + i + 1);
		Units.insert(Units.end(), units.begin(), units.end());
		units = GetLine(m_mapPosition.x + m_damageRadius - i, m_mapPosition.y - i, m_mapPosition.x + m_damageRadius - i, m_mapPosition.y + i + 1);
		Units.insert(Units.end(), units.begin(), units.end());
	}
}

void Bullet::ReachedTarget()
{
}
