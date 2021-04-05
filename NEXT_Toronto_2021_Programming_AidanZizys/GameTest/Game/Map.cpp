#include "stdafx.h"
#include "Map.h"

#include "../App/AppSettings.h"

#include "Pathfinding.h"

Map::Map(unsigned int CellSize)
{
	m_cellSize = CellSize;
	m_width = (APP_VIRTUAL_WIDTH / CellSize);
	m_height = (APP_VIRTUAL_HEIGHT / CellSize);

	for (int h = 0; h < m_height; h++)
	{
		for (int w = 0; w < m_width; w++)
		{
			map.emplace(Vector2(w, h), std::vector<IMapUnit*>());
		}
	}

	// Cheeky way of getting walls around the boundaries
	wall = new MapWall();
	wall->Initialize(this, Vector2(0, 0), 1, 1, true);

	for (int w = 0; w < m_width; w++)
	{
		map[Vector2(w, 1)].push_back(wall);
		map[Vector2(w, m_height)].push_back(wall);
	}

	for (int h = 1; h < m_height; h++)
	{
		map[Vector2(-1, h)].push_back(wall);
		map[Vector2(m_width, h)].push_back(wall);
	}
}

bool Map::CanAddToMap(IMapUnit* Unit)
{
	Vector2 Position = Unit->GetMapPosition();

	for (unsigned int h = 0; h < Unit->GetHeight(); h++)
	{
		for (unsigned int w = 0; w < Unit->GetWidth(); w++)
		{
			Vector2 tempPos = Vector2(Position.x + w, Position.y + h);

			if (!map[tempPos].empty())
			{
				// Check to see if we can legally place a new unit here
				if (map[tempPos].front()->HasSpaceExclusivity() ||
					Unit->HasSpaceExclusivity())
				{
					return false;
				}
			}

			map[tempPos].push_back(wall); // temp to check if all paths would be obstructure

			// check to see if it would block the path to the goal
			if (Unit->HasSpaceExclusivity())
			{
				if (!Pathfinding::GetInstance().CanUpdatePath())
				{
					auto it = std::find(map[tempPos].begin(), map[tempPos].end(), wall);
					if (it != map[tempPos].end())
					{
						map[tempPos].erase(it);
					}

					return false;
				}
			}

			auto it = std::find(map[tempPos].begin(), map[tempPos].end(), wall);
			if (it != map[tempPos].end())
			{
				map[tempPos].erase(it);
			}
		}
	}

	return true;
}

void Map::AddUnitToMap(IMapUnit* Unit)
{
	Vector2 Position = Unit->GetMapPosition();

	for (unsigned int h = 0; h < Unit->GetHeight(); h++)
	{
		for (unsigned int w = 0; w < Unit->GetWidth(); w++)
		{
			Vector2 newPos = Vector2(Position.x + w, Position.y + h);
			map[newPos].push_back(Unit);
		}
	}
}

void Map::RemoveUnitFromMap(IMapUnit* Unit)
{
	for (unsigned int h = 0; h < Unit->GetHeight(); h++)
	{
		for (unsigned int w = 0; w < Unit->GetWidth(); w++)
		{
			Vector2 unitPos = Vector2(Unit->GetMapPosition().x + w, Unit->GetMapPosition().y + h);

			auto it = std::find(map[unitPos].begin(), map[unitPos].end(), Unit);
			if (it != map[unitPos].end())
			{
				map[unitPos].erase(it);
			}
		}
	}
}

IMapUnit* Map::GetSingleUnitAt(Vector2 Position)
{
	if (!map[Position].empty())
		return map[Position].front();

	return nullptr;
}

void Map::GetAllUnitsAt(Vector2 Position, std::vector<IMapUnit*>& Map)
{
	Map = map[Position];
}

void Map::GetAllUnitsAcross(std::vector<Vector2> Positions, std::vector<IMapUnit*>& Map)
{
	for (auto pos : Positions)
		Map.insert(Map.end(), map[pos].begin(), map[pos].end());
}

void Map::Draw()
{
	float cellWidth = APP_VIRTUAL_WIDTH / m_width;
	float cellHeight = APP_VIRTUAL_HEIGHT / m_height;

	for (unsigned int h = 2; h < m_height; h++)
	{
		App::DrawLine(0, (h * cellHeight), m_width * cellWidth, (h * cellHeight));
	}

	for (unsigned int w = 0; w < m_width; w++)
	{
		App::DrawLine((w * cellWidth) + cellWidth, cellHeight * 2, (w * cellWidth) + cellWidth, APP_VIRTUAL_HEIGHT);
	}
}
