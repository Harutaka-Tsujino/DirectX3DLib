#include <windows.h>
#include <tchar.h>
#include <d3dx9.h>
#include <dinput.h>
#include <crtdbg.h>
#include <tchar.h>
#include <cstdio>
#include <cstdlib>
#include "DirectX.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

DirectXInstances DirectX::m_directXInstances;

HRESULT DirectXObjectInitializer::Initialize(BOOL canWindow)
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();

	if (NULL == (rDirectXInstances.m_pDirectX = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, _T("Direct3Dの作成に失敗しました"), _T(""), MB_OK);
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
	rDirectXPresentParam.Windowed = TRUE;

	const INT WIDTH_FULLSCREEN = 1920;
	const INT HEIGHT_FULLSCREEN = 1080;

	if (!canWindow)
	{
		rDirectXPresentParam.BackBufferWidth = WIDTH_FULLSCREEN;
		rDirectXPresentParam.BackBufferHeight = HEIGHT_FULLSCREEN;
		rDirectXPresentParam.hDeviceWindow = *rDirectXInstances.m_pHWnd;
		rDirectXPresentParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		rDirectXPresentParam.Windowed = FALSE;
	}
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
	rpDirectX3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00CCCCCC);
	rpDirectX3DDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	rpDirectX3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	rpDirectX3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	rpDirectX3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x33);

	if (!canCullPolygon)
	{
		rpDirectX3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}
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
		MessageBox(0, _T("HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します"), NULL, MB_OK);
		if (FAILED(rDirectXInstances.m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, rHWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			&rDirectXPresentParam, &rpDirectX3DDevice)))
		{
			MessageBox(0, _T("DIRECT3Dデバイスの作成に失敗しました"), NULL, MB_OK);
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
}

VOID DirectX3DDevice::SetVertexFormat(t_VERTEX_FORMAT d3DFVF)
{
	m_d3DFVF = d3DFVF;
}

VOID DirectX3DDevice::PrepareRender()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0xE0, 0xF8, 0xF7), 1.f, 0);
	rpDirectX3DDevice->BeginScene();
}

VOID DirectX3DDevice::CleanUpRender()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	rpDirectX3DDevice->EndScene();
	rpDirectX3DDevice->Present(NULL, NULL, NULL, NULL);
}

VOID DirectX3DDevice::SetFont(INT scaleX, UINT scaleY, const TCHAR *pFontType, LPD3DXFONT *pFontId, UINT thickness, INT charSet)
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	D3DXCreateFont(rpDirectX3DDevice, scaleY, scaleX, thickness, 0, 0, charSet, 0, 0, 0, pFontType, pFontId);
}

VOID DirectX3DDevice::WriteText(INT posX, INT posY, const TCHAR *pText, UINT format, LPD3DXFONT pFontId, DWORD color)
{
	D3DXFONT_DESC fontSetting;

	pFontId->GetDesc(&fontSetting);

	const FLOAT charSpace = 1.3f;
	INT textScaleX = (INT)(fontSetting.Width* charSpace * _tcslen(pText)) / 2;
	INT textScaleY = fontSetting.Height / 2;

	RECT rcText = { posX - textScaleX ,posY - textScaleY ,posX + textScaleX ,posY + textScaleY };

	pFontId->DrawText(NULL, pText, -1, &rcText, format, color);
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
}

