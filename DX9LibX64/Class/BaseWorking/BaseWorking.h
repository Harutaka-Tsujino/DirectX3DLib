#pragma once
#include <windows.h>

class BaseWorking
{
public:
	BaseWorking() {};
	~BaseWorking() {};

	virtual VOID Update() = 0;
	virtual VOID Render() = 0;
};
