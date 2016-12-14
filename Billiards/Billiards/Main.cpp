#include "Header.h"
#include "GetBillBoardRotation.h"
#include "MyD3D.h"
#include "ScreenViewChange.h"
#include "Level.h"
#include "Input.h"
#include "MSTATE.h"
#include "MeshData.h"
#include "GoalCylinder.h"
#include "BallData.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"

//-----------------------------------------------------------------
//    Main.
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevinst, LPSTR nCmdLine, int nCmdShow)
{

	ZeroMemory(&msg, sizeof(msg));  //msg初期化

	hWnd = InitApp(hInst, nCmdShow);
	if (!hWnd) return FALSE;

	if (!InitDirect3D(hWnd)) return FALSE;

	if (!InitFont(hWnd)) return FALSE;

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			RenderDirect3D();
		}
		Sleep(1);
	}

	return msg.wParam;
}



//-----------------------------------------------------------------
//    Initialize Application.
//-----------------------------------------------------------------
HWND InitApp(HINSTANCE hInst, int nCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;

	char szClassName[] = "Direct3D Test";

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = szClassName;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	if (!RegisterClass(&wc)) return FALSE;

	hWnd = CreateWindow(szClassName, "Billiards", WS_OVERLAPPEDWINDOW,
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInst, NULL);
	if (!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// DirectInputオブジェクトの初期化
	if (FAILED(InitDInput(hInst))) return (FALSE);

	// Keyboard Object の初期化
	if (FAILED(InitKeyboard())) return (FALSE);

	// Mouse Object の初期化
	if (FAILED(InitMouse())) return (FALSE);

	//------------------------------------------------------- マウスポインタ情報の初期化
	SetRect(&MState.moveRect, 10, 10, 630, 470);	// マウスカーソルの動く範囲
	MState.x = MState.moveRect.left;	// マウスカーソルのＸ座標を初期化
	MState.y = MState.moveRect.top;	// マウスカーソルのＹ座標を初期化
	MState.lButton = false;	// 左ボタンの情報を初期化
	MState.rButton = false;	// 右ボタンの情報を初期化
	MState.cButton = false;	// 中央ボタンの情報を初期化
	MState.moveAdd = 2;	// マウスカーソルの移動量を設定

	isClick = false;
	isTitleClick = false;
	isResult = false;

	return hWnd;
}



//-----------------------------------------------------------------
//    Initialize Direct3D.
//-----------------------------------------------------------------
BOOL InitDirect3D(HWND hWnd)
{
	D3DPRESENT_PARAMETERS d3dpp;
	HRESULT hr;

	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
		MessageBox(hWnd, "Can't create Direct3D.", "Error", MB_OK);
		return FALSE;
	}
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice);
	if (FAILED(hr)) {
		MessageBox(hWnd, "Can't create device.", "Error", MB_OK);
		return FALSE;
	}

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	InitParameter();

	return TRUE;
}

