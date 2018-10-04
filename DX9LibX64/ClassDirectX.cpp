#include <windows.h>
#include <tchar.h>
#include <d3dx9.h>
#include <dinput.h>
#include <tchar.h>
#include "ClassDirectX.h"

DirectXInstances DirectX::m_directXInstances;

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
	rpDirectX3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00CCCCCC);
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

	rpDirectX3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0xE0, 0xF8, 0xF7), 1.f, 0);
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

template<typename T>
VOID DirectX3DDevice::DrawVertex(const T* pVertex, const LPDIRECT3DTEXTURE9* pTexture, DWORD fVF, size_t size)
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = NULL;

	rpDirectX3DDevice->CreateVertexBuffer(
		size,
		D3DUSAGE_WRITEONLY,
		fVF,
		D3DPOOL_MANAGED,
		&vertexBuffer,
		NULL);

	T* pVertexTmp;

	vertexBuffer->Lock(0, 0, (void**)&pVertexTmp, 0);

	const INT ARRAY_NUM = size / sizeof(T);

	pVertexTmp = new T[ARRAY_NUM];

	memcpy(pVertexTmp, pVertex, size);

	vertexBuffer->Unlock();

	rpDirectX3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(T));

	rpDirectX3DDevice->SetTexture(0, *pTexture);

	const INT TRIANGLE_VERTICES_NUM = 3;
	const INT TRIANGLE_NUM = size / (sizeof(T)*TRIANGLE_VERTICES_NUM)

	rpDirectX3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, TRIANGLE_NUM);

	vertexBuffer->Release();
	delete[]pVertexTmp;
}

VOID DirectX3DDevice::Custom2DVertices(CustomVertex *pCustomVertex, FLOAT posX, FLOAT posY, FLOAT posZ, FLOAT scaleX, FLOAT scaleY,
	DWORD color, FLOAT startPosTu, FLOAT startPosTv, FLOAT scaleTu, FLOAT scaleTv, FLOAT scaleImageX, FLOAT scaleImageY)
{
	pCustomVertex[0] = { posX - scaleX ,posY - scaleY,posZ,1,color,startPosTu / scaleImageX,startPosTv / scaleImageY };
	pCustomVertex[1] = { posX + scaleX ,posY - scaleY,posZ,1,color,(startPosTu + scaleTu) / scaleImageX,startPosTv / scaleImageY };
	pCustomVertex[2] = { posX + scaleX ,posY + scaleY,posZ,1,color,(startPosTu + scaleTu) / scaleImageX,(startPosTv + scaleTv) / scaleImageY };
	pCustomVertex[3] = { posX - scaleX ,posY + scaleY,posZ,1,color,startPosTu / scaleImageX,(startPosTv + scaleTv) / scaleImageY };

	return;
}

VOID DirectX3DDevice::SetFont(INT scaleX, UINT scaleY, const TCHAR *pFontType, LPD3DXFONT *pFontId, UINT thickness, INT charSet)
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	if (FAILED(D3DXCreateFont(rpDirectX3DDevice, scaleY, scaleX, thickness, 0, 0, charSet, 0, 0, 0, pFontType, pFontId)))
	{
		return;
	}

	return;
}

VOID DirectX3DDevice::WriteText(INT posX, INT posY, const TCHAR *pText, UINT format, LPD3DXFONT pFontId, DWORD color)
{
	D3DXFONT_DESC fontSetting;

	pFontId->GetDesc(&fontSetting);

	const FLOAT charSpace = 1.3f;
	INT textScaleX = (INT)(fontSetting.Width* charSpace * _tcslen(pText)) / 2;
	INT textScaleY = fontSetting.Height / 2;

	RECT rcText = { posX - textScaleX ,posY - textScaleY ,posX + textScaleX ,posY + textScaleY };

	if (FAILED(pFontId->DrawText(NULL, pText, -1, &rcText, format, color)))
	{
		return;
	}

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

	return;
}

VOID Camera::GetCameraPos(D3DXVECTOR3* pCameraPos)
{
	pCameraPos->x = m_cameraPos.x;
	pCameraPos->y = m_cameraPos.y;
	pCameraPos->z = m_cameraPos.z;

	return;
}

VOID Camera::SetEyePoint(FLOAT x, FLOAT y, FLOAT z)
{
	m_eyePoint.x = x;
	m_eyePoint.y = y;
	m_eyePoint.z = z;

	return;
}

VOID Camera::GetEyePoint(D3DXVECTOR3* pEyePoint)
{
	pEyePoint->x = m_eyePoint.x;
	pEyePoint->y = m_eyePoint.y;
	pEyePoint->z = m_eyePoint.z;

	return;
}

