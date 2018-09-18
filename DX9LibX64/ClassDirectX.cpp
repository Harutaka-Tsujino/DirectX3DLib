#include<windows.h>
#include<d3dx9.h>
#include <dinput.h>
#include"ClassDirectX.h"

LPDIRECT3D9 DirectXInstances::m_pDirectX;
LPDIRECT3DDEVICE9 DirectXInstances::m_pDirectX3DDevice;
LPDIRECTINPUT8 DirectXInstances::m_pDirectXInput;
LPDIRECTINPUTDEVICE8 DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MAX_INPUT_DEVICES];
LPD3DXFONT DirectXInstances::m_pDirectXFont;
D3DPRESENT_PARAMETERS DirectXInstances::m_DirectXPresentParam;
DirectX* DirectXInstances::m_pDirectXClass;

HWND* DirectX::m_pHWnd;

BOOL DirectXObjectDatas::m_window;

HRESULT DirectXObjectInitializer::Init()
{
	if (NULL == (DirectXInstances::m_pDirectX = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	ZeroMemory(&DirectXInstances::m_DirectXPresentParam, sizeof(DirectXInstances::m_DirectXPresentParam));

	SetBuckBufferOverall();

	DirectXInstances::m_DirectXPresentParam.Windowed = DirectXObjectDatas::m_window;

	return S_OK;
}

VOID DirectXObjectInitializer::SetBuckBufferOverall()
{
	D3DPRESENT_PARAMETERS directXPresentParam = DirectXInstances::m_DirectXPresentParam;

	directXPresentParam.BackBufferFormat = D3DFMT_X8R8G8B8;
	directXPresentParam.BackBufferCount = 1;
	directXPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

	const INT WIDTH_FULLSCREEN = 1920;
	const INT HEIGHT_FULLSCREEN = 1080;

	if (!DirectXObjectDatas::m_window)
	{
		directXPresentParam.BackBufferWidth = WIDTH_FULLSCREEN;
		directXPresentParam.BackBufferHeight = HEIGHT_FULLSCREEN;
		directXPresentParam.hDeviceWindow = *DirectX::m_pHWnd;
		directXPresentParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	return;
}

HRESULT DirectXObject::Init()
{
	IDirectXInitializer* pDirectXObjectInitializer = m_pDirectXObjectInitializer;

	return pDirectXObjectInitializer->Init();
}

VOID DirectXObject::SetWindowMode(BOOL window)
{
	DirectXObjectDatas::m_window = window;

	return;
}

VOID DirectX3DDeviceInitializer::SetRenderState()
{
	const LPDIRECT3DDEVICE9 pDirectX3DDevice = DirectXInstances::m_pDirectX3DDevice;

	pDirectX3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDirectX3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDirectX3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (DirectX3DDeviceDatas::m_cullPolygon)
	{
		pDirectX3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	return;
}

VOID DirectX3DDeviceInitializer::SetTextureStageState()
{
	const LPDIRECT3DDEVICE9 pDirectX3DDevice = DirectXInstances::m_pDirectX3DDevice;

	pDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return;
}

HRESULT DirectX3DDeviceInitializer::Init()
{
	if (FAILED(DirectXInstances::m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *DirectX::m_pHWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectXInstances::m_DirectXPresentParam, &DirectXInstances::m_pDirectX3DDevice)))
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(DirectXInstances::m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, *DirectX::m_pHWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&DirectXInstances::m_DirectXPresentParam, &DirectXInstances::m_pDirectX3DDevice)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}

	SetRenderState();
	SetTextureStageState();

	DirectXInstances::m_pDirectX3DDevice->SetFVF(DirectX3DDeviceDatas::m_d3DFVF);

	return S_OK;
}

BOOL DirectX3DDeviceDatas::m_cullPolygon;
t_VERTEX_FOTMAT DirectX3DDeviceDatas::m_d3DFVF;

HRESULT DirectX3DDevice::Init()
{
	m_pDirectX3DDeviceInitializer = new DirectX3DDeviceInitializer;

	return m_pDirectX3DDeviceInitializer->Init();
}

VOID DirectX3DDevice::SetCullPolygon(const BOOL cullPolygon)
{
	DirectX3DDeviceDatas::m_cullPolygon = cullPolygon;

	return;
}

VOID DirectX3DDevice::SetVertexFormat(t_VERTEX_FOTMAT d3DFVF)
{
	DirectX3DDeviceDatas::m_d3DFVF = d3DFVF;

	return;
}

VOID DirectX3DDevice::PrepareRender()
{
	DirectXInstances::m_pDirectX3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	DirectXInstances::m_pDirectX3DDevice->BeginScene();

	return;
}

VOID DirectX3DDevice::CleanUpRender()
{
	DirectXInstances::m_pDirectX3DDevice->EndScene();
	DirectXInstances::m_pDirectX3DDevice->Present(NULL, NULL, NULL, NULL);

	return;
}

HRESULT DirectXInputDevicesInitializer::Init()
{
	HRESULT hr;

	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&DirectXInstances::m_pDirectXInput, NULL)))
	{
		return hr;
	}

	if (FAILED(hr = DirectXInstances::m_pDirectXInput->CreateDevice(GUID_SysKeyboard,
		&(DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD]), NULL)))
	{
		return hr;
	}

	if (FAILED(hr = (DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD])->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	if (FAILED(hr = (DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD])->SetCooperativeLevel(
		*DirectX::m_pHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	if (FAILED(hr = DirectXInstances::m_pDirectXInput->CreateDevice(GUID_SysMouse,
		&(DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]), NULL)))
	{
		return hr;
	}

	if (FAILED(hr = (DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetDataFormat(&c_dfDIMouse)))
	{
		return hr;
	}

	if (FAILED(hr = (DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetCooperativeLevel(
		*DirectX::m_pHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return hr;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(hr = (DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return hr;
	}

	return S_OK;
}

HRESULT DirectXInputDevices::Init()
{
	m_pDirectXInputDevicesInitializer = new DirectXInputDevicesInitializer;

	return m_pDirectXInputDevicesInitializer->Init();
}

VOID DirectXInputDevices::GetInputStates()
{
	memcpy(m_keyBoardState.m_prevDiks, m_keyBoardState.m_diks, sizeof(BYTE) * 256);
	memcpy(&m_mouseState.m_prevDirectInputMouseState, &m_mouseState.m_directInputMouseState, sizeof(DIMOUSESTATE));

	return;
}

VOID DirectXInputDevices::UpdateInputStates()
{
	static BOOL isFirst = TRUE;
	if (isFirst)
	{
		ZeroMemory(m_keyBoardState.m_prevDiks, sizeof(BYTE) * 256);
		ZeroMemory(&m_mouseState.m_prevDirectInputMouseState, sizeof(DIMOUSESTATE));

		isFirst = FALSE;
	}

	DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->Acquire();
	DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState.m_directInputMouseState);

	GetCursorPos(&m_mouseState.m_absolutePos);

	ScreenToClient(*DirectX::m_pHWnd, &m_mouseState.m_absolutePos);

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

	DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD]->Acquire();
	DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD]->GetDeviceState(sizeof(BYTE) * 256, m_keyBoardState.m_diks);

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

DirectX* DirectX::GetInstancePointer()
{
	if (!DirectXInstances::m_pDirectXClass)
	{
		return DirectXInstances::m_pDirectXClass = new(DirectX);
	}

	return DirectXInstances::m_pDirectXClass;
}
