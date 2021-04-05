#ifndef ASTAR_H
#define ASTAR_H

#include "../Game/Map.h"
#include "../Utility/MapUtility.h"
#include "Vector.h"

#include <queue>

template<typename T, typename P>
struct PriorityQueue 
{
	typedef std::pair<P, T> Node;
	std::priority_queue<Node, std::vector<Node>,
		std::greater<Node>> elements;

	inline bool empty() const 
	{
		return elements.empty();
	}

	inline void put(T item, P priority) 
	{
		elements.emplace(priority, item);
	}

	inline T get() 
	{
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}

	inline int size()
	{
		return elements.size();
	}
};

class AStar
{
public:

	void Search(Map* pMap,
		Vector2 Start,
		Vector2 Goal,
		std::unordered_map<Vector2, Vector2>& CameFrom,
		std::unordered_map<Vector2, int>& CostSoFar);

	std::queue<Vector2> ReconstructPath(
		Vector2 Start,
		Vector2 Goal,
		std::unordered_map<Vector2, Vector2>& CameFrom);
};

inline void AStar::Search(Map* pMap, Vector2 Start, Vector2 Goal, std::unordered_map<Vector2, Vector2>& CameFrom, std::unordered_map<Vector2, int>& CostSoFar)
{
	PriorityQueue<Vector2, int> frontier;
	frontier.put(Start, 0);

	CameFrom[Start] = Start;

	CostSoFar[Start] = 0;

	std::queue<Vector2> q = std::queue<Vector2>();
	Vector2 next;

	while (!frontier.empty()) 
	{
		Vector2 current = frontier.get();

		if (current == Goal)
			break;

		// Fill q with neighbours
		// Only towers have space exclusivity, so they'll a) be the only thing in a cell, and b) will return true to HasSpaceExclusivity()
		if (pMap->GetSingleUnitAt(Vector2(current.x + 1, current.y)) == nullptr || 
			!pMap->GetSingleUnitAt(Vector2(current.x + 1, current.y))->HasSpaceExclusivity())
			q.push(Vector2(current.x + 1, current.y));

		if (pMap->GetSingleUnitAt(Vector2(current.x - 1, current.y)) == nullptr ||
			!pMap->GetSingleUnitAt(Vector2(current.x - 1, current.y))->HasSpaceExclusivity())
			q.push(Vector2(current.x - 1, current.y));

		if (pMap->GetSingleUnitAt(Vector2(current.x, current.y + 1)) == nullptr ||
			!pMap->GetSingleUnitAt(Vector2(current.x, current.y + 1))->HasSpaceExclusivity())
			q.push(Vector2(current.x, current.y + 1));

		if (pMap->GetSingleUnitAt(Vector2(current.x, current.y - 1)) == nullptr ||
			!pMap->GetSingleUnitAt(Vector2(current.x, current.y - 1))->HasSpaceExclusivity())
			q.push(Vector2(current.x, current.y - 1));

		// For each neighbour, find out if map location gets closer to goal
		// If it does, add it to queue with priority based on heuristic and accumulated movement cost
		while (!q.empty())
		{
			next = q.front(); q.pop();

			int new_cost = CostSoFar[current] + 1;

			if (CostSoFar.find(next) == CostSoFar.end() || new_cost < CostSoFar[next])
			{
				CostSoFar[next] = new_cost;
				int priority = new_cost + MapUtility::Heuristic(next, Goal);
				frontier.put(next, priority);
				CameFrom[next] = current;
			}
		}
	}
}

inline std::queue<Vector2> AStar::ReconstructPath(Vector2 Start, Vector2 Goal, std::unordered_map<Vector2, Vector2>& CameFrom)
{
	std::vector<Vector2> returnPath;
	Vector2 current = Goal;

	bool pathExists = true;

	while (current != Start)
	{
		returnPath.push_back(current);
		current = CameFrom[current];

		if (returnPath.size() > 1000)
		{
			pathExists = false;
			break;
		}
	}

	std::reverse(returnPath.begin(), returnPath.end());

	std::queue<Vector2> path;

	if (pathExists)
	{
		for (int i = 0; i < returnPath.size(); i++)
			path.push(returnPath[i]);
	}

	return path;
}


#endif