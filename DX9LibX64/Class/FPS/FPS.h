#pragma once
#include <windows.h>

class FPS
{
public:
	static FPS* GetInstance();

	BOOL CoordinateFrame();
	VOID UpdateTime();
	VOID DeleteInstance();

private:
	FPS();
	~FPS();

	static FPS* m_pFPSClass;

	typedef DWORD t_TIME;
	t_TIME m_timeCurrent;
	t_TIME m_timePrev;
};
