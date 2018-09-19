#pragma once
#include <windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include "DX9LibX64.h"

//入力デバイス列挙体
enum class INPUT_DEVICES
{
	//キーボード
	KEYBOARD,

	//マウス
	MOUSE,

	//入力デバイスの種類の最大数
	MAX_INPUT_DEVICES
};

//キーボードの入力状態構造体
struct KeyBoardState
{
public:

	//現在でフレームのキーボードのキーが押されていた場合上位ビットが立つ
	BYTE m_diks[256];

	//前回のでフレームのキーボードのキーが押されていた場合上位ビットが立つ
	BYTE m_diksPrev[256];

	//キーボードのキーが押された瞬間のフレームだけTRUE
	BOOL m_keyPush[256];

	//キーボードのキーが押され続けられている間TRUE
	BOOL m_keyHold[256];

	//キーボードのキーが離された瞬間のフレームだけTRUE
	BOOL m_keyRelease[256];

	//キーボードのキーが前回と現在のフレームで押されていない間TRUE
	BOOL m_keyUninput[256];
};

//マウスのカーソルの位置ボタンの入力状態構造体
struct MouseState
{
public:

	//現在のフレームでの前回のカーソル位置からの相対位置とボタンの入力状態
	DIMOUSESTATE m_mouseState;

	//前回のフレームでの前回のカーソル位置からの相対位置とボタンの入力状態
	DIMOUSESTATE m_mouseStatePrev;

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
};

//インターフェイスを用いることによって機能実装部分と機能追加部分で切り離すことができる
//それにより新しく機能を拡張する場合に修正が楽になる

//DirectXのオブジェクト初期化インターフェイス
class IDirectXObjectInitializer
{
public:
	IDirectXObjectInitializer() {};
	~IDirectXObjectInitializer() {};

	virtual HRESULT Initialize(BOOL canWindow) = 0;
};

//DirectXのオブジェクト初期化クラス
class DirectXObjectInitializer :public IDirectXObjectInitializer
{
public:
	HRESULT Initialize(BOOL canWindow);

private:
	VOID SetBuckBuffer(BOOL canWindow);
};

//DirectXのオブジェクトクラス
class DirectXObject
{
public:
	DirectXObject() :m_canWindow(TRUE) {};
	~DirectXObject() {};

	HRESULT Initialize();
	VOID SetWindowMode(BOOL canWindow);

private:
	BOOL m_canWindow;
	IDirectXObjectInitializer* m_pDirectXObjectInitializer;
};

//ポリゴンの頂点データ
typedef DWORD t_VERTEX_FORMAT;

//DirectXの3Dデバイス初期化インターフェイス
class IDirectX3DDeviceInitializer
{
public:
	IDirectX3DDeviceInitializer() {};
	~IDirectX3DDeviceInitializer() {};

	virtual HRESULT Initialize(t_VERTEX_FORMAT d3DFVF, BOOL canCullPolygon) = 0;

private:
};

//DirectXの3Dデバイス初期化クラス
class DirectX3DDeviceInitializer :public IDirectX3DDeviceInitializer
{
public:
	HRESULT Initialize(t_VERTEX_FORMAT d3DFVF, BOOL canCullPolygon);

private:
	VOID SetRenderState(BOOL canCullPolygon);
	VOID SetTextureStageState();
};

//DirectXの3Dデバイスクラス
class DirectX3DDevice
{
public:
	DirectX3DDevice() :m_canCullPolygon(TRUE), m_d3DFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE) {};
	~DirectX3DDevice() {};

	HRESULT Initialize();
	VOID SetCullPolygon(BOOL canCullPolygon);
	VOID SetVertexFormat(t_VERTEX_FORMAT d3DFVF);
	VOID PrepareRender();
	VOID CleanUpRender();

private:
	BOOL m_canCullPolygon;
	t_VERTEX_FORMAT m_d3DFVF;
	IDirectX3DDeviceInitializer* m_pDirectX3DDeviceInitializer;
};

//DirectXのXInput初期化インターフェイス
class IDirectXInputDevicesInitializer
{
public:
	IDirectXInputDevicesInitializer() {};
	~IDirectXInputDevicesInitializer() {};

	virtual HRESULT Initialize() = 0;
};

//DirectXInput初期化クラス
class DirectXInputDevicesInitializer :public IDirectXInputDevicesInitializer
{
public:
	HRESULT Initialize();
};

//入力デバイスのデータ構造体
struct InputData
{
public:
	KeyBoardState m_keyBoardState;
	MouseState m_mouseState;
};

//入力デバイスデータ保存インターフェイス
class IInputStatesStorage
{
public:
	IInputStatesStorage() {};
	~IInputStatesStorage() {};

	virtual VOID Store(InputData& rInputData) = 0;
};

//マウスキーボード の入力データ保存クラス
class MouseAndKeyboardStatesStorage :public IInputStatesStorage
{
public:
	VOID Store(InputData& rInputData);
};

//入力デバイスデータ取得インターフェイス
class IInputStatesGetter
{
public:
	IInputStatesGetter() {};
	~IInputStatesGetter() {};

	virtual VOID Get(InputData& rInputData) = 0;
};

//マウスキーボード の入力データ取得クラス
class MouseAndKeyboardStatesGetter :public IInputStatesGetter
{
public:
	VOID Get(InputData& rInputData);
};

//DirectXInputクラス
class DirectXInputDevices
{
public:
	DirectXInputDevices();
	~DirectXInputDevices() {};
	HRESULT Initialize();
	VOID StoreInputStates();
	VOID GetInputStates();

	InputData m_InputData;

private:
	IDirectXInputDevicesInitializer * m_pDirectXInputDevicesInitializer;
	IInputStatesStorage* m_pInputStatesStoreter;
	IInputStatesGetter* m_pInputStatesGetter;
};

class DirectX;

//DirectX関係のインスタンス構造体
struct DirectXInstances
{
public:
	HWND* m_pHWnd;
	LPDIRECT3D9 m_pDirectX;
	LPDIRECT3DDEVICE9 m_pDirectX3DDevice;
	LPDIRECTINPUT8 m_pDirectXInput;
	LPDIRECTINPUTDEVICE8 m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MAX_INPUT_DEVICES];
	LPD3DXFONT m_pDirectXFont;
	D3DPRESENT_PARAMETERS m_DirectXPresentParam;
	DirectX* m_pDirectXClass;
};

//DirectXクラス
class DirectX
{
public:
	static DirectX* GetInstance();
	VOID DeleteInstance();
	VOID SetHWND(HWND* pHWnd);

	DirectXObject m_DirectXObject;
	DirectX3DDevice m_DirectX3DDevice;
	DirectXInputDevices m_DirectXInputDevices;
	static DirectXInstances m_directXInstances;

private:
	DirectX() {};
	~DirectX() {};
};

//ポリゴンの頂点情報構造体
struct CustomVertex
{
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_z;
	DWORD m_aRGB;
	FLOAT m_tu;
	FLOAT m_tv;
};
