#include<windows.h>
#include<d3dx9.h>
#include <dinput.h>
#include"ClassDirectX.h"

HWND* DirectXInstances::m_pHWnd;
LPDIRECT3D9 DirectXInstances::m_pDirectX;
LPDIRECT3DDEVICE9 DirectXInstances::m_pDirectX3DDevice;
LPDIRECTINPUT8 DirectXInstances::m_pDirectXInput;
LPDIRECTINPUTDEVICE8 DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MAX_INPUT_DEVICES];
LPD3DXFONT DirectXInstances::m_pDirectXFont;
D3DPRESENT_PARAMETERS DirectXInstances::m_DirectXPresentParam;
DirectX* DirectXInstances::m_pDirectXClass;

HRESULT DirectXObjectInitializer::Initialize(BOOL window)
{
	if (NULL == (DirectXInstances::m_pDirectX = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	ZeroMemory(&DirectXInstances::m_DirectXPresentParam, sizeof(DirectXInstances::m_DirectXPresentParam));

	SetBuckBufferOverall(window);

	DirectXInstances::m_DirectXPresentParam.Windowed = window;

	return S_OK;
}

VOID DirectXObjectInitializer::SetBuckBufferOverall(BOOL window)
{
	D3DPRESENT_PARAMETERS directXPresentParam = DirectXInstances::m_DirectXPresentParam;

	directXPresentParam.BackBufferFormat = D3DFMT_X8R8G8B8;
	directXPresentParam.BackBufferCount = 1;
	directXPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

	const INT WIDTH_FULLSCREEN = 1920;
	const INT HEIGHT_FULLSCREEN = 1080;

	if (!window)
	{
		directXPresentParam.BackBufferWidth = WIDTH_FULLSCREEN;
		directXPresentParam.BackBufferHeight = HEIGHT_FULLSCREEN;
		directXPresentParam.hDeviceWindow = *DirectXInstances::m_pHWnd;
		directXPresentParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	return;
}

DirectXObject::DirectXObject()
{
	m_window = FALSE;
}

HRESULT DirectXObject::Initialize()
{
	m_pDirectXObjectInitializer= new DirectXObjectInitializer;

	return m_pDirectXObjectInitializer->Initialize(m_window);
}

VOID DirectXObject::SetWindowMode(BOOL window)
{
	m_window = window;

	return;
}

VOID DirectX3DDeviceInitializer::SetRenderState(BOOL cullPollygon)
{
	LPDIRECT3DDEVICE9 pDirectX3DDevice = DirectXInstances::m_pDirectX3DDevice;

	pDirectX3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDirectX3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDirectX3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (cullPollygon)
	{
		pDirectX3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	return;
}

VOID DirectX3DDeviceInitializer::SetTextureStageState()
{
	LPDIRECT3DDEVICE9 pDirectX3DDevice = DirectXInstances::m_pDirectX3DDevice;

	pDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return;
}

HRESULT DirectX3DDeviceInitializer::Initialize(t_VERTEX_FORMAT d3DFVF, BOOL cullPollygon)
{
	if (FAILED(DirectXInstances::m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *DirectXInstances::m_pHWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectXInstances::m_DirectXPresentParam, &DirectXInstances::m_pDirectX3DDevice)))
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(DirectXInstances::m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, *DirectXInstances::m_pHWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&DirectXInstances::m_DirectXPresentParam, &DirectXInstances::m_pDirectX3DDevice)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}

	SetRenderState(cullPollygon);
	SetTextureStageState();

	DirectXInstances::m_pDirectX3DDevice->SetFVF(d3DFVF);

	return S_OK;
}

DirectX3DDevice::DirectX3DDevice()
{
	m_cullPolygon = true;

	m_d3DFVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE);
}

HRESULT DirectX3DDevice::Initialize()
{
	m_pDirectX3DDeviceInitializer = new DirectX3DDeviceInitializer;

	return m_pDirectX3DDeviceInitializer->Initialize(m_d3DFVF,m_cullPolygon);
}

VOID DirectX3DDevice::SetCullPolygon(const BOOL cullPolygon)
{
	m_cullPolygon = cullPolygon;

	return;
}

VOID DirectX3DDevice::SetVertexFormat(t_VERTEX_FORMAT d3DFVF)
{
	m_d3DFVF = d3DFVF;

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

HRESULT DirectXInputDevicesInitializer::Initialize()
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
		*DirectXInstances::m_pHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
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
		*DirectXInstances::m_pHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
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

DirectXInputDevices::DirectXInputDevices()
{
	ZeroMemory(&m_InputData, sizeof(InputData));
}

HRESULT DirectXInputDevices::Initialize()
{
	m_pDirectXInputDevicesInitializer = new DirectXInputDevicesInitializer;

	return m_pDirectXInputDevicesInitializer->Initialize();
}

VOID MouseAndKeyBoardStatesGetter::Get(InputData& rInputData)
{
	memcpy(rInputData.m_keyBoardState.m_prevDiks, rInputData.m_keyBoardState.m_diks, sizeof(BYTE) * 256);
	memcpy(&rInputData.m_mouseState.m_prevDirectInputMouseState, &rInputData.m_mouseState.m_directInputMouseState, sizeof(DIMOUSESTATE));
	return;
}

VOID DirectXInputDevices::GetInputStates()
{
	m_pInputStatesGetter = new MouseAndKeyBoardStatesGetter;

	m_pInputStatesGetter->Get(m_InputData);

	return;
}

VOID MouseAndKeyBoardStatesUpdater::Update(InputData& rInputData)
{
	DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->Acquire();
	DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->GetDeviceState(sizeof(DIMOUSESTATE), &rInputData.m_mouseState.m_directInputMouseState);

	GetCursorPos(&rInputData.m_mouseState.m_absolutePos);

	ScreenToClient(*DirectXInstances::m_pHWnd, &rInputData.m_mouseState.m_absolutePos);

	memset(rInputData.m_mouseState.m_buttonPush, 0, sizeof(BOOL) * 4 * 4);

	for (INT button = 0; button < 4; button++)
	{
		if (rInputData.m_mouseState.m_directInputMouseState.rgbButtons[button])
		{
			if (rInputData.m_mouseState.m_prevDirectInputMouseState.rgbButtons[button])
			{
				rInputData.m_mouseState.m_buttonHold[button] = TRUE;
			}

			else
			{
				rInputData.m_mouseState.m_buttonRelease[button] = TRUE;
			}
		}

		else
		{
			if (rInputData.m_mouseState.m_prevDirectInputMouseState.rgbButtons[button])
			{
				rInputData.m_mouseState.m_buttonPush[button] = TRUE;
			}

			else
			{
				rInputData.m_mouseState.m_buttonUninput[button] = TRUE;
			}
		}
	}

	DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD]->Acquire();
	DirectXInstances::m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEY_BOARD]->GetDeviceState(sizeof(BYTE) * 256, rInputData.m_keyBoardState.m_diks);

	memset(rInputData.m_keyBoardState.m_keyPush, NULL, sizeof(BOOL) * 256 * 4);

	for (INT key = 0; key < 256; key++)
	{
		if (rInputData.m_keyBoardState.m_prevDiks[key] & 0x80)
		{
			if (rInputData.m_keyBoardState.m_diks[key] & 0x80)
			{
				rInputData.m_keyBoardState.m_keyHold[key] = TRUE;
			}

			else
			{
				rInputData.m_keyBoardState.m_keyRelease[key] = TRUE;
			}
		}

		else
		{
			if (rInputData.m_keyBoardState.m_diks[key] & 0x80)
			{
				rInputData.m_keyBoardState.m_keyPush[key] = TRUE;
			}

			else
			{
				rInputData.m_keyBoardState.m_keyUninput[key] = TRUE;
			}
		}
	}

	return;
}

VOID DirectXInputDevices::UpdateInputStates()
{
	m_pInputStatesUpdater = new MouseAndKeyBoardStatesUpdater;

	m_pInputStatesUpdater->Update(m_InputData);

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
