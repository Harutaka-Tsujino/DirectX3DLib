#pragma once
#include <windows.h>
#include"DX9LibX64.h"

class Window
{
public:
	static Window* GetInstancePointer();
	template<typename T1, typename T2>
	VOID Init(const HINSTANCE hInst, const CHAR* nameApp, const T1 widthDisplay, const T2 heightDisplay, const BOOL window);
	template<typename T1, typename T2>
	VOID SetSizeDisplay(const T1 widthDisplay, const T2 heightDisplay);
	VOID SetWindowMode(const BOOL window);
	VOID MakeWindow();
	BOOL ConfirmMessage();

	INT m_heightDisplay;
	INT m_widthDisplay;
	BOOL m_window;
	MSG m_msg;
	HWND m_hWnd;

private:
	CREATE_CONSTRACTA_AND_DESTRACTA(Window)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static Window* m_pWindowClass;
	const CHAR* m_nameApp;
	HINSTANCE m_hInst;
};
