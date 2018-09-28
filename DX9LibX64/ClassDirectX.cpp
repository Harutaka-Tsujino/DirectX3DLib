#include <windows.h>
#include <tchar.h>
#include <d3dx9.h>
#include <dinput.h>
#include "ClassDirectX.h"

DirectXInstances DirectX::m_directXInstances;

typedef LPD3DXFONT FONTID;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID SetFont(INT scaleX, UINT scaleY, const CHAR *pFontType, FONTID *pFontId, UINT thickness, INT charSet)
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	if (FAILED(D3DXCreateFontA(rpDirectX3DDevice, scaleY, scaleX, thickness, 0, 0, charSet, 0, 0, 0, pFontType, pFontId)))
	{
		return;
	}

	return;
}

VOID WriteText(INT posX, INT posY, const CHAR *pText, UINT format, FONTID pFontId, DWORD color)
{
	D3DXFONT_DESC fontSetting;

	pFontId->GetDesc(&fontSetting);

	const FLOAT charSpace = 1.3f;
	INT textScaleX = (INT)(fontSetting.Width* charSpace * strlen(pText)) / 2;
	INT textScaleY = fontSetting.Height / 2;

	RECT rcText = { posX - textScaleX ,posY - textScaleY ,posX + textScaleX ,posY + textScaleY };

	if (FAILED(pFontId->DrawTextA(NULL, pText, -1, &rcText, format, color)))
	{
		return;
	}

	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT DirectXObjectInitializer::Initialize(BOOL canWindow)
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();

	if (NULL == (rDirectXInstances.m_pDirectX = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, TEXT("Direct3Dの作成に失敗しました"), TEXT(""), MB_OK);
		return E_FAIL;
	}

	ZeroMemory(&rDirectXInstances.m_DirectXPresentParam, sizeof(rDirectXInstances.m_DirectXPresentParam));

	SetBuckBuffer(canWindow);

	rDirectXInstances.m_DirectXPresentParam.Windowed = canWindow;

	return S_OK;
}

VOID DirectXObjectInitializer::SetBuckBuffer(BOOL canWindow)
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	D3DPRESENT_PARAMETERS& rDirectXPresentParam = rDirectXInstances.m_DirectXPresentParam;

	rDirectXPresentParam.BackBufferFormat = D3DFMT_X8R8G8B8;
	rDirectXPresentParam.BackBufferCount = 1;
	rDirectXPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	rDirectXPresentParam.EnableAutoDepthStencil = TRUE;
	rDirectXPresentParam.AutoDepthStencilFormat = D3DFMT_D16;

	const INT WIDTH_FULLSCREEN = 1920;
	const INT HEIGHT_FULLSCREEN = 1080;

	if (!canWindow)
	{
		rDirectXPresentParam.BackBufferWidth = WIDTH_FULLSCREEN;
		rDirectXPresentParam.BackBufferHeight = HEIGHT_FULLSCREEN;
		rDirectXPresentParam.hDeviceWindow = *rDirectXInstances.m_pHWnd;
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
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	rpDirectX3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	rpDirectX3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	rpDirectX3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	rpDirectX3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	rpDirectX3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00BBBBBB);
	rpDirectX3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	rpDirectX3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	rpDirectX3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	rpDirectX3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x33);

	if (!canCullPolygon)
	{
		rpDirectX3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	return;
}

VOID DirectX3DDeviceInitializer::SetTextureStageState()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	rpDirectX3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	rpDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	rpDirectX3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return;
}

VOID DirectX3DDeviceInitializer::SetViewPort()
{
	D3DVIEWPORT9 viewPort;

	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	D3DPRESENT_PARAMETERS& rDirectXPresentParam = rDirectXInstances.m_DirectXPresentParam;
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	viewPort.Width = rDirectXPresentParam.BackBufferWidth;
	viewPort.Height = rDirectXPresentParam.BackBufferHeight;
	viewPort.MinZ = 0.f;
	viewPort.MaxZ = 1.0f;
	viewPort.X = 0;
	viewPort.Y = 0;

	rpDirectX3DDevice->SetViewport(&viewPort);

	return;
}

HRESULT DirectX3DDeviceInitializer::Initialize(t_VERTEX_FORMAT d3DFVF, BOOL canCullPolygon)
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	D3DPRESENT_PARAMETERS& rDirectXPresentParam = rDirectXInstances.m_DirectXPresentParam;
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;
	HWND& rHWnd = *rDirectXInstances.m_pHWnd;

	if (FAILED(rDirectXInstances.m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, rHWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &rDirectXPresentParam, &rpDirectX3DDevice)))
	{
		MessageBox(0, TEXT("HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します"), NULL, MB_OK);
		if (FAILED(rDirectXInstances.m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, rHWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			&rDirectXPresentParam, &rpDirectX3DDevice)))
		{
			MessageBox(0, TEXT("DIRECT3Dデバイスの作成に失敗しました"), NULL, MB_OK);
			return E_FAIL;
		}
	}

	SetRenderState(canCullPolygon);
	SetTextureStageState();
	SetViewPort();

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
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0xE0, 0xF8, 0xF7), 1.f, 0);
	rpDirectX3DDevice->BeginScene();

	return;
}

VOID DirectX3DDevice::CleanUpRender()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->EndScene();
	rpDirectX3DDevice->Present(NULL, NULL, NULL, NULL);

	return;
}

