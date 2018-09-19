#include <windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include "ClassDirectX.h"

DirectXInstances DirectX::m_directXInstances;

HRESULT DirectXObjectInitializer::Initialize(BOOL canWindow)
{
	DirectXInstances& rDirectXInstances = DirectX::m_directXInstances;

	if (NULL == (rDirectXInstances.m_pDirectX = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	ZeroMemory(&rDirectXInstances.m_DirectXPresentParam, sizeof(rDirectXInstances.m_DirectXPresentParam));

	SetBuckBuffer(canWindow);

	rDirectXInstances.m_DirectXPresentParam.Windowed = canWindow;

	return S_OK;
}

VOID DirectXObjectInitializer::SetBuckBuffer(BOOL canWindow)
{
	D3DPRESENT_PARAMETERS& rDirectXPresentParam = DirectX::m_directXInstances.m_DirectXPresentParam;

	rDirectXPresentParam.BackBufferFormat = D3DFMT_X8R8G8B8;
	rDirectXPresentParam.BackBufferCount = 1;
	rDirectXPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;

	const INT WIDTH_FULLSCREEN = 1920;
	const INT HEIGHT_FULLSCREEN = 1080;

	if (!canWindow)
	{
		rDirectXPresentParam.BackBufferWidth = WIDTH_FULLSCREEN;
		rDirectXPresentParam.BackBufferHeight = HEIGHT_FULLSCREEN;
		rDirectXPresentParam.hDeviceWindow = *DirectX::m_directXInstances.m_pHWnd;
		rDirectXPresentParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	return;
}

HRESULT DirectXObject::Initialize()
{
	m_pDirectXObjectInitializer = new DirectXObjectInitializer;

	HRESULT hr = m_pDirectXObjectInitializer->Initialize(m_canWindow);

	delete m_pDirectXObjectInitializer;

	return hr;
}

VOID DirectXObject::SetWindowMode(BOOL canWindow)
{
	m_canWindow = canWindow;

	return;
}

VOID DirectX3DDeviceInitializer::SetRenderState(BOOL canCullPolygon)
{
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = DirectX::m_directXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	rpDirectX3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	rpDirectX3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (canCullPolygon)
	{
		rpDirectX3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	return;
}

VOID DirectX3DDeviceInitializer::SetTextureStageState()
{
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = DirectX::m_directXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	rpDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	rpDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	rpDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return;
}

HRESULT DirectX3DDeviceInitializer::Initialize(t_VERTEX_FORMAT d3DFVF, BOOL canCullPolygon)
{
	DirectXInstances& rDirectXInstances = DirectX::m_directXInstances;

	if (FAILED(rDirectXInstances.m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *rDirectXInstances.m_pHWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &rDirectXInstances.m_DirectXPresentParam, &rDirectXInstances.m_pDirectX3DDevice)))
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(rDirectXInstances.m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, *rDirectXInstances.m_pHWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			&rDirectXInstances.m_DirectXPresentParam, &rDirectXInstances.m_pDirectX3DDevice)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}

	SetRenderState(canCullPolygon);
	SetTextureStageState();

	rDirectXInstances.m_pDirectX3DDevice->SetFVF(d3DFVF);

	return S_OK;
}

HRESULT DirectX3DDevice::Initialize()
{
	m_pDirectX3DDeviceInitializer = new DirectX3DDeviceInitializer;

	HRESULT hr = m_pDirectX3DDeviceInitializer->Initialize(m_d3DFVF, m_canCullPolygon);

	delete m_pDirectX3DDeviceInitializer;

	return hr;
}

VOID DirectX3DDevice::SetCullPolygon(const BOOL canCullPolygon)
{
	m_canCullPolygon = canCullPolygon;

	return;
}

VOID DirectX3DDevice::SetVertexFormat(t_VERTEX_FORMAT d3DFVF)
{
	m_d3DFVF = d3DFVF;

	return;
}

VOID DirectX3DDevice::PrepareRender()
{
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = DirectX::m_directXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	rpDirectX3DDevice->BeginScene();

	return;
}

VOID DirectX3DDevice::CleanUpRender()
{
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = DirectX::m_directXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->EndScene();
	rpDirectX3DDevice->Present(NULL, NULL, NULL, NULL);

	return;
}

HRESULT DirectXInputDevicesInitializer::Initialize()
{
	HRESULT hr;

	DirectXInstances& rDirectXInstances = DirectX::m_directXInstances;

	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&rDirectXInstances.m_pDirectXInput, NULL)))
	{
		return hr;
	}

	if (FAILED(hr = rDirectXInstances.m_pDirectXInput->CreateDevice(GUID_SysKeyboard,
		&(rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD]), NULL)))
	{
		return hr;
	}

	if (FAILED(hr = (rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD])->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	if (FAILED(hr = (rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD])->SetCooperativeLevel(
		*rDirectXInstances.m_pHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	if (FAILED(hr = rDirectXInstances.m_pDirectXInput->CreateDevice(GUID_SysMouse,
		&(rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]), NULL)))
	{
		return hr;
	}

	if (FAILED(hr = (rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetDataFormat(&c_dfDIMouse)))
	{
		return hr;
	}

	if (FAILED(hr = (rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetCooperativeLevel(
		*rDirectXInstances.m_pHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return hr;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(hr = (rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
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

	HRESULT hr = m_pDirectXInputDevicesInitializer->Initialize();

	delete m_pDirectXInputDevicesInitializer;

	return hr;
}

VOID MouseAndKeyboardStatesStorage::Store(InputData& rInputData)
{
	memcpy(rInputData.m_keyBoardState.m_diksPrev, rInputData.m_keyBoardState.m_diks, sizeof(BYTE) * 256);
	memcpy(&rInputData.m_mouseState.m_mouseStatePrev, &rInputData.m_mouseState.m_mouseState, sizeof(DIMOUSESTATE));

	return;
}

VOID DirectXInputDevices::StoreInputStates()
{
	m_pInputStatesStoreter = new MouseAndKeyboardStatesStorage;

	m_pInputStatesStoreter->Store(m_InputData);

	return;
}

VOID MouseAndKeyboardStatesGetter::Get(InputData& rInputData)
{
	DirectXInstances& rDirectXInstances = DirectX::m_directXInstances;

	rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->Acquire();
	rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->GetDeviceState(sizeof(DIMOUSESTATE), &rInputData.m_mouseState.m_mouseState);

	GetCursorPos(&rInputData.m_mouseState.m_absolutePos);

	ScreenToClient(*rDirectXInstances.m_pHWnd, &rInputData.m_mouseState.m_absolutePos);

	memset(rInputData.m_mouseState.m_buttonPush, 0, sizeof(BOOL) * 4 * 4);

	for (INT button = 0; button < 4; button++)
	{
		if (rInputData.m_mouseState.m_mouseState.rgbButtons[button])
		{
			if (rInputData.m_mouseState.m_mouseStatePrev.rgbButtons[button])
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
			if (rInputData.m_mouseState.m_mouseStatePrev.rgbButtons[button])
			{
				rInputData.m_mouseState.m_buttonPush[button] = TRUE;
			}

			else
			{
				rInputData.m_mouseState.m_buttonUninput[button] = TRUE;
			}
		}
	}

	rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD]->Acquire();
	rDirectXInstances.m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD]->GetDeviceState(sizeof(BYTE) * 256, rInputData.m_keyBoardState.m_diks);

	memset(rInputData.m_keyBoardState.m_keyPush, NULL, sizeof(BOOL) * 256 * 4);

	for (INT key = 0; key < 256; key++)
	{
		if (rInputData.m_keyBoardState.m_diksPrev[key] & 0x80)
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

VOID DirectXInputDevices::GetInputStates()
{
	m_pInputStatesGetter = new MouseAndKeyboardStatesGetter;

	m_pInputStatesGetter->Get(m_InputData);

	delete m_pInputStatesGetter;

	return;
}

DirectX* DirectX::GetInstance()
{
	DirectXInstances& rDirectXInstances = DirectX::m_directXInstances;

	if (!rDirectXInstances.m_pDirectXClass)
	{
		return rDirectXInstances.m_pDirectXClass = new(DirectX);
	}

	return rDirectXInstances.m_pDirectXClass;
}

VOID DirectX::DeleteInstance()
{
	delete DirectX::m_directXInstances.m_pDirectXClass;

	return;
}

VOID DirectX::SetHWND(HWND* pHWnd)
{
	m_directXInstances.m_pHWnd = pHWnd;

	return;
}
