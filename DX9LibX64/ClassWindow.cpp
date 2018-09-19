#include <windows.h>
#include "ClassWindow.h"

Window* Window::m_pWindowClass;

Window* Window::GetInstance(const HINSTANCE hInst, const CHAR* nameApp)
{
	if (!m_pWindowClass)m_pWindowClass = new Window(hInst, nameApp);

	return m_pWindowClass;
}

Window::Window(const HINSTANCE hInst, const CHAR* nameApp) :m_widthDisplay(1280), m_heightDisplay(720), m_canWindow(TRUE), m_hWnd(NULL)
{
	m_nameApp = nameApp;
	m_hInst = hInst;
	ZeroMemory(&m_msg, sizeof(m_msg));
}

VOID Window::SetWindowMode(const BOOL canWindow)
{
	m_canWindow = canWindow;

	return;
}

template<typename T1, typename T2>
VOID Window::SetSizeDisplay(const T1 widthDisplay, const T2 heightDisplay)
{
	m_widthDisplay = (INT)widthDisplay;
	m_heightDisplay = (INT)heightDisplay;

	return;
}

template VOID Window::SetSizeDisplay<INT, INT>(INT, INT);

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);

		break;

	case WM_KEYDOWN:

		switch ((CHAR)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);

			break;
		}

		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

VOID Window::MakeWindow()
{
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_nameApp;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	const INT WIDTH_FULLSCREEN = 1920;
	const INT HEIGHT_FULLSCREEN = 1080;

	if (m_canWindow)
	{
		m_hWnd = CreateWindow(m_nameApp, m_nameApp, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, m_widthDisplay, m_heightDisplay, NULL, NULL, m_hInst, NULL);
	}

	else
	{
		m_hWnd = CreateWindow(m_nameApp, m_nameApp, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, WIDTH_FULLSCREEN, HEIGHT_FULLSCREEN, NULL, NULL, m_hInst, NULL);
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	ZeroMemory(&m_msg, sizeof(m_msg));

	return;
}

BOOL Window::ConfirmMessage()
{
	if (PeekMessage(&m_msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&m_msg);

		DispatchMessage(&m_msg);

		return FALSE;
	}

	return TRUE;
}