HRESULT DirectXInputDevicesInitializer::Initialize()
{
	HRESULT hr;

	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECTINPUT8& rDirectXInput = rDirectXInstances.m_pDirectXInput;
	LPDIRECTINPUTDEVICE8* pDirectXInputDevices = rDirectXInstances.m_pDirectXInputDevices;
	HWND& rHWnd = *rDirectXInstances.m_pHWnd;

	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&rDirectXInput, NULL)))
	{
		return hr;
	}

	if (FAILED(hr = rDirectXInstances.m_pDirectXInput->CreateDevice(GUID_SysKeyboard,
		&(pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD]), NULL)))
	{
		return hr;
	}

	if (FAILED(hr = (pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD])->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	if (FAILED(hr = (pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD])->SetCooperativeLevel(
		rHWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	if (FAILED(hr = rDirectXInstances.m_pDirectXInput->CreateDevice(GUID_SysMouse,
		&(pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]), NULL)))
	{
		return hr;
	}

	if (FAILED(hr = (pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetDataFormat(&c_dfDIMouse)))
	{
		return hr;
	}

	if (FAILED(hr = (pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetCooperativeLevel(
		rHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return hr;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(hr = (pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE])->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
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
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECTINPUTDEVICE8* pDirectXInputDevices = rDirectXInstances.m_pDirectXInputDevices;
	HWND& rHWnd = *rDirectXInstances.m_pHWnd;

	pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->Acquire();
	pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MOUSE]->GetDeviceState(sizeof(DIMOUSESTATE), &rInputData.m_mouseState.m_mouseState);

	GetCursorPos(&rInputData.m_mouseState.m_absolutePos);

	ScreenToClient(rHWnd, &rInputData.m_mouseState.m_absolutePos);

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

	pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD]->Acquire();
	pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::KEYBOARD]->GetDeviceState(sizeof(BYTE) * 256, rInputData.m_keyBoardState.m_diks);

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

Camera::Camera()
{
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 0.0f;
	m_cameraPos.z = -0.1f;

	m_eyePoint.x = 0.0f;
	m_eyePoint.y = 0.0f;
	m_eyePoint.z = 0.0f;

	m_cameraOverhead.x = 0.0f;
	m_cameraOverhead.y = 1.0f;
	m_cameraOverhead.z = 0.0f;
}

VOID Camera::SetCameraPos(FLOAT x, FLOAT y, FLOAT z)
{
	m_cameraPos.x = x;
	m_cameraPos.y = y;
	m_cameraPos.z = z;

	return;
}

VOID Camera::SetEyePoint(FLOAT x, FLOAT y, FLOAT z)
{
	m_eyePoint.x = x;
	m_eyePoint.y = y;
	m_eyePoint.z = z;

	return;
}

VOID Camera::SetTransform()
{
	D3DXMATRIX view;

	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECTINPUTDEVICE8* pDirectXInputDevices = rDirectXInstances.m_pDirectXInputDevices;
	InputData inputData = pDirectX->m_DirectXInputDevices.m_InputData;
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	D3DXMatrixIdentity(&view);

	static int frameCount = 0;

	

	/*FONTID fontId;

	if (!frameCount)
	{
		SetFont(40, 40, "Times New Roman", &fontId, 20,0);
	}

	WriteText(600,600,)*/

	++frameCount;

	//D3DXVECTOR3 vecCenter(0.f, 0.f, 0.f);

	//D3DXMATRIX matRotation;
	//D3DXMatrixIdentity(&matRotation);


	//まず最初に、原点に半径を足しただけの座標を用意する
	//D3DXVECTOR3 vecTarget(-30.0f, 0.f, 0.0f);
	//次に、原点を中心とした回転（オイラー回転）の行列を作る
	//D3DXMatrixRotationX(&matRotation, 0 *(3.145f / 180.f));

	/*if (!(frameCount%90))
	{
		m_cameraOverhead.y *= -1;
	}*/

	//D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotation);
	////最後に本来の座標（回転対象の座標）を足す
	//D3DXVec3Add(&vecTarget, &vecTarget, &vecCenter);
	//m_cameraPos.x = vecTarget.x;
	//m_cameraPos.y = vecTarget.y;
	//m_cameraPos.z = vecTarget.z;

	TCHAR buff[256];

	_stprintf_s(buff, 256, _T("x:%f\n"), m_cameraPos.x);

	OutputDebugString(buff);

	_stprintf_s(buff, 256, _T("y:%f\n"), m_cameraPos.y);

	OutputDebugString(buff);

	_stprintf_s(buff, 256, _T("z:%f\n"), m_cameraPos.z);

	OutputDebugString(buff);
	//SetWindowText(*rDirectXInstances.m_pHWnd, buff);

	D3DXMatrixLookAtLH(&view,
		&m_cameraPos,
		&m_eyePoint,
		&m_cameraOverhead);

	rpDirectX3DDevice->SetTransform(D3DTS_VIEW, &view);

	D3DVIEWPORT9 viewPort;
	rpDirectX3DDevice->GetViewport(&viewPort);

	float aspect = (float)viewPort.Width / (float)viewPort.Height;

	const INT DEFAULT_EYE_RADIAN = 60;
	const FLOAT DEFAULT_FAR = 100.f;

	D3DXMATRIX projection;

	D3DXMatrixPerspectiveFovLH(
		&projection,
		D3DXToRadian(DEFAULT_EYE_RADIAN),
		aspect,
		1.f,
		DEFAULT_FAR);

	rpDirectX3DDevice->SetTransform(D3DTS_PROJECTION, &projection);

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

DirectXInstances& DirectX::GetDirectXInstances()
{
	return m_directXInstances;
}
