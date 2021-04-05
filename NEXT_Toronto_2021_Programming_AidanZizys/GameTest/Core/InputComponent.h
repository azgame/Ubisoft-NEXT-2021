#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "Component.h"
#include "Action.h"

#include <unordered_map>

class InputComponent : public Component
{
public:
	void OnAttach(GameObject* pParent) override { m_gameObject = pParent; };
	void OnDetach() override;
	void OnStart() override {};
	void Update(float deltaTime) override;
	void Render() override {};

	void SetInputAction(int Key, Action* pAction);

private:

	std::unordered_map<int, Action*> m_mappedActions;
};

inline void InputComponent::SetInputAction(int Key, Action* pAction)
{
	m_mappedActions[Key] = pAction;
}

#endif