#pragma once
#include <windows.h>

template <class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		if (!m_pSingleton)m_pSingleton = new T;
	}

	VOID DeleteInstatnce()
	{
		delete m_pSingleton;
		m_pSingleton = NULL;
	}

protected:
	Singleton() {};
	~Singleton() {};

	static T* m_pSingleton = NULL;
};
