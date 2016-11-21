#define _USE_MATH_DEFINES
#define IKD_EPSIRON 0.00001f	// 誤差

#include <stdio.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include <iostream>

#pragma once
#pragma comment(lib,"winmm.lib")

const double Gravity = 9.8;
const int SCREEN_WIDTH = 1680;	// ウィンドウの幅
const int SCREEN_HEIGHT = 768;	// ウィンドウの高さ


///////////////////////////////////////////////////
// パーティクル衝突後速度位置算出関数
//   pColliPos_A : 衝突中のパーティクルAの中心位置
//   pVelo_A     : 衝突の瞬間のパーティクルAの速度
//   pColliPos_B : 衝突中のパーティクルBの中心位置
//   pVelo_B     : 衝突の瞬間のパーティクルBの速度
//   weight_A    : パーティクルAの質量
//   weight_B    : パーティクルBの質量
//   res_A       : パーティクルAの反発率
//   res_B       : パーティクルBの反発率
//   time        : 反射後の移動時間
//   pOut_pos_A  : パーティクルAの反射後位置
//   pOut_velo_A : パーティクルAの反射後速度ベクトル
//   pOut_pos_B  : パーティクルBの反射後位置
//   pOut_velo_B : パーティクルBの反射後速度ベクトル

bool CalcParticleColliAfterPos(
	D3DXVECTOR3 *pColliPos_A, D3DXVECTOR3 *pVelo_A,
	D3DXVECTOR3 *pColliPos_B, D3DXVECTOR3 *pVelo_B,
	FLOAT weight_A, FLOAT weight_B,
	FLOAT res_A, FLOAT res_B,
	FLOAT time,
	D3DXVECTOR3 *pOut_pos_A, D3DXVECTOR3 *pOut_velo_A,
	D3DXVECTOR3 *pOut_pos_B, D3DXVECTOR3 *pOut_velo_B
	)
{
	FLOAT TotalWeight = weight_A + weight_B; // 質量の合計
	FLOAT RefRate = (1 + res_A*res_B); // 反発率
	D3DXVECTOR3 C = *pColliPos_B - *pColliPos_A; // 衝突軸ベクトル
	D3DXVec3Normalize(&C, &C);
	FLOAT Dot = D3DXVec3Dot(&(*pVelo_A - *pVelo_B), &C); // 内積算出
	D3DXVECTOR3 ConstVec = RefRate*Dot / TotalWeight * C; // 定数ベクトル

	// 衝突後速度ベクトルの算出
	*pOut_velo_A = -weight_B * ConstVec + *pVelo_A;
	*pOut_velo_B = weight_A * ConstVec + *pVelo_B;

	// 衝突後位置の算出
	*pOut_pos_A = *pColliPos_A + time * (*pOut_velo_A);
	*pOut_pos_B = *pColliPos_B + time * (*pOut_velo_B);

	if (D3DXVec3Length(&(*pOut_pos_A - *pOut_pos_B)) < (0.02855 * 2 - IKD_EPSIRON)) {
		D3DXVECTOR3 A = *pOut_velo_A;		D3DXVec3Normalize(&A, &A); //　単位速度ベクトル
		D3DXVECTOR3 B = *pOut_velo_B;		D3DXVec3Normalize(&B, &B); //　単位速度ベクトル
		*pOut_pos_A = *pColliPos_A + 0.02855 * A;
		*pOut_pos_B = *pColliPos_B + 0.02855 * B;
	}

	return true;
}

//-----------------------------------------------------------------
//    Grobal Variables.
//-----------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;


//-----------------------------------------------------------------
//    Struct
//-----------------------------------------------------------------
struct MeshData {
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

struct BallData {
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
		Coefficient_Restitution = 0.99;
		Attenuation_Coefficient = 0.99;
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
			Pos += Speed + Rot/10.0f;
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

}Ball[9],hand;




//-----------------------------------------------------------------
//    Prototypes.
//-----------------------------------------------------------------
HWND    InitApp(HINSTANCE, int);
BOOL    InitDirect3D(HWND);
BOOL    CleanupDirect3D();
BOOL    SetupMatrices();
BOOL    RenderDirect3D();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



//-----------------------------------------------------------------
//    Main.
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevinst, LPSTR nCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;

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
		CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInst, NULL);
	if (!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
	hand.Speed.x += 0.1f; hand.Rot.x += 0.02f; hand.Rot.z += 0.03f;

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
	//D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	//D3DXMatrixRotationY(&matWorld, (float)(270*M_PI/180));
	//g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Camera.
	//vEyePt.x = 0.0f - 1.9f;
	//vEyePt.y = 1.2f;
	//vEyePt.z = 0.0f;

	float theta = fmod((timeGetTime())/30.0f, 360.0f) * M_PI / 180.0f;

	vEyePt.x = 1.9f*cos(theta);
	vEyePt.y = 1.4f;
	vEyePt.z = 1.9f*sin(theta);

	vLookatPt.x = 0.0f;
	vLookatPt.y = 0.976f;
	vLookatPt.z = 0.0f;
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
	RECT rc;
	D3DXVECTOR3 center, position;
	

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0x00000000, 1.0f, 0);

	g_pd3dDevice->BeginScene();

	SetupMatrices();

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


	for (int i = 0; i < 9; i++) {
		Ball[i].DrawingData();
	}
	hand.DrawingData();
	
	for (int i = 0; i < 9; i++) {
		if (D3DXVec3Length(&(hand.Pos - Ball[i].Pos)) <= (0.02855 * 2)) {
			CalcParticleColliAfterPos(&hand.Pos, &hand.Speed, &Ball[i].Pos, &Ball[i].Speed, hand.Ball_Weight, Ball[i].Ball_Weight, hand.Coefficient_Restitution, Ball[i].Coefficient_Restitution, 0.001f, &hand.Pos, &hand.Speed, &Ball[i].Pos, &Ball[i].Speed);
			Ball[i].SetRotate();
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = i+1; j < 9; j++) {
			if (D3DXVec3Length(&(Ball[i].Pos - Ball[j].Pos)) <= (0.02855 * 2)) {
				CalcParticleColliAfterPos(&Ball[i].Pos, &Ball[i].Speed, &Ball[j].Pos, &Ball[j].Speed, Ball[i].Ball_Weight, Ball[j].Ball_Weight, Ball[i].Coefficient_Restitution, Ball[j].Coefficient_Restitution, 0.001f, &Ball[i].Pos, &Ball[i].Speed, &Ball[j].Pos, &Ball[j].Speed);
				Ball[j].SetRotate();
			}
		}
	}
	
	

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	return TRUE;
}