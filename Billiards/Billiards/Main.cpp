#include <stdio.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma once
#pragma comment(lib,"winmm.lib")

//-----------------------------------------------------------------
//    Grobal Variables.
//-----------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
LPD3DXMESH              g_pMesh = NULL;
LPD3DXBUFFER			g_pMaterial;
DWORD					dwNumMaterials = NULL;
D3DMATERIAL9*			g_pMeshMaterials = NULL;
LPDIRECT3DTEXTURE9*		g_pMeshTextures = NULL;
D3DXMATERIAL*			d3dxMaterials;



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

	ZeroMemory(&msg, sizeof(msg));  //msg‰Šú‰»
	ZeroMemory(&g_pMaterial, sizeof(g_pMaterial));  //material‰Šú‰»

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

	hWnd = CreateWindow(szClassName, "Direct3D Test", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
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
	
	// Load model data
	D3DXLoadMeshFromX(".\\table_9.x", D3DXMESH_SYSTEMMEM, g_pd3dDevice,
		NULL, &g_pMaterial, NULL, &dwNumMaterials, &g_pMesh);

	// Material load
	d3dxMaterials = (D3DXMATERIAL*)g_pMaterial -> GetBufferPointer();
	g_pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
	g_pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i<dwNumMaterials; i++)
	{
			g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
			g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;
			g_pMeshTextures[i] = NULL;

			if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0) {
				D3DXCreateTextureFromFile(g_pd3dDevice, d3dxMaterials[i].pTextureFilename, &g_pMeshTextures[i]);
			}
	}


	return TRUE;
}



//-----------------------------------------------------------------
//    Cleanup Direct3D.
//-----------------------------------------------------------------
BOOL CleanupDirect3D()
{
	if (g_pMesh != NULL)
		g_pMesh->Release();

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
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Camera.
	vEyePt.x = 0.0f;
	vEyePt.y = 3.0f;
	vEyePt.z = 0.0f - 5.0f;
	vLookatPt.x = 0.0f;
	vLookatPt.y = 0.0f;
	vLookatPt.z = 0.0f;
	vUpVec.x = 0.0f;
	vUpVec.y = 1.0f;
	vUpVec.z = 0.0f;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// Projection Matrix.
	D3DXMatrixPerspectiveFovLH(&matProj, 3.0f / 4.0f, 1.0f, 1.0f, 100.0f);
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

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		g_pd3dDevice->SetMaterial(&g_pMeshMaterials[i]);
		g_pd3dDevice->SetTexture(0, g_pMeshTextures[i]);
		g_pMesh->DrawSubset(i);
	}

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	return TRUE;
}
