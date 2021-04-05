#include "stdafx.h"
#include "PlayerComponent.h"

#include "TowerComponent.h"
#include "../Core/SpriteComponent.h"
#include "../Core/InputComponent.h"

#include "../Core/SceneObjects.h"

void PlayerComponent::OnDetach()
{
	SAFE_DELETE(m_pTowerFactory);
	SAFE_DELETE(m_pTowerSelector);
	SAFE_DELETE(m_pPlacedTower);
}

void PlayerComponent::OnStart()
{
	InputComponent* input = m_gameObject->GetComponent<InputComponent>();
	input->SetInputAction(XINPUT_GAMEPAD_RIGHT_SHOULDER, new MutableAction(std::function<void(void)>([this](void) { return m_pTowerSelector->Next(); }))); // function <return(args...)>(lambda->[capture list](decltype(args)) { return lambda function; })
	input->SetInputAction(XINPUT_GAMEPAD_LEFT_SHOULDER, new MutableAction(std::function<void(void)>([this](void) { return m_pTowerSelector->Previous(); })));
	input->SetInputAction(XINPUT_GAMEPAD_X, new MutableAction(std::function<void(void)>([this](void) { return this->TryReroll(); })));
}

void PlayerComponent::Update(float deltaTime)
{

	if (m_pPlacedTower == nullptr && App::GetController().CheckButton(XINPUT_GAMEPAD_Y))
	{
		if (m_gold >= m_pTowerSelector->SelectTower()->GetComponent<TowerComponent>()->GetCost())
		{
			m_pPlacedTower = new GameObject();
			m_pPlacedTower->AddComponent<TowerComponent>(m_pTowerSelector->SelectTower()->GetComponent<TowerComponent>()->Clone());
			m_pPlacedTower->AddComponent<SpriteComponent>(m_pTowerSelector->SelectTower()->GetComponent<SpriteComponent>()->Clone());
			m_pPlacedTower->GetComponent<TowerComponent>()->UpdateMapPosition(Vector2(m_pMap->GetWidth() / 2, m_pMap->GetHeight() / 2)); // Put in middle

			m_pTowerSelector->ToggleLock();

			SceneObjects::GetInstance().InitObjects();
		}
	}
	else if (m_pPlacedTower != nullptr)
	{
		if (!m_pPlacedTower->HasComponent<InputComponent>())
		{
			m_pPlacedTower->AddComponent<InputComponent>();
			IMapUnit* mapUnit = m_pPlacedTower->GetComponent<TowerComponent>();
			m_pPlacedTower->GetComponent<InputComponent>()->SetInputAction(XINPUT_GAMEPAD_DPAD_LEFT, new MoveAction(mapUnit, -1, 0));
			m_pPlacedTower->GetComponent<InputComponent>()->SetInputAction(XINPUT_GAMEPAD_DPAD_RIGHT, new MoveAction(mapUnit, 1, 0));
			m_pPlacedTower->GetComponent<InputComponent>()->SetInputAction(XINPUT_GAMEPAD_DPAD_UP, new MoveAction(mapUnit, 0, 1));
			m_pPlacedTower->GetComponent<InputComponent>()->SetInputAction(XINPUT_GAMEPAD_DPAD_DOWN, new MoveAction(mapUnit, 0, -1));
		}

		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A))
		{
			if (m_pPlacedTower->GetComponent<TowerComponent>()->TryPlaceTower())
			{
				Notify();

				m_gold -= m_pPlacedTower->GetComponent<TowerComponent>()->GetCost();
				m_pPlacedTower->GetComponent<TowerComponent>()->SetPlayer(this);
				m_pPlacedTower->RemoveComponent<InputComponent>();
				m_pPlacedTower = nullptr;
				m_pTowerSelector->PlaceSelectedTower();

				m_pTowerSelector->ToggleLock();
			}
		}
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_B))
		{
			m_pPlacedTower->MarkForDeletion();
			m_pPlacedTower = nullptr;

			m_pTowerSelector->ToggleLock();
		}
	}
}

void PlayerComponent::Render()
{
	std::string gold = "Gold: " + std::to_string(m_gold);
	App::Print(900, 50, gold.c_str());
	App::Print(900, 10, "Reroll: 5");
	m_pTowerSelector->Draw();
}

void PlayerComponent::Initialize(Map* pMap)
{
	m_pMap = pMap;
	m_pTowerFactory = new TowerFactory(pMap);
	m_pTowerSelector = new TowerSelector(m_pTowerFactory);
}

void PlayerComponent::OnNotify()
{

#ifdef RELEASE
	
#else //--Debug
	BREAKPOINT() // Game end
#endif

	exit(0);
	
}