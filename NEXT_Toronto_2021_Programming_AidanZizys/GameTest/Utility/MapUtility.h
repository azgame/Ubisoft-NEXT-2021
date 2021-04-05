#ifndef MAPUTILITY_H
#define MAPUTILITY_H

#include "../App/AppSettings.h"
#include "../Core/Vector.h"

namespace MapUtility
{
	extern Vector2 ConvertMapSpaceToAppVirtual(Vector2 Position, int Width, int Height);
	
	extern int Heuristic(Vector2 A, Vector2 B); // Simple distance heuristic	
}

#endif