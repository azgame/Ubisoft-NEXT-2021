#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include "GameObject.h"

#include <mutex>

class SceneObjects
{
public:
	static SceneObjects& GetInstance();

	void AddGameObject(GameObject* pObject);
	bool HasGameObject(GameObject* pObject);
	void RemoveGameObject(GameObject* pObject);

	void InitObjects();
	void UpdateObjects(float deltaTime);
	void DrawObjects();
	
	void CleanupObjects();
	void Shutdown();

private:
	static std::vector<GameObject*> sm_objects;
	static std::mutex s_mutex;
};


#endif