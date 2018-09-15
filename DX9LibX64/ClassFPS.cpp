#include<windows.h>
#include"ClassFPS.h"

FPS* FPS::m_pFPSClass;

FPS* FPS::GetInstancePointer()
{
	if (!m_pFPSClass)m_pFPSClass = new(FPS);

	return m_pFPSClass;
}

VOID FPS::Init(UINT frame)
{
	m_frame = frame;

	timeBeginPeriod(1);

	m_timeCurrent = timeGetTime();

	timeEndPeriod(1);

	m_timePrev = 0;

	return;
}

BOOL FPS::CheckTime()
{
	timeBeginPeriod(1);

	if (m_timeCurrent - m_timePrev >= 1000 / m_frame)
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

	return;
}