VOID DirectXInputDevices::StoreInputStates()
{
	/*m_pInputStatesStoreter = new MouseAndKeyboardStatesStorage;

	m_pInputStatesStoreter->Store(m_InputData);

	delete m_pInputStatesStoreter;*/

	static MouseAndKeyboardStatesStorage inputStorage;

	inputStorage.Store(m_InputData);
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

	for (INT button = 0; button < 4; ++button)
	{
		if (rInputData.m_mouseState.m_mouseStatePrev.rgbButtons[button] & 0x80)
		{
			if (rInputData.m_mouseState.m_mouseState.rgbButtons[button])
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
			if (rInputData.m_mouseState.m_mouseState.rgbButtons[button])
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
}

VOID DirectXInputDevices::GetInputStates()
{
	/*m_pInputStatesGetter = new MouseAndKeyboardStatesGetter;

	m_pInputStatesGetter->Get(m_InputData);

	delete m_pInputStatesGetter;*/

	static MouseAndKeyboardStatesGetter inputGetter;

	inputGetter.Get(m_InputData);
}

VOID DirectXInputDevices::GetInputData(InputData* pInputData)
{
	memcpy(pInputData, &m_InputData, sizeof(InputData));
}

Camera::Camera()
{
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 0.0f;
	m_cameraPos.z = 0.0f;

	m_eyePoint.x = 0.0f;
	m_eyePoint.y = 0.0f;
	m_eyePoint.z = 1.0f;

	m_cameraOverhead.x = 0.0f;
	m_cameraOverhead.y = 1.0f;
	m_cameraOverhead.z = 0.0f;
}

VOID Camera::SetCameraPos(FLOAT x, FLOAT y, FLOAT z)
{
	m_cameraPos.x = x;
	m_cameraPos.y = y;
	m_cameraPos.z = z;
}

VOID Camera::GetCameraPos(D3DXVECTOR3* pCameraPos)
{
	pCameraPos->x = m_cameraPos.x;
	pCameraPos->y = m_cameraPos.y;
	pCameraPos->z = m_cameraPos.z;
}

VOID Camera::SetEyePoint(FLOAT x, FLOAT y, FLOAT z)
{
	m_eyePoint.x = x;
	m_eyePoint.y = y;
	m_eyePoint.z = z;
}

VOID Camera::GetEyePoint(D3DXVECTOR3* pEyePoint)
{
	pEyePoint->x = m_eyePoint.x;
	pEyePoint->y = m_eyePoint.y;
	pEyePoint->z = m_eyePoint.z;
}

VOID Camera::SetTransform()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECTINPUTDEVICE8* pDirectXInputDevices = rDirectXInstances.m_pDirectXInputDevices;
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	D3DXMatrixIdentity(&m_view);

	//{
	//	D3DXVECTOR2 straightVec(0.0f, 0.0f);
	//
	//	straightVec.y = m_eyePoint.z - m_cameraPos.z;
	//	straightVec.x = m_eyePoint.x - m_cameraPos.x;
	//
	//	FLOAT syntheticStraightVec = (FLOAT)sqrt(pow(straightVec.y, 2) + pow(straightVec.x, 2));
	//
	//	D3DXVECTOR2 straightUnitVec(0.0f, 0.0f);
	//
	//	straightUnitVec.y = straightVec.y / syntheticStraightVec;
	//
	//	if (isnan(straightUnitVec.y))
	//	{
	//		straightUnitVec.y = 1.0f;
	//	}
	//
	//	straightUnitVec.x = straightVec.x / syntheticStraightVec;
	//
	//	if (isnan(straightUnitVec.x))
	//	{
	//		straightUnitVec.x = 1.0f;
	//	}
	//
	//	D3DXVECTOR2 rightUnitVec;
	//
	//	FLOAT powStraightVecX = (FLOAT)pow(straightVec.x, 2);
	//
	//	rightUnitVec.y = (FLOAT)sqrt(powStraightVecX / (pow(straightVec.y, 2) + powStraightVecX));
	//
	//	if (isnan(rightUnitVec.y))
	//	{
	//		rightUnitVec.y = 1.0f;
	//	}
	//
	//	rightUnitVec.x = (-rightUnitVec.y*straightUnitVec.y) / straightUnitVec.x;
	//
	//	if (isnan(rightUnitVec.x))
	//	{
	//		rightUnitVec.x = 1.0f;
	//	}
	//
	//	FLOAT moveVecCross = straightUnitVec.x*rightUnitVec.y - straightUnitVec.y*rightUnitVec.x;
	//
	//	static bool canSlast = false;
	//
	//	if (rKeyState.m_keyPush[DIK_E] && !m_cameraPos.y)
	//	{
	//		canSlast = (canSlast) ? false : true;
	//	}
	//
	//	const INT NOT_SLAST = -1;
	//
	//	const INT DO_SLAST = 0;
	//
	//	static INT slastCount = NOT_SLAST;
	//
	//	if (rKeyState.m_keyPush[DIK_SPACE] && canSlast&&!m_cameraPos.y)
	//	{
	//		slastCount = DO_SLAST;
	//	}
	//
	//	if (rKeyState.m_keyHold[DIK_SPACE]&& slastCount >= DO_SLAST)
	//	{
	//		m_cameraPos.y += 0.1f;
	//
	//		slastCount = DO_SLAST;
	//	}
	//
	//	const INT fPS = 60;
	//
	//	static FLOAT gravity = -20.8f / fPS;
	//
	//	if (rKeyState.m_keyUninput[DIK_SPACE] && slastCount >= DO_SLAST)
	//	{
	//		FLOAT time = (FLOAT)slastCount / (FLOAT)fPS;
	//
	//		m_cameraPos.y += (0.5f *gravity * (FLOAT)pow(time, 2));
	//
	//		slastCount++;
	//
	//		if (m_cameraPos.y < 0.0f)
	//		{
	//			m_cameraPos.y = 0.0f;
	//			slastCount = NOT_SLAST;
	//		}
	//	}
	//
	//	FLOAT speedMultiply = 0.3f;
	//
	//	const INT NOT_JUMP = -1;
	//
	//	const INT DO_JUMP = 0;
	//
	//	static INT jumpCount = NOT_JUMP;
	//
	//	if (rKeyState.m_keyPush[DIK_SPACE] && jumpCount == NOT_JUMP&&!canSlast)
	//	{
	//		jumpCount = DO_JUMP;
	//	}
	//
	//	if (jumpCount >= DO_JUMP)
	//	{
	//		static FLOAT initialVelocity = 8.4f / fPS;
	//
	//		FLOAT time = (FLOAT)jumpCount / (FLOAT)fPS;
	//
	//		m_cameraPos.y += (initialVelocity*time + 0.5f *gravity * (FLOAT)pow(time, 2));
	//
	//		++jumpCount;
	//
	//		if (m_cameraPos.y < 0.0f)
	//		{
	//			m_cameraPos.y = 0.0f;
	//			jumpCount = NOT_JUMP;
	//		}
	//	}
	//	
	//	if (rKeyState.m_keyHold[DIK_LCONTROL])
	//	{
	//		speedMultiply = 0.1f;
	//	}
	//
	//	else if (rKeyState.m_keyHold[DIK_LSHIFT])
	//	{
	//		speedMultiply = 1.3f;
	//	}
	//
	//	if (rKeyState.m_keyHold[DIK_W])
	//	{
	//		m_cameraPos.z += straightUnitVec.y*speedMultiply;
	//		m_cameraPos.x += straightUnitVec.x*speedMultiply;
	//	}
	//
	//	if (rKeyState.m_keyHold[DIK_S])
	//	{
	//		m_cameraPos.z -= straightUnitVec.y*speedMultiply;
	//		m_cameraPos.x -= straightUnitVec.x*speedMultiply;
	//	}
	//
	//	if (rKeyState.m_keyHold[DIK_D])
	//	{
	//		if (moveVecCross > 0)
	//		{
	//			m_cameraPos.z -= rightUnitVec.y*speedMultiply;
	//			m_cameraPos.x -= rightUnitVec.x*speedMultiply;
	//		}
	//
	//		if (moveVecCross < 0)
	//		{
	//			m_cameraPos.z += rightUnitVec.y*speedMultiply;
	//			m_cameraPos.x += rightUnitVec.x*speedMultiply;
	//		}
	//	}
	//
	//	if (rKeyState.m_keyHold[DIK_A])
	//	{
	//		if (moveVecCross > 0)
	//		{
	//			m_cameraPos.z += rightUnitVec.y*speedMultiply;
	//			m_cameraPos.x += rightUnitVec.x*speedMultiply;
	//		}
	//
	//		if (moveVecCross < 0)
	//		{
	//			m_cameraPos.z -= rightUnitVec.y*speedMultiply;
	//			m_cameraPos.x -= rightUnitVec.x*speedMultiply;
	//		}
	//	}
	//}
	//
	//static bool canLockOn = false;
	//
	//if (rKeyState.m_keyPush[DIK_Q])
	//{
	//	canLockOn = (canLockOn) ? false : true;
	//}
	//
	//static FLOAT totalEyeRadX = 0.0f;
	//static FLOAT totalEyeRadY = 0.0f;

	//if (canLockOn)
	//{
	//	/*totalEyeRadX = 0.0f;
	//	totalEyeRadY = 0.0f;*/
	//
	//	m_eyePoint.x = 0.0f;
	//	m_eyePoint.y = 0.0f;
	//	m_eyePoint.z = 10.0f;
	//
	//	{
	//		D3DXVECTOR2 straightVec(0.0f, 0.0f);
	//
	//		straightVec.y = m_eyePoint.z - m_cameraPos.z;
	//		straightVec.x = m_eyePoint.x - m_cameraPos.x;
	//
	//		if (!straightVec.x && !straightVec.y)
	//		{
	//			straightVec.y = m_eyePoint.z - m_cameraPos.z*0.9f;
	//			straightVec.x = m_eyePoint.x - m_cameraPos.x*0.9f;
	//		}
	//
	//		FLOAT syntheticStraightVec = (FLOAT)sqrt(pow(straightVec.y, 2) + pow(straightVec.x, 2));
	//
	//		D3DXVECTOR2 straightUnitVec(0.0f, 0.0f);
	//
	//		straightUnitVec.y = straightVec.y / syntheticStraightVec;
	//
	//		if (isnan(straightUnitVec.y))
	//		{
	//			straightVec.y = 1.0f;
	//		}
	//
	//		straightUnitVec.x = straightVec.x / syntheticStraightVec;
	//
	//		if (isnan(straightUnitVec.x))
	//		{
	//			straightVec.x = 1.0f;
	//		}
	//
	//		D3DXVECTOR2 originalUnitVec(0.0f, 1.0f);
	//
	//		DOUBLE cos = (DOUBLE)D3DXVec2Dot(&straightUnitVec, &originalUnitVec);
	//
	//		FLOAT eyeVecCross = straightUnitVec.x*originalUnitVec.y - straightUnitVec.y*originalUnitVec.x;
	//
	//		if (eyeVecCross == 0)
	//		{
	//			totalEyeRadX = 0.0f;
	//		}
	//
	//		if (eyeVecCross > 0)
	//		{
	//			totalEyeRadX = (FLOAT)acos(cos);
	//		}
	//
	//		if (eyeVecCross < 0)
	//		{
	//			totalEyeRadX = -(FLOAT)acos(cos);
	//		}
	//	}
	//
	//	{
	//		D3DXVECTOR3 straightVec(m_cameraPos.x - m_eyePoint.x, m_cameraPos.y - m_eyePoint.y, m_cameraPos.z - m_eyePoint.z);
	//		
	//		FLOAT straightVecLen = D3DXVec3Length(&straightVec);
	//
	//		D3DXVECTOR3 originalVec(straightVec.x, 0.0f, straightVec.z);
	//
	//		FLOAT originalVecLen = D3DXVec3Length(&originalVec);
	//
	//		FLOAT cos = D3DXVec3Dot(&originalVec,&straightVec)/(straightVecLen*originalVecLen);
	//
	//		FLOAT rad = (FLOAT)acos((DOUBLE)cos);
	//
	//		FLOAT eyeVecCross = m_cameraPos.y - m_eyePoint.y;
	//
	//		if (eyeVecCross == 0)
	//		{
	//			totalEyeRadY = 0.0f;
	//		}
	//
	//		if (eyeVecCross > 0)
	//		{
	//			totalEyeRadY = rad;
	//		}
	//
	//		if (eyeVecCross < 0)
	//		{
	//			totalEyeRadY = -rad;
	//		}
	//	}
	//}
	//
	//if (!canLockOn)
	//{
	//	totalEyeRadX += D3DXToRadian(rMouseState.m_mouseState.lX * 0.05f);
	//
	//	D3DXMATRIX matRotatation;
	//	D3DXMatrixIdentity(&matRotatation);
	//
	//	D3DXMATRIX matRotationX;
	//	D3DXMatrixIdentity(&matRotationX);
	//
	//	//まず最初に、原点に半径を足しただけの座標を用意する
	//	D3DXVECTOR3 vecTarget(0.0f, 0.f, 10.0f);
	//
	//	//次に、原点を中心とした回転（オイラー回転）の行列を作る
	//	D3DXMatrixRotationY(&matRotationX, totalEyeRadX);
	//
	//	D3DXMATRIX matRotationY;
	//	D3DXMatrixIdentity(&matRotationY);
	//
	//	totalEyeRadY += D3DXToRadian(rMouseState.m_mouseState.lY * 0.05f);
	//
	//	const FLOAT EYE_RADIAN_Y_LIMIT = D3DXToRadian(88.0f);
	//
	//	totalEyeRadY = min(max(totalEyeRadY, -EYE_RADIAN_Y_LIMIT), EYE_RADIAN_Y_LIMIT);
	//
	//	D3DXMatrixRotationX(&matRotationY, totalEyeRadY);
	//
	//	D3DXMatrixMultiply(&matRotatation, &matRotationY, &matRotationX);
	//
	//	D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotatation);
	//
	//	//最後に本来の座標（回転対象の座標）を足す
	//	D3DXVec3Add(&vecTarget, &vecTarget, &m_cameraPos);
	//
	//	m_eyePoint.x = vecTarget.x;
	//	m_eyePoint.y = vecTarget.y;
	//	m_eyePoint.z = vecTarget.z;
	//}

	/*TCHAR buff[256];

	_stprintf_s(buff, 256, _T("x:%f\n"), m_eyePoint.x);

	OutputDebugString(buff);

	_stprintf_s(buff, 256, _T("y:%f\n"), m_eyePoint.y);

	OutputDebugString(buff);

	_stprintf_s(buff, 256, _T("z:%f\n"), m_eyePoint.z);

	OutputDebugString(buff);
	SetWindowText(*rDirectXInstances.m_pHWnd, buff);*/

	D3DXMatrixLookAtLH(&m_view,
		&m_cameraPos,
		&m_eyePoint,
		&m_cameraOverhead);

	rpDirectX3DDevice->SetTransform(D3DTS_VIEW, &m_view);

	D3DVIEWPORT9 viewPort;
	rpDirectX3DDevice->GetViewport(&viewPort);

	FLOAT aspect = (FLOAT)viewPort.Width / (FLOAT)viewPort.Height;

	const INT DEFAULT_EYE_RADIAN = 60;
	const FLOAT DEFAULT_FAR = 10000.0f;

	D3DXMATRIX projection;

	D3DXMatrixPerspectiveFovLH(
		&projection,
		D3DXToRadian(DEFAULT_EYE_RADIAN),
		aspect,
		0.01f,
		DEFAULT_FAR);

	rpDirectX3DDevice->SetTransform(D3DTS_PROJECTION, &projection);
}

VOID Camera::NegateView(D3DXMATRIX* pMatRotate)
{
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, NULL, &m_view);
	D3DXMatrixMultiply(pMatRotate, pMatRotate, &viewInverse);
}

DirectX::~DirectX()
{
}

DirectX* DirectX::GetInstance()
{
	DirectXInstances& rDirectXInstances = DirectX::m_directXInstances;

	if (!rDirectXInstances.m_pDirectXClass)
	{
		return rDirectXInstances.m_pDirectXClass = new DirectX;
	}

	return rDirectXInstances.m_pDirectXClass;
}

VOID DirectX::DeleteInstance()
{
	delete m_directXInstances.m_pDirectXClass;
	m_directXInstances.m_pDirectXClass = NULL;
}

VOID DirectX::SetHWND(HWND* pHWnd)
{
	m_directXInstances.m_pHWnd = pHWnd;
}

DirectXInstances& DirectX::GetDirectXInstances()
{
	return m_directXInstances;
}
