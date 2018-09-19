#pragma once
#include <windows.h>
#include "DX9LibX64.h"

typedef DWORD t_TIME;

class FPS
{
public:
	static FPS* GetInstance();

	BOOL CoordinateFrame();

	VOID UpdateTime();

private:
	FPS();
	~FPS() {};

	static FPS* m_pFPSClass;
	t_TIME m_timeCurrent;
	t_TIME m_timePrev;
};
