#include "stdafx.h"
#include "SceneObjects.h"

std::vector<GameObject*> SceneObjects::sm_objects;
std::mutex SceneObjects::s_mutex;

SceneObjects& SceneObjects::GetInstance()
{
	static SceneObjects instance;
	return instance;
}

void SceneObjects::AddGameObject(GameObject* pObject)
{
	std::lock_guard<std::mutex> lg(s_mutex);

	if (!HasGameObject(pObject))
		sm_objects.push_back(pObject);
}

bool SceneObjects::HasGameObject(GameObject* pObject)
{
	auto it = std::find(sm_objects.begin(), sm_objects.end(), pObject);
	if (it != sm_objects.end())
		return true;

	return false;
}

void SceneObjects::RemoveGameObject(GameObject* pObject)
{
	std::lock_guard<std::mutex> lg(s_mutex);

	auto it = std::find(sm_objects.begin(), sm_objects.end(), pObject);
	if (it != sm_objects.end())
		sm_objects.erase(it);

	SAFE_DELETE(pObject);
}

void SceneObjects::InitObjects()
{
	for (auto go : sm_objects)
		go->OnStart();
}

void SceneObjects::UpdateObjects(float deltaTime)
{
	for (int i = 0; i < sm_objects.size(); i++)
		sm_objects[i]->Update(deltaTime);
}

void SceneObjects::DrawObjects()
{
	for (auto go : sm_objects)
		go->Render();
}

void SceneObjects::CleanupObjects()
{
	std::vector<GameObject*> deleteList;
	std::vector<GameObject*> liveList;

	for (auto go : sm_objects)
	{
		if (go->GetMarkedForDeletion())
			deleteList.push_back(go);
		else
			liveList.push_back(go);
	}

	for (auto go : deleteList)
		SAFE_DELETE(go);

	deleteList.clear();

	sm_objects = liveList;
}

void SceneObjects::Shutdown()
{
	for (auto go : sm_objects)
		SAFE_DELETE(go);

	sm_objects.clear();
}
