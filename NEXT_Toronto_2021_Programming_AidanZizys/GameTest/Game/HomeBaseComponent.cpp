#include "stdafx.h"
#include "HomeBaseComponent.h"

#include "Map.h"

void HomeBaseComponent::Update(float deltaTime)
{
}

void HomeBaseComponent::Render()
{
	std::string health = "Base health: " + std::to_string(m_health);
	App::Print(850, 90, health.c_str());
}

void HomeBaseComponent::UpdateMapPosition(Vector2 Position)
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

void HomeBaseComponent::UpdatePosition()
{
	float cellWidth = APP_VIRTUAL_WIDTH / m_pMap->GetWidth();
	float cellHeight = APP_VIRTUAL_HEIGHT / m_pMap->GetHeight();

	Vector2 snappedPosition;

	snappedPosition.x = (cellWidth * (m_mapPosition.x)) + m_width * (cellWidth / 2);
	snappedPosition.y = (cellHeight * (m_mapPosition.y)) + m_height * (cellHeight / 2);

	m_gameObject->SetPosition(snappedPosition);
	m_gameObject->SetScale(((m_width > m_height) ? m_width : m_height) * ((cellWidth > cellHeight) ? cellWidth : cellHeight) / 64.0f);
}
