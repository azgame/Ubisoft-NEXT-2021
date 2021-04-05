#include "stdafx.h"
#include "SpriteComponent.h"

void SpriteComponent::OnDetach()
{
	SAFE_DELETE(m_sprite);
}

void SpriteComponent::OnStart()
{
}

void SpriteComponent::Update(float deltaTime)
{
	Vector2 pos = m_gameObject->GetPosition();
	m_sprite->SetPosition(pos.x, pos.y);
	m_sprite->SetScale(m_gameObject->GetScale());
	m_sprite->SetAngle(m_gameObject->GetRotation());

	m_sprite->Update(deltaTime);
}

void SpriteComponent::Render()
{
	m_sprite->Draw();
}

void SpriteComponent::LoadSprite(const char* pFileName, UINT nColumns, UINT nRows)
{
	SAFE_DELETE(m_sprite);

	m_sprite = new CSimpleSprite(pFileName, nColumns, nRows);
}

SpriteComponent* SpriteComponent::Clone()
{
	SpriteComponent* newSprite = new SpriteComponent();
	newSprite->m_sprite = new CSimpleSprite(*m_sprite);
	return newSprite;
}
