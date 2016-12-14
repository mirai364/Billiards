#pragma once

#include "Header.h"

//-----------------------------------------------------------------------------
// Direct Input 初期化
//-----------------------------------------------------------------------------
bool InitDInput(HINSTANCE hInstApp)
{
	HRESULT		hr;

	hr = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDInput, NULL);
	if (FAILED(hr))
		return false;	// DirectInput8の作成に失敗

	return true;

}

//-----------------------------------------------------------------------------
// Direct Input 開放処理
//-----------------------------------------------------------------------------
void ReleaseDInput(void)
{
	// DirectInputのデバイスを開放
	if (g_lpDIKeyboard) g_lpDIKeyboard->Unacquire();
	RELEASE(g_lpDIKeyboard);
	if (g_lpDIMouse) g_lpDIMouse->Unacquire();
	RELEASE(g_lpDIMouse);
	RELEASE(g_lpDInput);

}

//-----------------------------------------------------------------------------
// キーボード用オブジェクト作成
//-----------------------------------------------------------------------------
bool InitKeyboard(void)
{
	HRESULT		hr;

	//キーボード用にデバイスオブジェクトを作成
	hr = g_lpDInput->CreateDevice(GUID_SysKeyboard, &g_lpDIKeyboard, NULL);
	if (FAILED(hr))
		return false;  // デバイスの作成に失敗

					   //キーボード用のデータ・フォーマットを設定
	hr = g_lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return false; // デバイスの作成に失敗

					  //モードを設定（フォアグラウンド＆非排他モード）
	hr = g_lpDIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
		return false; // モードの設定に失敗

					  //キーボード入力制御開始
	g_lpDIKeyboard->Acquire();

	return true;

}

//-----------------------------------------------------------------------------
// 関数名　：　GetKeyboardState()
// 機能概要：　キーボードの状態を取得
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
// マウス用オブジェクト作成
//-----------------------------------------------------------------------------
bool InitMouse(void)
{
	HRESULT		hr;

	// マウス用にデバイスオブジェクトを作成
	hr = g_lpDInput->CreateDevice(GUID_SysMouse, &g_lpDIMouse, NULL);
	if (FAILED(hr))
		return false;	// デバイスの作成に失敗

						// データフォーマットを設定
	hr = g_lpDIMouse->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if (FAILED(hr))
		return false;	// データフォーマットに失敗

						// モードを設定（フォアグラウンド＆非排他モード）
	hr = g_lpDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
		return false;	// モードの設定に失敗

						// デバイスの設定
	DIPROPDWORD					diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
	hr = g_lpDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hr))
		return false;	// デバイスの設定に失敗

						// 入力制御開始
	g_lpDIMouse->Acquire();

	return true;

}

//-----------------------------------------------------------------------------
// 関数名　：　GetMouseState()
// 機能概要：　マウスの状態を取得
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