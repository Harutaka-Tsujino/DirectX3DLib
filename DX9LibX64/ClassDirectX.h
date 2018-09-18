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

//DirectX�̏������C���^�[�t�F�C�X 3D�ɑΉ������Ƃ���Bridge���s����������
class IDirectXObjectInitializer
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(IDirectXObjectInitializer)
		virtual HRESULT Initialize(BOOL window) = 0;

private:
};

//DirectX�I�u�W�F�N�g�̏������@�\�ǉ����� ���������ɒǉ��������������Ȃ����Ƃɂ���ďC���ύX���e�ՂɂȂ�
class DirectXObjectInitializer :public IDirectXObjectInitializer
{
public:
	HRESULT Initialize(BOOL window);
private:
	VOID SetBuckBufferOverall(BOOL window);
};

class DirectXObject
{
public:
	DirectXObject();
	~DirectXObject() {};
	HRESULT Initialize();
	VOID SetWindowMode(BOOL window);

	IDirectXObjectInitializer* m_pDirectXObjectInitializer;
private:
	BOOL m_window;
};

typedef DWORD t_VERTEX_FORMAT;

class IDirectX3DDeviceInitializer
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(IDirectX3DDeviceInitializer)
		virtual HRESULT Initialize(t_VERTEX_FORMAT d3DFVF, BOOL cullPollygon) = 0;

private:
};

class DirectX3DDeviceInitializer:public IDirectX3DDeviceInitializer
{
public:
	HRESULT Initialize(t_VERTEX_FORMAT d3DFVF, BOOL cullPollygon);
private:
	VOID SetRenderState(BOOL cullPollygon);
	VOID SetTextureStageState();
};

class DirectX3DDevice
{
public:
	DirectX3DDevice();
	~DirectX3DDevice() {};
	HRESULT Initialize();
	VOID SetCullPolygon(BOOL cullPolygon);
	VOID SetVertexFormat(t_VERTEX_FORMAT d3DFVF);
	VOID PrepareRender();
	VOID CleanUpRender();

	IDirectX3DDeviceInitializer* m_pDirectX3DDeviceInitializer;
private:
	BOOL m_cullPolygon;
	t_VERTEX_FORMAT m_d3DFVF;
};

class IDirectXInputDevicesInitializer
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(IDirectXInputDevicesInitializer)
	virtual HRESULT Initialize() = 0;

private:
};

class DirectXInputDevicesInitializer :public IDirectXInputDevicesInitializer
{
public:
	HRESULT Initialize();
private:
};

class IInputStatesGetter
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(IInputStatesGetter)
	virtual VOID Get(InputData& rInputData)=0;

private:
};

class MouseAndKeyBoardStatesGetter:public IInputStatesGetter
{
public:
	VOID Get(InputData& rInputData);

private:
};

class IInputStatesUpdater
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(IInputStatesUpdater)
	virtual VOID Update(InputData& rInputData) = 0;

private:
};

class MouseAndKeyBoardStatesUpdater :public IInputStatesUpdater
{
private:
	VOID Update(InputData& rInputData);

private:
};

class InputData
{
public:
	KeyBoardState m_keyBoardState;
	MouseState m_mouseState;

private:
};

class DirectXInputDevices
{
public:
	DirectXInputDevices();
	~DirectXInputDevices() {};
	HRESULT Initialize();
	VOID GetInputStates();
	VOID UpdateInputStates();

	IDirectXInputDevicesInitializer* m_pDirectXInputDevicesInitializer;
	IInputStatesGetter* m_pInputStatesGetter;
	IInputStatesUpdater* m_pInputStatesUpdater;
	InputData m_InputData;
private:
};

class DirectXInstances
{
public:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectXInstances)
	static HWND* m_pHWnd;
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
