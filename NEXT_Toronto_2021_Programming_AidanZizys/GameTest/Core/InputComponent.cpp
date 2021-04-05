#include "stdafx.h"

#include "InputComponent.h"

#include "../App/app.h"

void InputComponent::OnDetach()
{
	for (auto keyval : m_mappedActions)
	{
		SAFE_DELETE(keyval.second);
	}
}

void InputComponent::Update(float deltaTime)
{
	for (auto KeyValPair : m_mappedActions)
	{
		if (App::IsKeyPressed(KeyValPair.first))
			KeyValPair.second->Execute();

		if (App::GetController().CheckButton(KeyValPair.first))
			KeyValPair.second->Execute();
	}
}