//-----------------------------------------------------------------
//    Initialize Objects
//-----------------------------------------------------------------
void InitParameter() {
	/*Mesh Load*/
	Table.LoadMeshData(".\\table.x");
	Shop.LoadMeshData(".\\Shop.x");
	Ball[0].LoadData(".\\1.x", D3DXVECTOR3(0.795f, 0.976f, 0.0f));
	Ball[1].LoadData(".\\2.x", D3DXVECTOR3(0.925f, 0.976f, -0.072f));
	Ball[2].LoadData(".\\3.x", D3DXVECTOR3(1.055f, 0.976f, 0.0f));
	Ball[3].LoadData(".\\4.x", D3DXVECTOR3(0.925f, 0.976f, 0.072f));
	Ball[4].LoadData(".\\5.x", D3DXVECTOR3(0.86f, 0.976f, -0.036f));
	Ball[5].LoadData(".\\6.x", D3DXVECTOR3(0.86f, 0.976f, 0.036f));
	Ball[6].LoadData(".\\7.x", D3DXVECTOR3(0.99f, 0.976f, -0.036f));
	Ball[7].LoadData(".\\8.x", D3DXVECTOR3(0.99f, 0.976f, 0.036f));
	Ball[8].LoadData(".\\9.x", D3DXVECTOR3(0.925f, 0.976f, 0.0f));
	hand.LoadData(".\\hand.x", D3DXVECTOR3(-1.0f, 0.976f, 0.0f));
	Goal[0].LoadData(D3DXVECTOR3(-1.65f, 0.9f,  0.85f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.7f, 0.7f, 0.2f);
	Goal[1].LoadData(D3DXVECTOR3( 1.65f, 0.9f,  0.85f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.7f, 0.7f, 0.2f);
	Goal[2].LoadData(D3DXVECTOR3(-1.65f, 0.9f, -0.85f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.7f, 0.7f, 0.2f);
	Goal[3].LoadData(D3DXVECTOR3( 1.65f, 0.9f, -0.85f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.7f, 0.7f, 0.2f);
	Goal[4].LoadData(D3DXVECTOR3(  0.0f, 0.9f, -0.9f ), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.7f, 0.7f, 0.2f);
	Goal[5].LoadData(D3DXVECTOR3(  0.0f, 0.9f,  0.9f ), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.7f, 0.7f, 0.2f);
	Ray[0].LoadData(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.1f, 0.1f, 1.0f);
	Ray[1].LoadData(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.1f, 0.1f, 1.0f);

	theta = M_PI;
	lookheight = 0.0f;
	NextBallNumber = 0;
	isFirstConflict = false;
	isClickAfter = false;

	for (int i = 0; i < 9; i++) {
		isFALL[i] = FALSE;
	}
}

//-----------------------------------------------------------------
//    Initialize Font
//-----------------------------------------------------------------
BOOL InitFont(HWND hWnd)
{
	//フォントの生成 MSゴシック
	//デバイス,文字高さ,文字幅,フォントスタイル,ミップマップのレベル,斜体にするかどうか,文字セット,出力精度,出力品質,フォントピッチとファミリ,フォント名,フォントポインタ
	HRESULT hr = D3DXCreateFont(g_pd3dDevice, 40, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "ＭＳ ゴシック", &g_pFont);
	if FAILED(hr) { return(E_FAIL); }
	return TRUE;
}


//-----------------------------------------------------------------
//    Cleanup Direct3D.
//-----------------------------------------------------------------
BOOL CleanupDirect3D()
{
	Table.CleanupMesh();
	Shop.CleanupMesh();

	for (int i = 0; i < 9; i++) {
		Ball[i].Ball.CleanupMesh();
	}
	hand.Ball.CleanupMesh();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();

	if (g_pFont != NULL)
		g_pFont->Release();

	// DirectInputオブジェクトの開放
	ReleaseDInput();

	return TRUE;
}


//-----------------------------------------------------------------
//    Window Proc.
//-----------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_DESTROY:
		CleanupDirect3D();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}

	return 0L;
}


//-----------------------------------------------------------------
//    Setup Matrices.
//-----------------------------------------------------------------
BOOL SetupMatrices()
{
	D3DXMATRIXA16 matWorld, matView, matProj;

	// Tilteクリックが押されていないと, カメラ回転
	if (!isTitleClick) {
		D3DXMatrixRotationY(&matWorld, (float)(270.0f * M_PI / 180.0f));
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		
		float thetaT = fmod((timeGetTime())/60.0f, 360.0f) * M_PI / 180.0f;		
		vEyePt.x = 1.5f*cos(thetaT);
		vEyePt.y = 2.5f;
		vEyePt.z = 1.5f*sin(thetaT);
		vLookatPt.x = 0.0f;
		vLookatPt.y = 0.0f;
		vLookatPt.z = 0.0f;
		vUpVec.x = 0.0f;
		vUpVec.y = 1.0f;
		vUpVec.z = 0.0f;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	}

	// 動いているボールがなければプレイヤー目線
	if (!ExistsMoveBall() && isTitleClick) {
		// World Matrix.
		//D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
		D3DXMatrixRotationY(&matWorld, (float)(270 * M_PI / 180));
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// Camera.
		//vEyePt.x = 0.0f - 1.9f;
		//vEyePt.y = 1.2f;
		//vEyePt.z = 0.0f;

		/*
		float theta = fmod((timeGetTime())/30.0f, 360.0f) * M_PI / 180.0f;
		*/
		vEyePt.x = 1.2f*cos(theta) + hand.Pos.x;
		vEyePt.y = 1.4f + lookheight;
		vEyePt.z = 1.2f*sin(theta) + hand.Pos.z;
		vLookatPt = hand.Pos;
		vUpVec.x = 0.0f;
		vUpVec.y = 1.5f;
		vUpVec.z = 0.0f;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	}
	// 動いているボールがあれば全体表示
	else if(ExistsMoveBall() && isTitleClick) {
		D3DXMatrixRotationY(&matWorld, (float)(90.0f * M_PI / 180.0f));
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		vEyePt.x = 0.0f;
		vEyePt.y = 2.5f;
		vEyePt.z = 0.0f;
		vLookatPt.x = 0.0f;
		vLookatPt.y = 0.0f;
		vLookatPt.z = 0.0f;
		vUpVec.x = 0.0f;
		vUpVec.y = 0.0f;
		vUpVec.z = 1.0f;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	}
	// Projection Matrix.
	D3DXMatrixPerspectiveFovLH(&matProj, (float)(60 * M_PI / 180), SCREEN_WIDTH / SCREEN_HEIGHT, 0.5f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	return TRUE;
}

//-----------------------------------------------------------------
//    Render Direct3D.
//-----------------------------------------------------------------
BOOL RenderDirect3D()
{
	D3DXVECTOR3 center, position;
	POINT MousePoint;
	

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0x00000000, 1.0f, 0);

	g_pd3dDevice->BeginScene();

	SetupMatrices();

	// マウスの状態を取得
	GetMouseState();
	GetCursorPos(&MousePoint);

	/* キーボードの状態を取得 */
	GetKeyboardState();


	// 取得した情報を元にマウスの情報を更新
	MState.x = MousePoint.x;
	MState.y = MousePoint.y;
	(g_diMouseState.rgbButtons[0] & 0x80) ? MState.lButton = true : MState.lButton = false;
	(g_diMouseState.rgbButtons[1] & 0x80) ? MState.rButton = true : MState.rButton = false;
	(g_diMouseState.rgbButtons[2] & 0x80) ? MState.cButton = true : MState.cButton = false;

	if (!isTitleClick && !isResult) {
		TitleScene();
	}else if(isTitleClick && !isResult){
		GameScene();
	}else {
		ResultScene();
	}

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	return TRUE;
}
