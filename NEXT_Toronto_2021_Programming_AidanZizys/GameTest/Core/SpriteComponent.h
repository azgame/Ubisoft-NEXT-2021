#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "Component.h"

#include "../App/SimpleSprite.h"

class SpriteComponent : public Component
{
public:
	void OnAttach(GameObject* pParent) override { m_gameObject = pParent; };
	void OnDetach() override;
	void OnStart() override;
	void Update(float deltaTime) override;
	void Render() override;

	void LoadSprite(const char* pFileName, UINT nColumns = 1u, UINT nRows = 1u);

	float GetWidth() const { return m_sprite->GetWidth(); }
	float GetHeight() const { return m_sprite->GetHeight(); }

	void SetFrame(UINT f) { m_sprite->SetFrame(f); }
	UINT GetFrame() { return m_sprite->GetFrame(); }

	void SetColor(float r, float g, float b, float a = 1.0f) { m_sprite->SetColor(r, g, b, a); }

	void CreateAnimation(unsigned int id, float speed, const std::vector<int>& frames) 
		{ m_sprite->CreateAnimation(id, speed, frames); }

	void SetAnimation(int id) { m_sprite->SetAnimation(id); }

	void SetVertex(UINT i, float v) { m_sprite->SetVertex(i, v); }
	float GetVertex(UINT i) { m_sprite->GetVertex(i); }

	SpriteComponent* Clone();

private:

	CSimpleSprite* m_sprite;
};


#endif