#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "../Core/Vector.h"
#include "../Core/Observer.h"

#include "../Core/AStar.h"

#include "SpawnComponent.h"
#include "HomeBaseComponent.h"

#include <queue>
#include <unordered_map>

class Map;

class Pathfinding : public Subject, public Observer
{
public:

	static Pathfinding& GetInstance();

	Pathfinding() {}
	~Pathfinding()
	{
		start = nullptr;
		end = nullptr;
		m_pMap = nullptr;

		while (!m_path->empty()) m_path->pop();
		SAFE_DELETE(m_path);
	}

	void Initialize(Map* pMap, SpawnComponent* Start, HomeBaseComponent* End)
	{
		m_pMap = pMap;
		start = Start;
		end = End;

		m_path = new std::queue<Vector2>();

		if (CanUpdatePath())
			UpdatePath();
	}

	bool TryMakePath(Vector2 Start, Vector2 End, std::queue<Vector2>& Path);
	bool CanUpdatePath();
	void UpdatePath();
	void DrawPath();

	std::queue<Vector2>* GetPath() { return m_path; }
	Vector2 GetGoal() { return end->GetMapPosition(); }

	inline bool IsPathEmpty()
	{
		return m_path->empty();
	}

	void OnNotify() override
	{
		UpdatePath();
	}

private:

	SpawnComponent* start;
	HomeBaseComponent* end;

	Map* m_pMap;

	AStar astar;
	std::unordered_map<Vector2, Vector2> m_cameFrom;
	std::unordered_map<Vector2, int> m_costSoFar;
	std::queue<Vector2>* m_path;
	std::queue<Vector2> tempPath;

};

#endif