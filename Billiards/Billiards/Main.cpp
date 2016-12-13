#include "Header.h"
#include "CalcParticleColliAfterPos.h"
#include "GetBillBoardRotation.h"
#include "MyD3D.h"
#include "ScreenViewChange.h"

//-----------------------------------------------------------------
//    Grobal Variables.
//-----------------------------------------------------------------

const double Gravity = 9.8;
const int SCREEN_WIDTH = 1680;	// ウィンドウの幅
const int SCREEN_HEIGHT = 768;	// ウィンドウの高さ

LPD3DXMESH              pMesh;
LPD3DXBUFFER			pMaterial;
DWORD					dwNumMaterials;
D3DMATERIAL9*			pMeshMaterials;
LPDIRECT3DTEXTURE9*		pMeshTextures;
D3DXMATERIAL*			d3dxMaterials;
MyD3D*                  myd3d = NULL;
BOOL					isFALL[9];

//-----------------------------------------------------------------
//    Struct.
//-----------------------------------------------------------------
struct MSTATE {
    RECT    moveRect;     // 画面上で動ける範囲
    int     x;            // X座標
    int     y;            // Y座標
    bool    lButton;      // 左ボタン
    bool    rButton;      // 右ボタン
    bool    cButton;      // 真ん中ボタン
    int     moveAdd;      // 移動量
}MState;

struct MeshData{
	LPD3DXMESH              pMesh;
	LPD3DXBUFFER			pMaterial;
	DWORD					dwNumMaterials;
	D3DMATERIAL9*			pMeshMaterials;
	LPDIRECT3DTEXTURE9*		pMeshTextures;
	D3DXMATERIAL*			d3dxMaterials;
public:
	MeshData() {			/* constructor	*/
		ZeroMemory(&pMaterial, sizeof(pMaterial));  //material初期化
		pMesh = NULL;
		dwNumMaterials = NULL;
		pMeshMaterials = NULL;
		pMeshTextures = NULL;
	}
	BOOL LoadMeshData(char* file) {
		// Load model data
		D3DXLoadMeshFromX(file, D3DXMESH_SYSTEMMEM, g_pd3dDevice,
			NULL, &pMaterial, NULL, &dwNumMaterials, &pMesh);

		// Material load
		d3dxMaterials = (D3DXMATERIAL*)pMaterial->GetBufferPointer();
		pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
		pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

		for (DWORD i = 0; i<dwNumMaterials; i++)
		{
			pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
			pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;
			pMeshTextures[i] = NULL;

			if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0) {
				D3DXCreateTextureFromFile(g_pd3dDevice, d3dxMaterials[i].pTextureFilename, &pMeshTextures[i]);
			}
		}
		return TRUE;
	}
	BOOL CleanupMesh() {
		if (pMesh != NULL)
			pMesh->Release();
		if (pMaterial != NULL)
			pMaterial->Release();
		if (dwNumMaterials != NULL)
			dwNumMaterials = NULL;
		if (pMeshMaterials != NULL)
			pMeshMaterials = NULL;
		if (pMeshTextures != NULL)
			pMeshTextures = NULL;
		if (d3dxMaterials != NULL)
			d3dxMaterials = NULL;

		return TRUE;
	}
	BOOL RenderMesh() {
		for (DWORD i = 0; i < dwNumMaterials; i++)
		{
			g_pd3dDevice->SetMaterial(&pMeshMaterials[i]);
			g_pd3dDevice->SetTexture(0, pMeshTextures[i]);
			pMesh->DrawSubset(i);
		}
		return TRUE;
	}

}Table, Shop;

struct GoalCylinder {
	LPD3DXMESH              pMesh;
	D3DXVECTOR3				Pos;						// 円柱座標
	D3DMATERIAL9			d3dxMaterials;
public:
	GoalCylinder() {			/* constructor	*/
		pMesh = NULL;
	}
	BOOL LoadData(D3DXVECTOR3 _Pos, D3DXVECTOR3 _Color) {
		D3DXCreateCylinder(g_pd3dDevice, 0.07f, 0.07f, 0.2f, 30, 30, &pMesh, NULL);
		Pos = _Pos;

		pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];

		// マテリアルなし
		d3dxMaterials.Diffuse.r = _Color.x;
		d3dxMaterials.Diffuse.g = _Color.y;
		d3dxMaterials.Diffuse.b = _Color.z;
		d3dxMaterials.Diffuse.a = 0.0f;
		d3dxMaterials.Ambient = d3dxMaterials.Diffuse;

