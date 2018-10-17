#include<windows.h>
#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>
#include"FPS.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

FPS* FPS::m_pFPSClass = NULL;

FPS::~FPS()
{
}

FPS* FPS::GetInstance()
{
	if (!m_pFPSClass)
	{
		m_pFPSClass = new FPS;
	}

	return m_pFPSClass;
}

FPS::FPS() :m_timePrev(0)
{
	timeBeginPeriod(1);

	m_timeCurrent = timeGetTime();

	timeEndPeriod(1);
}

BOOL FPS::CoordinateFrame()
{
	timeBeginPeriod(1);

	const INT DEFAULT_FPS = 60;

	if (m_timeCurrent - m_timePrev >= 1000 / DEFAULT_FPS)
	{
		m_timePrev = timeGetTime();

		timeEndPeriod(1);

		return TRUE;
	}

	timeEndPeriod(1);

	return FALSE;
}

VOID FPS::UpdateTime()
{
	timeBeginPeriod(1);

	m_timeCurrent = timeGetTime();

	timeEndPeriod(1);
}

VOID FPS::DeleteInstance()
{
	delete m_pFPSClass;
	m_pFPSClass = NULL;
}
