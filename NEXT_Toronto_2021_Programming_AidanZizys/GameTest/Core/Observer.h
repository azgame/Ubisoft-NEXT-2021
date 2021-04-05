#ifndef OBSERVER_H
#define OBSERVER_H

#include "GameObject.h"

#include <vector>

// simple observer pattern

class Observer
{
public:
	virtual ~Observer() {}
	virtual void OnNotify() = 0;
};

class Subject
{
public:
	void AddObserver(Observer* Obs)
	{
		m_observers.push_back(Obs);
	}

	void RemoveObserver(Observer* Obs)
	{
		auto it = std::find(m_observers.begin(), m_observers.end(), Obs);
		if (it != m_observers.end())
			m_observers.erase(it);
	}

protected:
	virtual void Notify()
	{
		for (auto observer : m_observers)
			observer->OnNotify();
	}

private:
	std::vector<Observer*> m_observers;
};

#endif
