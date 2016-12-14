#pragma once

#include "Header.h"

//-----------------------------------------------------------------------------
// Direct Input ������
//-----------------------------------------------------------------------------
bool InitDInput(HINSTANCE hInstApp)
{
	HRESULT		hr;

	hr = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDInput, NULL);
	if (FAILED(hr))
		return false;	// DirectInput8�̍쐬�Ɏ��s

	return true;

}

//-----------------------------------------------------------------------------
// Direct Input �J������
//-----------------------------------------------------------------------------
void ReleaseDInput(void)
{
	// DirectInput�̃f�o�C�X���J��
	if (g_lpDIKeyboard) g_lpDIKeyboard->Unacquire();
	RELEASE(g_lpDIKeyboard);
	if (g_lpDIMouse) g_lpDIMouse->Unacquire();
	RELEASE(g_lpDIMouse);
	RELEASE(g_lpDInput);

}

//-----------------------------------------------------------------------------
// �L�[�{�[�h�p�I�u�W�F�N�g�쐬
//-----------------------------------------------------------------------------
bool InitKeyboard(void)
{
	HRESULT		hr;

	//�L�[�{�[�h�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	hr = g_lpDInput->CreateDevice(GUID_SysKeyboard, &g_lpDIKeyboard, NULL);
	if (FAILED(hr))
		return false;  // �f�o�C�X�̍쐬�Ɏ��s

					   //�L�[�{�[�h�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = g_lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return false; // �f�o�C�X�̍쐬�Ɏ��s

					  //���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_lpDIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
		return false; // ���[�h�̐ݒ�Ɏ��s

					  //�L�[�{�[�h���͐���J�n
	g_lpDIKeyboard->Acquire();

	return true;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetKeyboardState()
// �@�\�T�v�F�@�L�[�{�[�h�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void GetKeyboardState(void)
{
	HRESULT			hr;

	if (g_lpDIKeyboard == NULL) return;

	hr = g_lpDIKeyboard->GetDeviceState(256, g_diKeyState);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
		hr = g_lpDIKeyboard->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = g_lpDIKeyboard->Acquire();
	}

}

//-----------------------------------------------------------------------------
// �}�E�X�p�I�u�W�F�N�g�쐬
//-----------------------------------------------------------------------------
bool InitMouse(void)
{
	HRESULT		hr;

	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	hr = g_lpDInput->CreateDevice(GUID_SysMouse, &g_lpDIMouse, NULL);
	if (FAILED(hr))
		return false;	// �f�o�C�X�̍쐬�Ɏ��s

						// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_lpDIMouse->SetDataFormat(&c_dfDIMouse);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if (FAILED(hr))
		return false;	// �f�[�^�t�H�[�}�b�g�Ɏ��s

						// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_lpDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
		return false;	// ���[�h�̐ݒ�Ɏ��s

						// �f�o�C�X�̐ݒ�
	DIPROPDWORD					diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j
	hr = g_lpDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hr))
		return false;	// �f�o�C�X�̐ݒ�Ɏ��s

						// ���͐���J�n
	g_lpDIMouse->Acquire();

	return true;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetMouseState()
// �@�\�T�v�F�@�}�E�X�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void GetMouseState(void)
{
	HRESULT			hr;

	hr = g_lpDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_diMouseState);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
		hr = g_lpDIMouse->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = g_lpDIMouse->Acquire();
	}

	/*
	#if DEBUG
	char buf[80];
	wsprintf(buf, "(%d, %d, %d) %s %s %s",
	g_diMouseState.lX,
	g_diMouseState.lY,
	g_diMouseState.lZ,
	(g_diMouseState.rgbButtons[0] & 0x80) ? "Left" : "--",
	(g_diMouseState.rgbButtons[1] & 0x80) ? "Right" : "--",
	(g_diMouseState.rgbButtons[2] & 0x80) ? "Center" : "--");
	std::cout << buf << std::endl;
	#endif
	*/

}