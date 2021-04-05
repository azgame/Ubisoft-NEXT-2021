#ifndef MAPUNIT_H
#define MAPUNIT_H

#include "../Core/Vector.h"

#include "../Core/Component.h"

class Map;

class IMapUnit : public Component
{
public:

	virtual void OnAttach(GameObject* pParent) override { m_gameObject = pParent; };
	virtual void OnDetach() override { m_pMap = nullptr; };
	virtual void OnStart() override {};
	virtual void Update(float deltaTime) override {};
	virtual void Render() override {};

	virtual void UpdateMapPosition(Vector2 Position) = 0;
	virtual void UpdatePosition() = 0;
	virtual void Move(Vector2 Position) = 0;

	std::string GetName() { return m_name; }

	virtual void Initialize(Map* pMap, Vector2 Position, int Width, int Height, bool Exclusive)
	{
		m_pMap = pMap;
		m_mapPosition = Position;
		m_width = Width;
		m_height = Height;
		m_exclusiveSpace = Exclusive;
	}

	bool HasSpaceExclusivity() { return m_exclusiveSpace; }
	Vector2 GetMapPosition() { return m_mapPosition; }
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

protected:

	Map* m_pMap;

	Vector2 m_mapPosition;
	int m_width, m_height;

	bool m_isOnMap;
	bool m_exclusiveSpace;

	std::string m_name;
};

#endif