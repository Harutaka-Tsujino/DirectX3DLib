#include<windows.h>
#include"ClassWindow.h"

Window* Window::m_pWindowClass;

Window* Window::GetInstancePointer()
{
	if (!m_pWindowClass)m_pWindowClass = new(Window);

	return m_pWindowClass;
}

template<typename T1, typename T2>
VOID Window::Init(const HINSTANCE hInst, const CHAR* nameApp, const T1 widthDisplay, const T2 heightDisplay, const BOOL window)
{
	m_widthDisplay = (INT)widthDisplay;
	m_heightDisplay = (INT)heightDisplay;
	m_window = window;
	m_nameApp = nameApp;
	m_hInst = hInst;
	m_hWnd = 0;
	ZeroMemory(&m_msg, sizeof(m_msg));

	return;
}

template VOID Window::Init<INT, INT>(const HINSTANCE hInst, const CHAR* nameApp, const INT widthDisplay, const INT heightDisplay, const BOOL window);

VOID Window::SetWindowMode(const BOOL window)
{
	m_window = window;

	return;
}

template<typename T1, typename T2>
VOID Window::SetSizeDisplay(const T1 widthDisplay, const T2 heightDisplay)
{
	m_widthDisplay = (INT)widthDisplay;
	m_heightDisplay = (INT)heightDisplay;

	return;
}

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

	if (m_window)
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
