#include "stdafx.h"
#include "Action.h"

#include "../Game/MapUnit.h"

MoveAction::MoveAction(IMapUnit* pTarget, float xDir, float yDir)
{
	m_target = pTarget;
	x = xDir;
	y = yDir;
}

void MoveAction::Execute()
{
	Vector2 pos = m_target->GetMapPosition();
	pos.x += x;
	pos.y += y;

	m_target->Move(Vector2(x, y));
}

MutableAction::MutableAction(std::function<void(void)> Func)
{
	execute = Func;
}

void MutableAction::Execute()
{
	execute();
}
