#include "stdafx.h"
#include "Pathfinding.h"

#include "../Core/AStar.h"

#include "../Utility/MapUtility.h"

Pathfinding& Pathfinding::GetInstance()
{
	static Pathfinding instance;
	return instance;
}

bool Pathfinding::TryMakePath(Vector2 Start, Vector2 End, std::queue<Vector2>& Path)
{
	m_cameFrom.clear();
	m_costSoFar.clear();

	astar.Search(m_pMap, Start, End, m_cameFrom, m_costSoFar);
	Path = astar.ReconstructPath(Start, End, m_cameFrom);
	if (!Path.empty())
	{
		return true;
	}

	return false;
}

bool Pathfinding::CanUpdatePath()
{
	m_cameFrom.clear();
	m_costSoFar.clear();

	astar.Search(m_pMap, start->GetMapPosition(), end->GetMapPosition(), m_cameFrom, m_costSoFar);
	tempPath = astar.ReconstructPath(start->GetMapPosition(), end->GetMapPosition(), m_cameFrom);
	if (!tempPath.empty())
	{
		return true;
	}

	return false;
}

void Pathfinding::UpdatePath()
{
	while (!m_path->empty()) m_path->pop();

	*m_path = tempPath;

	Notify();
}

void Pathfinding::DrawPath()
{
	std::queue<Vector2> drawPath = *m_path;

	Vector2 previous = start->GetMapPosition(); drawPath.pop();

	while (!drawPath.empty())
	{
		Vector2 current = drawPath.front(); drawPath.pop();

		Vector2 first = MapUtility::ConvertMapSpaceToAppVirtual(previous, m_pMap->GetWidth(), m_pMap->GetHeight());
		Vector2 second = MapUtility::ConvertMapSpaceToAppVirtual(current, m_pMap->GetWidth(), m_pMap->GetHeight());

		App::DrawLine(first.x, first.y, second.x, second.y);

		previous = current;
	}
}
