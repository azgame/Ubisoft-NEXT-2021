#include "stdafx.h"
#include "SpawnComponent.h"

#include "Map.h"
#include "EnemyComponent.h"
#include "Pathfinding.h"

void SpawnComponent::Update(float deltaTime)
{
	spawnCounter++;

	if (spawnCounter > spawnCooldown)
	{
		if (!intermissionEnd)
		{
			spawnCounter = 0;
			intermissionEnd = !intermissionEnd;
		}
		else
		{
			spawnCounter = 0;
			spawnCount++;
			spawnCooldown--;

			float cellWidth = APP_VIRTUAL_WIDTH / m_pMap->GetWidth();
			float cellHeight = APP_VIRTUAL_HEIGHT / m_pMap->GetHeight();

			Vector2 snappedPosition;

			snappedPosition.x = (cellWidth * (m_mapPosition.x)) + m_width * (cellWidth / 2);
			snappedPosition.y = (cellHeight * (m_mapPosition.y)) + m_height * (cellHeight / 2);

			GameObject* newEnemy = m_pEnemyFactory->BuildEnemy((int)FRAND_RANGE(0, EnemyIDs::COUNT));
			newEnemy->GetComponent<EnemyComponent>()->SetSpawnLocation(m_mapPosition);
			newEnemy->GetComponent<EnemyComponent>()->SetHome(m_home);
			newEnemy->GetComponent<EnemyComponent>()->UpdateMapPosition(m_mapPosition);
			Pathfinding::GetInstance().AddObserver(newEnemy->GetComponent<EnemyComponent>());
			newEnemy->GetComponent<EnemyComponent>()->SetPath(Pathfinding::GetInstance().GetPath());
			newEnemy->SetPosition(snappedPosition);
			newEnemy->SetScale(0.75f);
		}
	}

	if (spawnCount > waveEnd)
	{
		spawnCount = 0;
		waveEnd += 10;
		spawnCooldown = originalSpawnCooldown;
		waveCount++;

		intermissionEnd = !intermissionEnd;

		Notify();
		m_mapPosition = Vector2(0, (int)FRAND_RANGE(2, m_pMap->GetHeight() - 1));
	}
}

void SpawnComponent::Render()
{
	std::string wave = "Wave: " + std::to_string(waveCount);
	App::Print(20, 80, wave.c_str());
}

void SpawnComponent::OnDetach()
{
	m_home = nullptr;
	SAFE_DELETE(m_pEnemyFactory);
}

void SpawnComponent::Initialize(Map* pMap, Vector2 Position, int Width, int Height, bool Exclusive)
{
	IMapUnit::Initialize(pMap, Position, Width, Height, Exclusive);

	m_pEnemyFactory = new EnemyFactory(pMap);
}

void SpawnComponent::UpdateMapPosition(Vector2 Position)
{
	m_pMap->RemoveUnitFromMap(this);

	m_mapPosition = Position;

	if (m_mapPosition.x + m_width > m_pMap->GetWidth())
		m_mapPosition.x = m_pMap->GetWidth() - m_width;

	if (m_mapPosition.x < 0)
		m_mapPosition.x = 0;

	if (m_mapPosition.y + m_height > m_pMap->GetHeight())
		m_mapPosition.y = m_pMap->GetHeight() - m_height;

	if (m_mapPosition.y < 2)
		m_mapPosition.y = 2;

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