#include "stdafx.h"
#include "MapUtility.h"

Vector2 MapUtility::ConvertMapSpaceToAppVirtual(Vector2 Position, int Width, int Height)
{
	
	float cellWidth = APP_VIRTUAL_WIDTH / Width;
	float cellHeight = APP_VIRTUAL_HEIGHT / Height;

	Vector2 snappedPosition;

	snappedPosition.x = (cellWidth * (Position.x)) + (cellWidth / 2);
	snappedPosition.y = (cellHeight * (Position.y)) + (cellHeight / 2);

	return snappedPosition;
	
}

int MapUtility::Heuristic(Vector2 A, Vector2 B)
{
	return std::abs(A.x - B.x) + std::abs(A.y - B.y);
}
