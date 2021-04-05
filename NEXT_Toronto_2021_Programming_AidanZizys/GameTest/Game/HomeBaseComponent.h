#ifndef HOMEBASECOMPONENT_H
#define HOMEBASECOMPONENT_H

#include "MapUnit.h"
#include "../Core/Observer.h"

class HomeBaseComponent : public IMapUnit, public Subject, public Observer
{
public:

	HomeBaseComponent() {}

	void Update(float deltaTime) override;
	void Render() override;

	void UpdateMapPosition(Vector2 Position) override;
	void UpdatePosition() override;
	void Move(Vector2 Destination) override {};
	void SetHealth(int Health) { m_health = Health; }

	void TakeDamage(int Damage) 
	{
		m_health -= Damage;
		if (m_health <= 0)
			Notify();
	}

	void OnNotify() override
	{
		m_health += 10;
	}

private:

	int m_health;
};

#endif