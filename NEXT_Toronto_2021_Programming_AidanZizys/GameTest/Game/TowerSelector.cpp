#include "stdafx.h"
#include "TowerSelector.h"

void Selector::Draw()
{
	// temp
	int xPadding = 40;
	int sWidth = 100;
	int initWidth = 150;
	int sHeight = 10;
	int eHeight = 90;

	App::DrawLine(initWidth + (xPadding * index) + (sWidth * index), sHeight, initWidth + (xPadding * index) + (sWidth * index) + sWidth, sHeight);
	App::DrawLine(initWidth + (xPadding * index) + (sWidth * index) + sWidth, sHeight, initWidth + (xPadding * index) + (sWidth * index) + sWidth, eHeight);
	App::DrawLine(initWidth + (xPadding * index) + (sWidth * index), eHeight, initWidth + (xPadding * index) + (sWidth * index) + sWidth, eHeight);
	App::DrawLine(initWidth + (xPadding * index) + (sWidth * index), sHeight, initWidth + (xPadding * index) + (sWidth * index), eHeight);
}

void Selector::Next()
{
	index = (index + 1) % maxSelections;
}

void Selector::Previous()
{
	index--;

	if (index < 0)
		index = maxSelections - 1;
}

TowerSelector::TowerSelector(TowerFactory* pTowerFactory) : selector(5)
{
	lock = false;

	towerFactory = pTowerFactory;

	m_maxSelections = 5;

	m_availableTowers.resize(m_maxSelections);

	for (int i = 0; i < m_maxSelections; i++)
	{
		m_availableTowers[i] = towerFactory->BuildTower(TowerIDs::BASIC_TOWER);
		m_availableTowers[i]->SetPosition(Vector2(initWidth + (width * i), height));
		m_availableTowers[i]->SetScale(0.4f);
	}

	// Polish with tower randomization
	m_upcomingTowers.push(towerFactory->BuildTower((int)FRAND_RANGE(0, TowerIDs::COUNT)));
}

void TowerSelector::Draw()
{
	DrawSelector();
	DrawTowers();
}

void TowerSelector::PlaceSelectedTower()
{
	m_availableTowers[selector.GetIndex()]->MarkForDeletion();

	m_availableTowers[selector.GetIndex()] = nullptr;

	FillAvailableTowers(selector.GetIndex());
}

void TowerSelector::RerollSelection()
{
	if (!lock)
	{
		for (int i = 0; i < m_maxSelections; i++)
		{
			PlaceSelectedTower();
			selector.Next();
		}
	}
}

void TowerSelector::DrawSelector()
{
	selector.Draw();
}

void TowerSelector::DrawTowers()
{
	for (int i = 0; i < m_maxSelections; i++)
	{
		if (m_availableTowers[i])
		{
			App::Print(initWidth + ((width * i) - width / 3), height / 3, m_availableTowers[i]->GetComponent<TowerComponent>()->GetName().c_str());
			App::Print(initWidth + ((width * i) + width / 6), height / 3, std::to_string(m_availableTowers[i]->GetComponent<TowerComponent>()->GetCost()).c_str());
		}
	}	
}

// Polish with tower randomization
void TowerSelector::FillAvailableTowers(int Index)
{
	if (m_upcomingTowers.empty())
	{
		m_upcomingTowers.push(towerFactory->BuildTower((int)FRAND_RANGE(0, TowerIDs::COUNT)));
	}

	m_availableTowers[Index] = m_upcomingTowers.front(); m_upcomingTowers.pop();

	Vector2 pos = Vector2(initWidth + (width * Index), height);

	m_availableTowers[Index]->SetPosition(pos);
	m_availableTowers[Index]->SetScale(0.4f);

	m_upcomingTowers.push(towerFactory->BuildTower((int)FRAND_RANGE(0, TowerIDs::COUNT)));
}
