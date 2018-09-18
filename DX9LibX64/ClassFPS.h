#pragma once
#include<windows.h>
#include"DX9LibX64.h"

class FPS
{
public:
	static FPS* GetInstancePointer();

	BOOL CoordinateFrame();

	VOID UpdateTime();

private:
	FPS();
	~FPS() {};

	static FPS* m_pFPSClass;

	typedef DWORD t_TIME;
	t_TIME m_timeCurrent;
	t_TIME m_timePrev;
};
