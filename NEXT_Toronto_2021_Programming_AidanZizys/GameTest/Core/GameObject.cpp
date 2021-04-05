#include "stdafx.h"

#include "GameObject.h"

#include "Component.h"

#include "SceneObjects.h"


GameObject::GameObject(std::string tag_)
{
	m_tag = tag_;

	isInitialized = false;
	markedForDeletion = false;

	SceneObjects::GetInstance().AddGameObject(this);
}

GameObject::~GameObject()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->OnDetach();
		delete m_components[i];
	}

	m_components.clear();
}

void GameObject::OnStart()
{
	if (!isInitialized)
	{
		for (auto component : m_components)
			component->OnStart();

		isInitialized = true;
	}
}

void GameObject::Destroy()
{
	SceneObjects::GetInstance().RemoveGameObject(this);

	/*for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->OnDetach();
		delete m_components[i];
	}

	m_components.clear();*/
}

void GameObject::Update(float deltaTime)
{
	for (auto component : m_components)
		component->Update(deltaTime);
}

void GameObject::Render()
{
	for (auto component : m_components)
		component->Render();
}

void GameObject::CreateAttachedComponent(Component* pAttachedComponent)
{
	// Attach T
	pAttachedComponent->OnAttach(this); // On attach allows implementations to define how they register to systems.  

	m_components.push_back(pAttachedComponent);
}