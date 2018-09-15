#pragma once
#include<windows.h>
#include"DX9LibX64.h"

class FPS
{
public:
	static FPS* GetInstancePointer();
	VOID Init(UINT frame);
	BOOL CheckTime();
	VOID UpdateTime();

	UINT m_frame;

private:
	CREATE_CONSTRACTA_AND_DESTRACTA(FPS)

	static FPS* m_pFPSClass;
	DWORD m_timeCurrent;
	DWORD m_timePrev;
};
