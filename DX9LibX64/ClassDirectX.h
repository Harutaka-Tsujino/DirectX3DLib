#pragma once
#include<windows.h>
#include<d3dx9.h>
#include <dinput.h>
#include"DX9LibX64.h"

//���̓f�o�C�X
enum class INPUT_DEVICES
{
	KEY_BOARD,
	MOUSE,
	MAX_INPUT_DEVICES
};

typedef struct
{
	//���݂Ńt���[���̃L�[�{�[�h�̃L�[��������Ă����ꍇ��ʃr�b�g������
	BYTE m_diks[256];

	//�O��̂Ńt���[���̃L�[�{�[�h�̃L�[��������Ă����ꍇ��ʃr�b�g������
	BYTE m_prevDiks[256];

	//�L�[�{�[�h�̃L�[�������ꂽ�u�Ԃ̃t���[������TRUE
	BOOL m_keyPush[256];

	//�L�[�{�[�h�̃L�[�������ꑱ�����Ă����TRUE
	BOOL m_keyHold[256];

	//�L�[�{�[�h�̃L�[�������ꂽ�u�Ԃ̃t���[������TRUE
	BOOL m_keyRelease[256];

	//�L�[�{�[�h�̃L�[���O��ƌ��݂̃t���[���ŉ�����Ă��Ȃ���TRUE
	BOOL m_keyUninput[256];
}KeyBoardState;

//�}�E�X�̃J�[�\���̈ʒu�{�^���̓��͏�Ԃ�ێ�����
typedef struct
{
	//���݂̃t���[���ł̑O��̃J�[�\���ʒu����̑��Έʒu�ƃ{�^���̓��͏��
	DIMOUSESTATE m_directInputMouseState;

	//�O��̃t���[���ł̑O��̃J�[�\���ʒu����̑��Έʒu�ƃ{�^���̓��͏��
	DIMOUSESTATE m_prevDirectInputMouseState;

	//�J�[�\���̐�΍��W
	POINT m_absolutePos;

	//�}�E�X�̃{�^���������ꂽ�u�Ԃ̃t���[������TRUE
	BOOL m_buttonPush[4];

	//�}�E�X�̃{�^���������ꑱ�����Ă����TRUE
	BOOL m_buttonHold[4];

	//�}�E�X�̃{�^���������ꂽ�u�Ԃ̃t���[������TRUE
	BOOL m_buttonRelease[4];

	//�}�E�X�̃{�^�����O��ƌ��݂̃t���[���ŉ�����Ă��Ȃ���TRUE
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

//�|���S���̒��_���
struct CustomVertex
{
	FLOAT m_x;
	FLOAT m_y;
	FLOAT m_z;
	DWORD m_aRGB;
	FLOAT m_tu;
	FLOAT m_tv;
};
