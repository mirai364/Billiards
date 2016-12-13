#pragma once

#define _USE_MATH_DEFINES
#define IKD_EPSIRON 0.00001f	// 誤差
#define RELEASE(x) if(x!=NULL){x->Release();x=NULL;} // オブジェクトの開放
#define DEBUG true          // デバッグ・モード（使わないときはコメントアウト）

#include <stdio.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include <iostream>
#include <dinput.h>     // DirectInputを使用可能にする
#include <tchar.h>
#include <mmsystem.h>
#include <stdio.h>
#include <commdlg.h>
#include <wingdi.h>

#include "NKC_DInput.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"msimg32.lib")

//-----------------------------------------------------------------
//    Prototypes.
//-----------------------------------------------------------------
HWND    InitApp(HINSTANCE, int);
BOOL    InitDirect3D(HWND);
BOOL    CleanupDirect3D();
BOOL    SetupMatrices();
BOOL    RenderDirect3D();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
D3DXVECTOR3* ScreenViewChanger(float x, float y, D3DXVECTOR3* pout);
MSG		msg;          // メッセージ
float	theta;		  //xz方向視点選択
float	lookheight;   //y方向視点高さ

HWND	hWnd;
WNDCLASS wc;
extern BYTE gl_KeyTbl[256];                 // キーボードの状態を格納

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;


