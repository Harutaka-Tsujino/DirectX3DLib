#pragma once
#include <windows.h>

class Activity
{
public:
	Activity() {};
	~Activity() {};

	virtual VOID Update() = 0;
	virtual VOID Render() = 0;
};
