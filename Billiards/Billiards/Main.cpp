#define _USE_MATH_DEFINES

#include <stdio.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>

#pragma once
#pragma comment(lib,"winmm.lib")

const double Gravity = 9.8;
const int SCREEN_WIDTH = 1680;	// ウィンドウの幅
const int SCREEN_HEIGHT = 768;	// ウィンドウの高さ

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
	double			X, Y, Z;					// ボール座標
	double			rX, rY, rZ;					// ボール回転方向
	double			Ball_Weight;				// ボール重さ
	double			Ball_Radius;				// ボール半径
	double			sX, sY, sZ;					// ボール速度
	double			Coefficient_Restitution;	// 反射係数
	double			Attenuation_Coefficient;	// 減衰係数
	

public:
	BallData() {			/* constructor	*/
		X = 0; Y = 0; Z = 0;
		Ball_Weight = 170;		// 170g
		Ball_Radius = 28.55;	// 直径57.1mm
		sX = 0; sY = 0; sZ = 0;
		rX = 0; rY = 0; rZ = 0;
		Coefficient_Restitution = 0.99;
		Attenuation_Coefficient = 1;
	}
	BOOL LoadData(char* file) {
		Ball.LoadMeshData(file);
		return TRUE;
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
	Ball[0].LoadData(".\\1.x");
	Ball[1].LoadData(".\\2.x");
	Ball[2].LoadData(".\\3.x");
	Ball[3].LoadData(".\\4.x");
	Ball[4].LoadData(".\\5.x");
	Ball[5].LoadData(".\\6.x");
	Ball[6].LoadData(".\\7.x");
	Ball[7].LoadData(".\\8.x");
	Ball[8].LoadData(".\\9.x");
	hand.LoadData(".\\hand.x");

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
	D3DXMatrixRotationY(&matWorld, 270*M_PI/180);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Camera.
	vEyePt.x = 0.0f;
	vEyePt.y = 1.2f;
	vEyePt.z = 0.0f - 1.9f;
	vLookatPt.x = 0.0f;
	vLookatPt.y = 0.976f;
	vLookatPt.z = -1.0f;
	vUpVec.x = 0.0f;
	vUpVec.y = 1.0f;
	vUpVec.z = 0.0f;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// Projection Matrix.
	D3DXMatrixPerspectiveFovLH(&matProj, 60 * M_PI / 180, (float)SCREEN_WIDTH/ SCREEN_HEIGHT, 0.5f, 100.0f);
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

	Shop.RenderMesh();
	Table.RenderMesh();

	for (int i = 0; i < 9; i++) {
		Ball[i].Ball.RenderMesh();
	}
	hand.Ball.RenderMesh();

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	return TRUE;
}
