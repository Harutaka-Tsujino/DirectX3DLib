#pragma once
#include<windows.h>
#include<d3dx9.h>
#include <dinput.h>
#include"DX9LibX64.h"

//入力デバイス
enum class INPUT_DEVICES
{
	KEY_BOARD,
	MOUSE,
	MAX_INPUT_DEVICES
};

typedef struct
{
	//現在でフレームのキーボードのキーが押されていた場合上位ビットが立つ
	BYTE m_diks[256];

	//前回のでフレームのキーボードのキーが押されていた場合上位ビットが立つ
	BYTE m_prevDiks[256];

	//キーボードのキーが押された瞬間のフレームだけTRUE
	BOOL m_keyPush[256];

	//キーボードのキーが押され続けられている間TRUE
	BOOL m_keyHold[256];

	//キーボードのキーが離された瞬間のフレームだけTRUE
	BOOL m_keyRelease[256];

	//キーボードのキーが前回と現在のフレームで押されていない間TRUE
	BOOL m_keyUninput[256];
}KeyBoardState;

//マウスのカーソルの位置ボタンの入力状態を保持する
typedef struct
{
	//現在のフレームでの前回のカーソル位置からの相対位置とボタンの入力状態
	DIMOUSESTATE m_directInputMouseState;

	//前回のフレームでの前回のカーソル位置からの相対位置とボタンの入力状態
	DIMOUSESTATE m_prevDirectInputMouseState;

	//カーソルの絶対座標
	POINT m_absolutePos;

	//マウスのボタンが押された瞬間のフレームだけTRUE
	BOOL m_buttonPush[4];

	//マウスのボタンが押され続けられている間TRUE
	BOOL m_buttonHold[4];

	//マウスのボタンが離された瞬間のフレームだけTRUE
	BOOL m_buttonRelease[4];

	//マウスのボタンが前回と現在のフレームで押されていない間TRUE
	BOOL m_buttonUninput[4];
}MouseState;

class IDirectXInitializer
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(IDirectXInitializer)
	virtual HRESULT Init() = 0;

private:
};

class DirectXObjectInitializer :public IDirectXInitializer
{
public:
	HRESULT Init();
private:
	VOID SetBuckBufferOverall();
};

class DirectXObjectDatas
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectXObjectDatas)
	static BOOL m_window;

private:
};

class DirectXObject
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectXObject)
	HRESULT Init();
	VOID SetWindowMode(BOOL window);

	IDirectXInitializer* m_pDirectXObjectInitializer;
private:
};

class DirectX3DDeviceInitializer:public IDirectXInitializer
{
public:
	HRESULT Init();
private:
	VOID SetRenderState();
	VOID SetTextureStageState();
};

typedef DWORD t_VERTEX_FOTMAT;

class DirectX3DDeviceDatas
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectX3DDeviceDatas)
	static BOOL m_cullPolygon;
	static t_VERTEX_FOTMAT m_d3DFVF;

private:
};

class DirectX3DDevice
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectX3DDevice)
	HRESULT Init();
	VOID SetCullPolygon(BOOL cullPolygon);
	VOID SetVertexFormat(t_VERTEX_FOTMAT d3DFVF);
	VOID PrepareRender();
	VOID CleanUpRender();

	IDirectXInitializer* m_pDirectX3DDeviceInitializer;
private:
};

class DirectXInputDevicesInitializer :public IDirectXInitializer
{
public:
	HRESULT Init();
private:
};

class DirectXInputDevices
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectXInputDevices)
	HRESULT Init();
	VOID GetInputStates();
	VOID UpdateInputStates();

	IDirectXInitializer* m_pDirectXInputDevicesInitializer;
	KeyBoardState m_keyBoardState;
	MouseState m_mouseState;

private:
};

class DirectXInstances
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectXInstances)
	static LPDIRECT3D9 m_pDirectX;
	static LPDIRECT3DDEVICE9 m_pDirectX3DDevice;
	static LPDIRECTINPUT8 m_pDirectXInput;
	static LPDIRECTINPUTDEVICE8 m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MAX_INPUT_DEVICES];
	static LPD3DXFONT m_pDirectXFont;
	static D3DPRESENT_PARAMETERS m_DirectXPresentParam;
	static DirectX* m_pDirectXClass;

private:
};

class DirectX
{
public:
	static DirectX* GetInstancePointer();
	DirectXObject m_DirectXObject;
	DirectX3DDevice m_DirectX3DDevice;
	DirectXInputDevices m_DirectXInputDevices;
	static HWND* m_pHWnd;

private:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectX)
};

//ポリゴンの頂点情報
struct CustomVertex
{
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_z;
	DWORD m_aRGB;
	FLOAT m_tu;
	FLOAT m_tv;
};
