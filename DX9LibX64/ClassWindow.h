#pragma once
#include <windows.h>
#include "DX9LibX64.h"

class Window
{
public:
	static Window* GetInstance(const HINSTANCE hInst, LPCWSTR nameApp);

	template<typename T1, typename T2>
	VOID SetSizeDisplay(const T1 widthDisplay, const T2 heightDisplay);

	VOID SetWindowMode(const BOOL canWindow);
	VOID MakeWindow();
	HWND* GetHWNDPtr();
	MSG GetMSG();

	BOOL ConfirmMessage();

	INT m_widthDisplay;
	INT m_heightDisplay;
	BOOL m_canWindow;

private:
	Window(const HINSTANCE hInst, LPCWSTR nameApp);
	~Window() {};

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static Window* m_pWindowClass;
	LPCWSTR m_nameApp;
	HINSTANCE m_hInst;
	HWND m_hWnd;
	MSG m_msg;
};