		return TRUE;
	}
	BOOL CleanupMesh() {
		if (pMesh != NULL)
			pMesh->Release();

		return TRUE;
	}
	BOOL RenderCylinder() {
		D3DXMATRIXA16 matWorld, matPosition, matRotation;
		D3DXMatrixIdentity(&matWorld);

		// モデルの回転
		D3DXVECTOR3		Rot = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		D3DXMatrixRotationAxis(&matRotation, &Rot,90.0f / 180.0f * M_PI);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotation);

		// モデルの移動
		D3DXMatrixTranslation(&matPosition, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//myd3d->SetMaterial(Color.x, Color.y, Color.z);

		// テクスチャのブレンディング方法を定義する
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// 引数の成分を乗算する
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pd3dDevice->SetMaterial(&d3dxMaterials);
		pMesh->DrawSubset(0);
		return TRUE;
	}

}Goal[6];

struct BallData{
	MeshData		Ball;						// ボール構造体
	D3DXVECTOR3		Pos;						// ボール座標
	D3DXVECTOR3		Rot;						// ボール回転方向
	double			Ball_Weight;				// ボール重さ
	double			Ball_Radius;				// ボール半径
	D3DXVECTOR3		Speed;						// ボール速度
	double			Coefficient_Restitution;	// 反発係数
	double			Attenuation_Coefficient;	// 減衰係数

public:
	BallData() {			/* constructor	*/
		Pos.x = 0; Pos.y = 0; Pos.z = 0;
		Ball_Weight = 0.00170;		// 170g
		Ball_Radius = 0.02855;	// 直径57.1mm
		Rot.x = 0; Rot.y = 0; Rot.z = 0;
		Speed.x = 0; Speed.y = 0; Speed.z = 0;
		Coefficient_Restitution = 0.985;
		Attenuation_Coefficient = 0.985;
	}
	BOOL LoadData(char* file, D3DXVECTOR3 _Pos) {
		Ball.LoadMeshData(file);
		Pos = _Pos;
		return TRUE;
	}
	BOOL DrawingData() {
		if (Pos.x >= 1.65 || Pos.x <= -1.65)
			ReflectBallPosX();
		if (Pos.z >= 0.85 || Pos.z <= -0.85)
			ReflectBallPosZ();
		UpdateBallPos();

		// モデルの配置
		D3DXMATRIXA16 matWorld, matPosition, matRotation;
		D3DXMatrixIdentity(&matWorld);

		// モデルの回転
		float theta = fmod((timeGetTime()) / 30.0f, 360.0f) * M_PI / 180.0f;
		D3DXVECTOR3		Rot_t;
		D3DXVec3Normalize(&Rot_t, &Rot);
		D3DXMatrixRotationAxis(&matRotation, &Rot_t, Ball_Radius * theta);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotation);

