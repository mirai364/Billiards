#pragma once

#define _USE_MATH_DEFINES
#define IKD_EPSIRON 0.00001f	// �덷
#define RELEASE(x) if(x!=NULL){x->Release();x=NULL;} // �I�u�W�F�N�g�̊J��
#define DEBUG true          // �f�o�b�O�E���[�h�i�g��Ȃ��Ƃ��̓R�����g�A�E�g�j

#include <stdio.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include <iostream>
#include <dinput.h>     // DirectInput���g�p�\�ɂ���
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
MSG		msg;          // ���b�Z�[�W
float	theta;		  //xz�������_�I��
float	lookheight;   //y�������_����

HWND	hWnd;
WNDCLASS wc;
extern BYTE gl_KeyTbl[256];                 // �L�[�{�[�h�̏�Ԃ��i�[

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;


