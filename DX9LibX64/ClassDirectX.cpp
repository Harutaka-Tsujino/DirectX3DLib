#include<windows.h>
#include<d3dx9.h>
#include <dinput.h>
#include"ClassDirectX.h"

DirectX* DirectX::m_pDirectXClass;

VOID DirectX::Init(HWND* hWnd, BOOL window, BOOL cullPolygon, DWORD d3DFVF)
{
	m_pHWnd = hWnd;
	m_window = window;
	m_cullPolygon = cullPolygon;
	m_d3DFVF = d3DFVF;
}

HRESULT DirectX::InitDirectX()
{
	if (NULL == (m_pDirectX = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	ZeroMemory(&m_DirectXPresentParam, sizeof(m_DirectXPresentParam));

	SetBuckBufferOverall();

	m_DirectXPresentParam.Windowed = m_window;

	return S_OK;
}

VOID DirectX::SetBuckBufferOverall()
{
	m_DirectXPresentParam.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_DirectXPresentParam.BackBufferCount = 1;
	m_DirectXPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

	const INT WIDTH_FULLSCREEN = 1920;
	const INT HEIGHT_FULLSCREEN = 1080;

	if (!m_window)
	{
		m_DirectXPresentParam.BackBufferWidth = WIDTH_FULLSCREEN;
		m_DirectXPresentParam.BackBufferHeight = HEIGHT_FULLSCREEN;
		m_DirectXPresentParam.hDeviceWindow = *m_pHWnd;
		m_DirectXPresentParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	return;
}

HRESULT DirectX::InitDirectX3DDevice()
{
	if (FAILED(m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *m_pHWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_DirectXPresentParam, &m_pDirectX3DDevice)))
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, *m_pHWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&m_DirectXPresentParam, &m_pDirectX3DDevice)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}

	SetRenderState();
	SetTextureStageState();

	m_pDirectX3DDevice->SetFVF(m_d3DFVF);

	return S_OK;
}

VOID DirectX::SetRenderState()
{
	m_pDirectX3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDirectX3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDirectX3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_cullPolygon)
	{
		m_pDirectX3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	return;
}

VOID DirectX::SetTextureStageState()
{
	m_pDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return;
}

HRESULT DirectX::InitDirectXInputDevices()
{
	HRESULT hr;

	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDirectXInput, NULL)))
	{
		return hr;
	}

	if (FAILED(hr = m_pDirectXInput->CreateDevice(GUID_SysKeyboard,
		&(m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD]), NULL)))
	{
		return hr;
	}

	if (FAILED(hr = (m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD])->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	if (FAILED(hr = (m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD])->SetCooperativeLevel(
		*m_pHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	memset(&m_keyBoardState, NULL, sizeof(KeyBoardState));

	if (FAILED(hr = m_pDirectXInput->CreateDevice(GUID_SysMouse,
		&(m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]), NULL)))
	{
		return hr;
	}

	if (FAILED(hr = (m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetDataFormat(&c_dfDIMouse)))
	{
		return hr;
	}

	if (FAILED(hr = (m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetCooperativeLevel(
		*m_pHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return hr;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(hr = (m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return hr;
	}

	memset(&m_mouseState, 0, sizeof(MouseState));

	return S_OK;
}

DirectX* DirectX::GetInstancePointer()
{
	if (!m_pDirectXClass)m_pDirectXClass = new(DirectX);

	return m_pDirectXClass;
}

VOID DirectX::PrepareRender()
{
	m_pDirectX3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	m_pDirectX3DDevice->BeginScene();

	return;
}

VOID DirectX::CleanUpRender()
{
	m_pDirectX3DDevice->EndScene();
	m_pDirectX3DDevice->Present(NULL, NULL, NULL, NULL);

	return;
}

VOID DirectX::GetInputStates()
{
	memcpy(m_keyBoardState.m_prevDiks, m_keyBoardState.m_diks, sizeof(BYTE) * 256);
	memcpy(&m_mouseState.m_prevDirectInputMouseState, &m_mouseState.m_directInputMouseState, sizeof(DIMOUSESTATE));
}

VOID DirectX::UpdateInputStates()
{
	m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->Acquire();
	m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState.m_directInputMouseState);

	GetCursorPos(&m_mouseState.m_absolutePos);

	ScreenToClient(*m_pHWnd, &m_mouseState.m_absolutePos);

	memset(m_mouseState.m_buttonPush, 0, sizeof(BOOL) * 4 * 4);

	for (INT button = 0; button < 4; button++)
	{
		if (m_mouseState.m_directInputMouseState.rgbButtons[button])
		{
			if (m_mouseState.m_prevDirectInputMouseState.rgbButtons[button])
			{
				m_mouseState.m_buttonHold[button] = TRUE;
			}

			else
			{
				m_mouseState.m_buttonRelease[button] = TRUE;
			}
		}

		else
		{
			if (m_mouseState.m_prevDirectInputMouseState.rgbButtons[button])
			{
				m_mouseState.m_buttonPush[button] = TRUE;
			}

			else
			{
				m_mouseState.m_buttonUninput[button] = TRUE;
			}
		}
	}

	m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD]->Acquire();
	m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD]->GetDeviceState(sizeof(BYTE) * 256, m_keyBoardState.m_diks);

	memset(m_keyBoardState.m_keyPush, NULL, sizeof(BOOL) * 256 * 4);

	for (INT key = 0; key < 256; key++)
	{
		if (m_keyBoardState.m_prevDiks[key] & 0x80)
		{
			if (m_keyBoardState.m_diks[key] & 0x80)
			{
				m_keyBoardState.m_keyHold[key] = TRUE;
			}

			else
			{
				m_keyBoardState.m_keyRelease[key] = TRUE;
			}
		}

		else
		{
			if (m_keyBoardState.m_diks[key] & 0x80)
			{
				m_keyBoardState.m_keyPush[key] = TRUE;
			}

			else
			{
				m_keyBoardState.m_keyUninput[key] = TRUE;
			}
		}
	}

	return;
}