		// モデルの移動
		D3DXMatrixTranslation(&matPosition, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

		// ワールドマトリックスをDirectXに設定
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		Ball.RenderMesh();
		return TRUE;
	}
	VOID UpdateBallPos() {
		//if (!(Speed.x == 0 && Speed.y == 0 && Speed.z == 0)) {
		Pos += Speed + Rot / 10.0f;
		Speed = Speed * Attenuation_Coefficient;
		Rot = Rot * Attenuation_Coefficient;
		//SetRotate();
		//}
	}
	VOID ReflectBallPosX() {
		Speed.x = -Speed.x * Coefficient_Restitution;
		Speed.z = Speed.z * Coefficient_Restitution;
		Rot.x = -Rot.x * 0.5f;
		Rot.z = Rot.z * Coefficient_Restitution;
		if (Pos.x >= 1.65) {
			Pos.x = 1.62;
		}
		else if (Pos.x <= -1.65) {
			Pos.x = -1.62;
		}
		//SetRotate();
	}
	VOID ReflectBallPosZ() {
		Speed.z = -Speed.z * Coefficient_Restitution;
		Speed.x = Speed.x * Coefficient_Restitution;
		Rot.z = -Rot.z * 0.5f;
		Rot.x = Rot.x * Coefficient_Restitution;
		if (Pos.z >= 0.85) {
			Pos.z = 0.82;
		}
		else if (Pos.z <= -0.85) {
			Pos.z = -0.82;
		}
		//SetRotate();
	}
	VOID SetRotate() {
		Rot.x = -Speed.z;
		Rot.y = Speed.y;
		Rot.z = Speed.x;
	}
	VOID Goal(float x,float y) {
		D3DXMATRIX TEMP;            // 板の回転行列

		D3DXVECTOR3 pout;
		//ZeroMemory(&pout, sizeof(pout));  //pout初期化
		ScreenViewChanger(x,y,&pout);
		Pos = pout;
		GetBillBoardRotation(&Pos, &hand.Pos, &TEMP);   // ターゲットの方を向く回転行列を決定します。

		// モデルの配置
		D3DXMATRIXA16 matWorld, matPosition;
		D3DXMatrixIdentity(&matWorld);

		// モデルの回転
		D3DXMatrixMultiply(&matWorld, &matWorld, &TEMP);

		// モデルの移動
		D3DXMatrixTranslation(&matPosition, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

		// ワールドマトリックスをDirectXに設定
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		Ball.RenderMesh();

		Speed.x = 0; Speed.y = 0; Speed.z = 0;
	}
}Ball[9], hand;



//-----------------------------------------------------------------
//    Main.
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevinst, LPSTR nCmdLine, int nCmdShow)
{

	ZeroMemory(&msg, sizeof(msg));  //msg初期化

	hWnd = InitApp(hInst, nCmdShow);
	if (!hWnd) return FALSE;

	if (!InitDirect3D(hWnd)) return FALSE;

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
	
	/*Mesh Load*/
	Table.LoadMeshData(".\\table.x");
	Shop.LoadMeshData(".\\Shop.x");
	Ball[0].LoadData(".\\1.x", D3DXVECTOR3(0.795f, 0.976f,    0.0f));
	Ball[1].LoadData(".\\2.x", D3DXVECTOR3(0.925f, 0.976f, -0.072f));
	Ball[2].LoadData(".\\3.x", D3DXVECTOR3(1.055f, 0.976f,    0.0f));
	Ball[3].LoadData(".\\4.x", D3DXVECTOR3(0.925f, 0.976f,  0.072f));
	Ball[4].LoadData(".\\5.x", D3DXVECTOR3( 0.86f, 0.976f, -0.036f));
	Ball[5].LoadData(".\\6.x", D3DXVECTOR3( 0.86f, 0.976f,  0.036f));
	Ball[6].LoadData(".\\7.x", D3DXVECTOR3( 0.99f, 0.976f, -0.036f));
	Ball[7].LoadData(".\\8.x", D3DXVECTOR3( 0.99f, 0.976f,  0.036f));
	Ball[8].LoadData(".\\9.x", D3DXVECTOR3(0.925f, 0.976f,    0.0f));
	hand.LoadData(".\\hand.x", D3DXVECTOR3( -1.0f, 0.976f,    0.0f));
	Goal[0].LoadData(D3DXVECTOR3(-1.65f, 0.9f, 0.85f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	Goal[1].LoadData(D3DXVECTOR3( 1.65f, 0.9f, 0.85f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	Goal[2].LoadData(D3DXVECTOR3(-1.65f, 0.9f,-0.85f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	Goal[3].LoadData(D3DXVECTOR3( 1.65f, 0.9f,-0.85f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	Goal[4].LoadData(D3DXVECTOR3( 0.0f, 0.9f,-0.9f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	Goal[5].LoadData(D3DXVECTOR3( 0.0f, 0.9f, 0.9f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));

	theta = M_PI;
	lookheight = 0.0f;

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
	D3DXVECTOR3 vEyePt, vLookatPt, vUpVec;

	// World Matrix.
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	D3DXMatrixRotationY(&matWorld, (float)(270*M_PI/180));
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

	// Projection Matrix.
	D3DXMatrixPerspectiveFovLH(&matProj, (float)(60 * M_PI / 180), SCREEN_WIDTH/ SCREEN_HEIGHT, 0.5f, 100.0f);
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

	if (MState.lButton) {
		//if (MState.x >= 812 && MState.x <= 869) {
		//	if (MState.y >= 450 && MState.y <= 500) {
				hand.Speed.x += cos(theta - M_PI) / 40.0f; hand.Speed.z += sin(theta - M_PI) / 40.0f;
				//hand.Rot.x += cos(theta - M_PI); hand.Rot.z += sin(theta - M_PI);
		//	}
		//}
	}

	// 方向キーによって視点移動
	if (g_diKeyState[DIK_LSHIFT] & 0x80) {
		// シフトキーを押しつつ方向キーによって倍視点移動
		if (g_diKeyState[DIK_LEFT] & 0x80) {
			theta += 0.8 * M_PI / 180.0f;
		}
		else if (g_diKeyState[DIK_RIGHT] & 0x80) {
			theta -= 0.8 * M_PI / 180.0f;
		}
		else if (g_diKeyState[DIK_UP] & 0x80) {
			lookheight -= 0.04f;
		}
		else if (g_diKeyState[DIK_DOWN] & 0x80) {
			lookheight += 0.04f;
		}
	}
	else {
		// 方向キーによって視点移動
		if (g_diKeyState[DIK_LEFT] & 0x80) {
			theta += 0.2 * M_PI / 180.0f;
		}
		else if (g_diKeyState[DIK_RIGHT] & 0x80) {
			theta -= 0.2 * M_PI / 180.0f;
		}
		else if (g_diKeyState[DIK_UP] & 0x80) {
			lookheight -= 0.01f;
		}
		else if (g_diKeyState[DIK_DOWN] & 0x80) {
			lookheight += 0.01f;
		}
	}	

	// モデルの配置
	D3DXMATRIXA16 matWorld, matPosition;
	D3DXMatrixIdentity(&matWorld);
	// モデルの移動
	D3DXMatrixTranslation(&matPosition, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
	// ワールドマトリックスをDirectXに設定
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	Shop.RenderMesh();
	Table.RenderMesh();



	// GoalとBall同士のあたり判定
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 6; j++) {
			if (D3DXVec3Length(&(Goal[j].Pos - Ball[i].Pos)) <= (0.02855f + 0.07f)) {
				isFALL[i] = TRUE;
			}
		}
	}
	// Goal済みボールの再配置
	for (int i = 0; i < 9; i++) {
		if (isFALL[i]) {
			Ball[i].Goal(60.0f * (i + 1), 50.0f);
		}
		else {
			Ball[i].DrawingData();
		}
	}

	hand.DrawingData();
	// handとBall同士のあたり判定
	for (int i = 0; i < 9; i++) {
		if (D3DXVec3Length(&(hand.Pos - Ball[i].Pos)) <= (0.02855 * 2) && !isFALL[i]) {
			CalcParticleColliAfterPos(&hand.Pos, &hand.Speed, &Ball[i].Pos, &Ball[i].Speed, hand.Ball_Weight, Ball[i].Ball_Weight, hand.Coefficient_Restitution, Ball[i].Coefficient_Restitution, 0.001f, &hand.Pos, &hand.Speed, &Ball[i].Pos, &Ball[i].Speed);
			Ball[i].SetRotate();
		}
	}
	// BallとBall同士のあたり判定
	for (int i = 0; i < 9; i++) {
		for (int j = i+1; j < 9; j++) {
			if (D3DXVec3Length(&(Ball[i].Pos - Ball[j].Pos)) <= (0.02855 * 2) && !isFALL[i] && !isFALL[j]) {
				CalcParticleColliAfterPos(&Ball[i].Pos, &Ball[i].Speed, &Ball[j].Pos, &Ball[j].Speed, Ball[i].Ball_Weight, Ball[j].Ball_Weight, Ball[i].Coefficient_Restitution, Ball[j].Coefficient_Restitution, 0.001f, &Ball[i].Pos, &Ball[i].Speed, &Ball[j].Pos, &Ball[j].Speed);
				Ball[j].SetRotate();
			}
		}
	}

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	return TRUE;
}


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

D3DXVECTOR3* ScreenViewChanger(float x, float y, D3DXVECTOR3* pout) {
	D3DXMATRIXA16 matView, matProj;
	D3DXVECTOR3 vEyePt, vLookatPt, vUpVec;
	D3DVIEWPORT9 viewData;

	// ビュー行列
	vEyePt.x = 1.2f*cos(theta) + hand.Pos.x;
	vEyePt.y = 1.4f + lookheight;
	vEyePt.z = 1.2f*sin(theta) + hand.Pos.z;
	vLookatPt = hand.Pos;
	vUpVec.x = 0.0f;
	vUpVec.y = 1.5f;
	vUpVec.z = 0.0f;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	//g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// 射影行列
	D3DXMatrixPerspectiveFovLH(&matProj, (float)(60 * M_PI / 180), SCREEN_WIDTH / SCREEN_HEIGHT, 0.5f, 100.0f);
	//g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	// ビューポート
	g_pd3dDevice->GetViewport(&viewData);

	CalcScreenToWorld(pout, x, y, 0.4f, viewData.Width, viewData.Height, &matView, &matProj);

	return pout;
}