#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>

#include "Vector.h"

class Component;

class GameObject
{
public:
	GameObject(std::string Tag = "GameObject");
	~GameObject();

	void OnStart();
	void Destroy();
	virtual void Update(float deltaTime);
	virtual void Render();

	template<typename T>
	void AddComponent(Component* C = nullptr);

	template<typename... Ts>
	void AddComponents();

	template<typename T>
	T* GetComponent();

	template<typename T>
	void RemoveComponent();

	template<typename... Ts>
	void RemoveComponents();

	template<typename T>
	bool HasComponent();

	void SetTag(std::string Tag) { m_tag = Tag; }
	std::string Tag() { return m_tag; }

	void SetPosition(Vector2 Position) { position = Position; }
	void SetScale(float Scale) { scale = Scale; }
	void SetRotation(float Rotation) { rotation = Rotation; }

	Vector2 GetPosition() { return position; }
	float GetScale () { return scale; }
	float GetRotation() { return rotation; }

	void MarkForDeletion() { markedForDeletion = true; }
	bool GetMarkedForDeletion() { return markedForDeletion; }

private:

	bool markedForDeletion;
	bool isInitialized;

	std::string m_tag;
	std::vector<Component*> m_components;

	Vector2 position;
	float scale, rotation;

	void CreateAttachedComponent(Component* pAttachedComponent);
};

template<typename T>
inline void GameObject::AddComponent(Component* C)
{
	// Verify T is of type Component, if it is not, it should throw an error, 
	//	so its likely that the following assert will never be reached in the event of an error...
	Component* component;
	if (!C)
		component = new T();
	else
		component = C;

	// ...but just in case
	ASSERT(component != nullptr, "Attempted attachement of class must be of type Component!");

	// Verify type of T is not already an attached component
	bool exists = false;
	for (auto c : m_components)
		if (dynamic_cast<T*>(c)) exists = true;

	// Allocate T and register to systems as necessary
	if (!exists)
		CreateAttachedComponent(component);
	else
	{
		SAFE_DELETE(component);
		throw std::runtime_error("Component of this type has already been attached, duplicates are unsupported");
	}

	component = nullptr;
}

template<typename ...Ts>
inline void GameObject::AddComponents()
{
	int unpacking[] = { 0, (AddComponent<Ts>(), 0)... }; // pack expansion trick. function call is a side effect of the initializer list being initialized to 0s, but is used for all va args, so we accomplish add component for all without recursively calling it
	(void)unpacking; // suppress unused variable warning
}

// Can lead to undefined behaviour in diamond inheritance structures. Left side classes pointing to most derived can be unsafely cast to Right side classes
template<typename T>
inline T* GameObject::GetComponent()
{
	for (auto c : m_components)
		if (dynamic_cast<T*>(c))
			return dynamic_cast<T*>(c);

	return nullptr;
}

template<typename T>
inline void GameObject::RemoveComponent()
{
	auto iter = m_components.begin();
	for (iter; iter != m_components.end(); iter++)
		if (dynamic_cast<T*>(*iter))
			break;

	if (iter != m_components.end())
		m_components.erase(iter);
}

template<typename ...Ts>
inline void GameObject::RemoveComponents()
{
	int unpacking[] = { 0, (RemoveComponent<Ts>(), 0)... }; // pack expansion trick. function call is a side effect of the initializer list being initialized to 0s, but is used for all va args, so we accomplish add component for all without recursively calling it
	(void)unpacking; // suppress unused variable warning
}

template<typename T>
inline bool GameObject::HasComponent()
{
	for (auto c : m_components)
		if (dynamic_cast<T*>(c))
			return true;

	return false;
}

#endif