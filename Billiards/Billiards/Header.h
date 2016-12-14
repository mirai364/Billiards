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
#include "MyD3D.h"
#include "CalcParticleColliAfterPos.h"

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
BOOL	InitFont(HWND hWnd);
BOOL    CleanupDirect3D();
BOOL    SetupMatrices();
BOOL    RenderDirect3D();
void	InitParameter();
bool	InitDInput(HINSTANCE hInstApp);
void	ReleaseDInput(void);
bool	InitKeyboard(void);
void	GetKeyboardState(void);
bool	InitMouse(void);
void	GetMouseState(void);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
D3DXVECTOR3* ScreenViewChanger(float x, float y, D3DXVECTOR3* pout);
BOOL	ExistsMoveBall();
void	GameScene();
void	TitleScene();
void	ResultScene();
void	GameToResult();
void	ResultToTitle();
void	TitleToGame();



//-----------------------------------------------------------------
//    Grobal Variables.
//-----------------------------------------------------------------
const double Gravity = 9.8;
const int SCREEN_WIDTH = 1680;	// �E�B���h�E�̕�
const int SCREEN_HEIGHT = 768;	// �E�B���h�E�̍���

LPD3DXMESH              pMesh;
LPD3DXBUFFER			pMaterial;
DWORD					dwNumMaterials;
D3DMATERIAL9*			pMeshMaterials;
LPDIRECT3DTEXTURE9*		pMeshTextures;
D3DXMATERIAL*			d3dxMaterials;
MyD3D*                  myd3d = NULL;
BOOL					isFALL[9];

MSG		msg;          // ���b�Z�[�W
float	theta;		  //xz�������_�I��
float	lookheight;   //y�������_����
BOOL	isClick;
float	ClickLength;
D3DXVECTOR3 vEyePt, vLookatPt, vUpVec;

HWND	hWnd;
WNDCLASS wc;
extern BYTE gl_KeyTbl[256];                 // �L�[�{�[�h�̏�Ԃ��i�[

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
LPD3DXFONT				g_pFont = NULL; //�t�H���g

BOOL	isTitleClick;
BOOL	isResult;
int		NextBallNumber;
BOOL	isFirstConflict;
BOOL	isClickAfter;


