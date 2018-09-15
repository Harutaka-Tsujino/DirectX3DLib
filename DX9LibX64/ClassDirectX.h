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

class DirectX
{
public:
	static DirectX* GetInstancePointer();
	VOID Init(HWND* hWnd, BOOL window, BOOL m_cullPolygon, DWORD d3DFVF);
	HRESULT InitDirectX3DDevice();
	HRESULT InitDirectXInputDevices();
	HRESULT InitDirectX();
	VOID GetInputStates();
	VOID UpdateInputStates();
	VOID PrepareRender();
	VOID CleanUpRender();

	BOOL m_window;
	KeyBoardState m_keyBoardState;
	MouseState m_mouseState;
	HWND* m_pHWnd;

private:
	CREATE_CONSTRACTA_AND_DESTRACTA(DirectX)
	VOID SetBuckBufferOverall();
	VOID SetRenderState();
	VOID SetTextureStageState();

	static DirectX* m_pDirectXClass;
	LPDIRECT3D9 m_pDirectX;
	LPDIRECT3DDEVICE9 m_pDirectX3DDevice;
	LPDIRECTINPUT8 m_pDirectXInput;
	LPDIRECTINPUTDEVICE8 m_pDirectXInputDevices[(ULONGLONG)INPUT_DEVICES::MAX_INPUT_DEVICES];
	LPD3DXFONT m_pDirectXFont;
	D3DPRESENT_PARAMETERS m_DirectXPresentParam;
	BOOL m_cullPolygon;
	DWORD m_d3DFVF;
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