VOID Camera::SetTransform()
{
	DirectX* pDirectX = DirectX::GetInstance();
	DirectXInstances& rDirectXInstances = pDirectX->GetDirectXInstances();
	LPDIRECTINPUTDEVICE8* pDirectXInputDevices = rDirectXInstances.m_pDirectXInputDevices;
	InputData inputData = pDirectX->m_DirectXInputDevices.m_InputData;
	LPDIRECT3DDEVICE9& rpDirectX3DDevice = rDirectXInstances.m_pDirectX3DDevice;

	MouseState& rMouseState = inputData.m_mouseState;

	KeyBoardState& rKeyState = inputData.m_keyBoardState;

	D3DXMatrixIdentity(&m_view);

	{
		D3DXVECTOR2 straightVec(0.0f, 0.0f);

		straightVec.y = m_eyePoint.z - m_cameraPos.z;
		straightVec.x = m_eyePoint.x - m_cameraPos.x;

		float syntheticStraightVec = (float)sqrt(pow(straightVec.y, 2) + pow(straightVec.x, 2));

		D3DXVECTOR2 straightUnitVec(0.0f, 0.0f);

		straightUnitVec.y = straightVec.y / syntheticStraightVec;

		if (isnan(straightUnitVec.y))
		{
			straightUnitVec.y = 1.0f;
		}

		straightUnitVec.x = straightVec.x / syntheticStraightVec;

		if (isnan(straightUnitVec.x))
		{
			straightUnitVec.x = 1.0f;
		}

		D3DXVECTOR2 rightUnitVec;

		float powStraightVecX = (float)pow(straightVec.x, 2);

		rightUnitVec.y = (float)sqrt(powStraightVecX / (pow(straightVec.y, 2) + powStraightVecX));

		if (isnan(rightUnitVec.y))
		{
			rightUnitVec.y = 1.0f;
		}

		rightUnitVec.x = (-rightUnitVec.y*straightUnitVec.y) / straightUnitVec.x;

		if (isnan(rightUnitVec.x))
		{
			rightUnitVec.x = 1.0f;
		}

		float moveVecCross = straightUnitVec.x*rightUnitVec.y - straightUnitVec.y*rightUnitVec.x;

		float speedMultiply = 0.3f;

		const INT NOT_JUMP = -1;

		const INT DO_JUMP = 0;

		static INT jumpCount = NOT_JUMP;

		if (rKeyState.m_keyPush[DIK_SPACE] && jumpCount == NOT_JUMP)
		{
			jumpCount = DO_JUMP;
		}

		if (jumpCount >= 0)
		{
			const INT fPS = 60;
			
			static float initialVelocity = 8.4f / fPS;

			static float gravity = -20.8f / fPS;

			float time = (float)jumpCount / (float)fPS;

			m_cameraPos.y += (initialVelocity*time + 0.5f *gravity * (float)pow(time, 2));

			++jumpCount;

			if (m_cameraPos.y < 0.0f)
			{
				m_cameraPos.y = 0.0f;
				jumpCount = NOT_JUMP;
			}
		}

		if (rKeyState.m_keyHold[DIK_LCONTROL])
		{
			speedMultiply = 0.1f;
		}

		else if (rKeyState.m_keyHold[DIK_LSHIFT])
		{
			speedMultiply = 1.3f;
		}

		if (rKeyState.m_keyHold[DIK_W])
		{
			m_cameraPos.z += straightUnitVec.y*speedMultiply;
			m_cameraPos.x += straightUnitVec.x*speedMultiply;
		}

		if (rKeyState.m_keyHold[DIK_S])
		{
			m_cameraPos.z -= straightUnitVec.y*speedMultiply;
			m_cameraPos.x -= straightUnitVec.x*speedMultiply;
		}

		if (rKeyState.m_keyHold[DIK_D])
		{
			if (moveVecCross > 0)
			{
				m_cameraPos.z -= rightUnitVec.y*speedMultiply;
				m_cameraPos.x -= rightUnitVec.x*speedMultiply;
			}

			if (moveVecCross < 0)
			{
				m_cameraPos.z += rightUnitVec.y*speedMultiply;
				m_cameraPos.x += rightUnitVec.x*speedMultiply;
			}
		}

		if (rKeyState.m_keyHold[DIK_A])
		{
			if (moveVecCross > 0)
			{
				m_cameraPos.z += rightUnitVec.y*speedMultiply;
				m_cameraPos.x += rightUnitVec.x*speedMultiply;
			}

			if (moveVecCross < 0)
			{
				m_cameraPos.z -= rightUnitVec.y*speedMultiply;
				m_cameraPos.x -= rightUnitVec.x*speedMultiply;
			}
		}
	}

	static bool canLockOn = false;

	if (rKeyState.m_keyPush[DIK_Q])
	{
		canLockOn = (canLockOn) ? false : true;
	}

	static float totalEyeRadX = 0.0f;
	static float totalEyeRadY = 0.0f;

	if (canLockOn)
	{
		/*totalEyeRadX = 0.0f;
		totalEyeRadY = 0.0f;*/

		m_eyePoint.x = 0.0f;
		m_eyePoint.y = 0.0f;
		m_eyePoint.z = 10.0f;

		D3DXVECTOR2 straightVec(0.0f, 0.0f);

		straightVec.y = m_eyePoint.z - m_cameraPos.z;
		straightVec.x = m_eyePoint.x - m_cameraPos.x;

		if (!straightVec.x && !straightVec.x)
		{
			straightVec.y = m_eyePoint.z - m_cameraPos.z*0.9f;
			straightVec.x = m_eyePoint.x - m_cameraPos.x*0.9f;
		}

		float syntheticStraightVec = (float)sqrt(pow(straightVec.y, 2) + pow(straightVec.x, 2));

		D3DXVECTOR2 straightUnitVec(0.0f, 0.0f);

		straightUnitVec.y = straightVec.y / syntheticStraightVec;

		if (isnan(straightUnitVec.y))
		{
			straightVec.y = 1.0f;
		}

		straightUnitVec.x = straightVec.x / syntheticStraightVec;

		if (isnan(straightUnitVec.x))
		{
			straightVec.x = 1.0f;
		}

		D3DXVECTOR2 originalUnitVec(0.0f, 1.0f);

		double cos = (double)D3DXVec2Dot(&straightUnitVec, &originalUnitVec);

		float eyeVecCross = straightUnitVec.x*originalUnitVec.y - straightUnitVec.y*originalUnitVec.x;

		if (eyeVecCross > 0)
		{
			totalEyeRadX = (float)acos(cos);
		}

		if (eyeVecCross < 0)
		{
			totalEyeRadX = -(float)acos(cos);
		}

		totalEyeRadY = 0.0f;
	}

	if (!canLockOn)
	{
		totalEyeRadX += D3DXToRadian(rMouseState.m_mouseState.lX * 0.05f);

		D3DXMATRIX matRotatation;
		D3DXMatrixIdentity(&matRotatation);

		D3DXMATRIX matRotationX;
		D3DXMatrixIdentity(&matRotationX);

		//まず最初に、原点に半径を足しただけの座標を用意する
		D3DXVECTOR3 vecTarget(0.0f, 0.f, 10.0f);

		//次に、原点を中心とした回転（オイラー回転）の行列を作る
		D3DXMatrixRotationY(&matRotationX, totalEyeRadX);

		D3DXMATRIX matRotationY;
		D3DXMatrixIdentity(&matRotationY);

		totalEyeRadY += D3DXToRadian(rMouseState.m_mouseState.lY * 0.05f);

		const float EYE_RADIAN_Y_LIMIT = D3DXToRadian(85.0f);

		totalEyeRadY = min(max(totalEyeRadY, -EYE_RADIAN_Y_LIMIT), EYE_RADIAN_Y_LIMIT);

		D3DXMatrixRotationX(&matRotationY, totalEyeRadY);

		D3DXMatrixMultiply(&matRotatation, &matRotationY, &matRotationX);

		D3DXVec3TransformCoord(&vecTarget, &vecTarget, &matRotatation);

		//最後に本来の座標（回転対象の座標）を足す
		D3DXVec3Add(&vecTarget, &vecTarget, &m_cameraPos);

		m_eyePoint.x = vecTarget.x;
		m_eyePoint.y = vecTarget.y;
		m_eyePoint.z = vecTarget.z;
	}

	/*TCHAR buff[256];

	_stprintf_s(buff, 256, _T("x:%f\n"), m_eyePoint.x);

	OutputDebugString(buff);

	_stprintf_s(buff, 256, _T("y:%f\n"), m_eyePoint.y);

	OutputDebugString(buff);

	_stprintf_s(buff, 256, _T("z:%f\n"), m_eyePoint.z);

	OutputDebugString(buff);*/
	//SetWindowText(*rDirectXInstances.m_pHWnd, buff);

	D3DXMatrixLookAtLH(&m_view,
		&m_cameraPos,
		&m_eyePoint,
		&m_cameraOverhead);

	rpDirectX3DDevice->SetTransform(D3DTS_VIEW, &m_view);

	D3DVIEWPORT9 viewPort;
	rpDirectX3DDevice->GetViewport(&viewPort);

	float aspect = (float)viewPort.Width / (float)viewPort.Height;

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

	return;
}

VOID Camera::NegateView(D3DXMATRIX* pMatRotate)
{
	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, NULL, &m_view);
	D3DXMatrixMultiply(pMatRotate, pMatRotate, &viewInverse);

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
