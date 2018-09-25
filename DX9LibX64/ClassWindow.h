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
	BOOL ConfirmMessage();

	INT m_widthDisplay;
	INT m_heightDisplay;
	BOOL m_canWindow;
	MSG m_msg;
	HWND m_hWnd;

private:
	Window(const HINSTANCE hInst, LPCWSTR nameApp);
	~Window() {};

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static Window* m_pWindowClass;
	LPCWSTR m_nameApp;
	HINSTANCE m_hInst;
};
