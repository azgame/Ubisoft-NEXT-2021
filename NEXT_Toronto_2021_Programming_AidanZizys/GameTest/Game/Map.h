#ifndef MAP_H
#define MAP_H

#include "MapUnit.h"
#include "../Core/Vector.h"

#include <unordered_map>

class MapUnitDispatcher
{
public:
	void Dispatch(IMapUnit* Unit)
	{
		Vector2 pos = Unit->GetMapPosition(); __debugbreak(); // Debug
	}
};

class MapWall : public IMapUnit
{
public:

	void UpdateMapPosition(Vector2 Position) override {};
	void UpdatePosition() override {};
	void Move(Vector2 Destination) override {};
};

// Defining custom hash to use Vector2 as key in unordered map
namespace std
{
	template <>
	struct hash<Vector2>
	{
		std::size_t operator()(const Vector2& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return ((hash<float>()(k.x)
				^ (hash<float>()(k.y) << 1)) >> 1);
		}
	};
}

class Map
{
public:

	Map(unsigned int CellSize);
	~Map() { SAFE_DELETE(wall); }

	bool CanAddToMap(IMapUnit* Unit);
	void AddUnitToMap(IMapUnit* Unit);
	void RemoveUnitFromMap(IMapUnit* Unit);

	IMapUnit* GetSingleUnitAt(Vector2 Position);
	void GetAllUnitsAt(Vector2 Position, std::vector<IMapUnit*>& Map);

	void GetAllUnitsAcross(std::vector<Vector2> Positions, std::vector<IMapUnit*>& Map);

	template <typename T>
	void GetAllUnitsOfType(Vector2 Position, std::vector<IMapUnit*>& Map);

	template <typename T>
	void GetAllUnitsOfTypeAcross(std::vector<Vector2> Positions, std::vector<IMapUnit*>& Map);

	void Dispatch(MapUnitDispatcher* Dispatcher)
	{
		for (auto tile : map)
		{
			for (auto unit : tile.second)
			{
				Dispatcher->Dispatch(unit);
			}
		}
	}

	void Draw();

	unsigned int GetWidth() { return m_width; }
	unsigned int GetHeight() { return m_height; }

private:

	MapWall* wall;

	unsigned int m_width, m_height;
	float m_cellSize;

	std::unordered_map<Vector2, std::vector<IMapUnit*>> map;
};

template<typename T>
inline void Map::GetAllUnitsOfType(Vector2 Position, std::vector<IMapUnit*>& Map)
{
	if (!map[Position].empty())
	{
		for (auto unit : map[Position])
			if (dynamic_cast<T*>(unit))
				Map.push_back(unit);
	}
}

template<typename T>
inline void Map::GetAllUnitsOfTypeAcross(std::vector<Vector2> Positions, std::vector<IMapUnit*>& Map)
{
	for (auto pos : Positions)
	{
		for (auto unit : map[pos])
			if (dynamic_cast<T*>(unit))
				Map.push_back(unit);
	}
}

#endif