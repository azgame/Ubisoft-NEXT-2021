#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "../Core/Component.h"
#include "../Core/Observer.h"

#include "Map.h"
#include "TowerFactory.h"
#include "TowerSelector.h"

class PlayerComponent : public Component, public Subject, public Observer
{
public:

	PlayerComponent() : m_gold(20) {}

	void OnAttach(GameObject* pParent) override { m_gameObject = pParent; };
	void OnDetach() override;
	void OnStart() override;
	void Update(float deltaTime) override;
	void Render() override;

	void Initialize(Map* pMap);

	void KilledEnemy(int Gold) { m_gold += Gold; }

	void OnNotify() override;

private:

	void TryReroll()
	{
		if (m_gold >= 5)
		{
			m_pTowerSelector->RerollSelection();
			m_gold -= 5;
		}
	}

	int m_gold;

	Map* m_pMap;
	TowerFactory* m_pTowerFactory;
	TowerSelector* m_pTowerSelector;
	GameObject* m_pPlacedTower;
};

#endif