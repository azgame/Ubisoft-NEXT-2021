#ifndef ACTION_H
#define ACTION_H

#include "GameObject.h"

#include <functional>

class IMapUnit;

class Action
{
public:
	virtual void Execute() = 0;
};

class MoveAction : public Action
{
public:
	MoveAction(IMapUnit* pTarget, float xDir, float yDir);
	void Execute() override;
private:
	IMapUnit* m_target;
	float x, y;
};

class RotateAction : public Action
{
public:
	RotateAction(GameObject* pTarget, float Rotation) {
		m_target = pTarget, rotation = Rotation; }

	void Execute() override { m_target->SetRotation(m_target->GetRotation() + rotation); }
private:
	GameObject* m_target;
	float rotation;
};

class MutableAction : public Action
{
public:
	MutableAction(std::function<void(void)> Func);

	void Execute() override;
private:
	std::function<void(void)> execute;
};

// Game Actions

#endif