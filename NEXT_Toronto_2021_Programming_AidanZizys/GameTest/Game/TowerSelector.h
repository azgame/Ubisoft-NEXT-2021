#ifndef TOWERSELECTOR_H
#define TOWERSELECTOR_H

#include "../Core/GameObject.h"
#include "TowerComponent.h"

#include "TowerFactory.h"

#include <queue>

class Selector
{
public:

	Selector(unsigned int MaxSelections) 
	{ 
		index = 0;
		maxSelections = MaxSelections;
	}

	void Draw();

	void Next();
	void Previous();

	int GetIndex() { return index; }

private:
	unsigned int maxSelections;
	int index;
};

class TowerSelector
{
public:

	TowerSelector(TowerFactory* pTowerFactory);

	// Draw available selections
	void Draw();

	// Move cursor (ie. change current selection)
	void Next() { if (!lock) selector.Next(); }
	void Previous() { if (!lock) selector.Previous(); }
	void ToggleLock() { lock = !lock; }

	// Choose to place current selection
	GameObject* SelectTower() { return m_availableTowers[selector.GetIndex()]; }

	// Place (and purchase) current selection
	void PlaceSelectedTower();

	// Reroll all available selections
	void RerollSelection();

private:

	bool lock;

	// Draw cursor around current selection
	void DrawSelector();

	void DrawTowers();

	// Fill missing selection spot
	void FillAvailableTowers(int Index);

	// Load upcoming towers into queue
	std::queue<GameObject*> m_upcomingTowers;

	// Selection of towers
	std::vector<GameObject*> m_availableTowers;

	int initWidth = 200;
	int width = 140;
	int height = 60;

	int m_maxSelections;

	// Selector
	Selector selector;

	TowerFactory* towerFactory;
};

#endif