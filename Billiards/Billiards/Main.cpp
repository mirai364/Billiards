#define     szClassName TEXT("3D Box Rotate")

#include <windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <tchar.h>

#pragma once
#pragma comment(lib,"winmm.lib")

//頂点フォーマット

#define D3DFVF_VERTEX  (D3DFVF_XYZ | D3DFVF_DIFFUSE)	//トランスフォームされていない頂点の位置座標+ディフューズ カラー成分

typedef struct _D3DVERTEXCOR_ {
	float   x, y, z;	// トランスフォームされていない頂点の位置座標
	DWORD   color;	// ディフューズ カラー成分
}   D3DVERTEXCOR;


#define TIME_SET 500.0f	//	立方体を回転させるスピード 1/1000秒単位

HWND                    hWnd;
LPDIRECT3DDEVICE9       g_pDEV = 0;
LPDIRECT3D9             pD3D;
D3DPRESENT_PARAMETERS   D3DPPWin;


//	立方体の頂点座標を定義（座標は立方体の中心を原点とする)


D3DVERTEXCOR v1[] =     //前面赤の矩形 
{ { -50, -50,  50, 0xffff0000 },
{ 50, -50,  50, 0xffff0000 },
{ -50,  50,  50, 0xffff0000 },
{ 50,  50,  50, 0xffff0000 } };
D3DVERTEXCOR v2[] =     //右側面緑の矩形
{ { 50, -50,  50, 0xff00ff00 },
{ 50, -50, -50, 0xff00ff00 },
{ 50,  50,  50, 0xff00ff00 },
{ 50,  50, -50, 0xff00ff00 } };
D3DVERTEXCOR v3[] =     //上面青の矩形
{ { -50, -50, -50, 0xff0000ff },
{ 50, -50, -50, 0xff0000ff },
{ -50, -50,  50, 0xff0000ff },
{ 50, -50,  50, 0xff0000ff } };
D3DVERTEXCOR v4[] =     //左側面黄の矩形
{ { -50,  50,  50, 0xffffff00 },
{ -50,  50, -50, 0xffffff00 },
{ -50, -50,  50, 0xffffff00 },
{ -50, -50, -50, 0xffffff00 } };
D3DVERTEXCOR v5[] =     //裏面紫の矩形 
{ { -50,  50, -50, 0xffff00ff },
{ 50,  50, -50, 0xffff00ff },
{ -50, -50, -50, 0xffff00ff },
{ 50, -50, -50, 0xffff00ff } };
D3DVERTEXCOR v6[] =     //下面白の矩形
{ { 50,  50, -50, 0xffffffff },
{ -50,  50, -50, 0xffffffff },
{ 50,  50,  50, 0xffffffff },
{ -50,  50,  50, 0xffffffff } };

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT Init3DDev(HWND hwnd);
void  SetupMatrices(float x, float y, float z = 0);
void Draw(void);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
	MSG     msg;

	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,WndProc,0L,0L,hInst,
		NULL,NULL,NULL,NULL,szClassName,NULL };
	if (RegisterClassEx(&wc) == 0)
		return FALSE;
	HWND  hWnd = CreateWindowEx(0, szClassName, szClassName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
		NULL, NULL, hInst, NULL);
	if (FAILED(Init3DDev(hWnd)))	return FALSE;

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			Draw();
	}
	g_pDEV->Release();
	pD3D->Release();
	UnregisterClass(szClassName, wc.hInstance);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0L;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//デバイス/モード等の初期化
HRESULT Init3DDev(HWND hwnd) {

	hWnd = hwnd;
	pD3D = NULL;
	HRESULT     hr;
	D3DDISPLAYMODE          dmode;

	// デバイス/モード等の初期化
	if (pD3D == NULL)
		pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL)
		return E_FAIL;
	//現在のディスプレイモードを得る
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dmode)))
		return E_FAIL;

	// D3DDeviceオブジェクトの作成
	ZeroMemory(&D3DPPWin, sizeof(D3DPPWin));
	D3DPPWin.BackBufferFormat = dmode.Format;
	D3DPPWin.BackBufferCount = 1;
	D3DPPWin.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPPWin.Windowed = TRUE;
	D3DPPWin.EnableAutoDepthStencil = TRUE;
	D3DPPWin.AutoDepthStencilFormat = D3DFMT_D16;
	D3DPPWin.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DPPWin, &g_pDEV);
	if (FAILED(hr)) {
		hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPPWin, &g_pDEV);
		if (FAILED(hr)) {
			hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPPWin, &g_pDEV);
			if (FAILED(hr)) {
				MessageBox(NULL, TEXT("Create Device Error"), TEXT("Surface Error"), MB_OK);
				return E_FAIL;
			}
		}
	}
	return S_OK;
}



//描画環境の設定
void  SetupMatrices(float x, float y, float z) {
	D3DXMATRIX      matView;
	D3DXMATRIX      matProj;
	D3DXMATRIX      w;

	D3DXVECTOR3 v;
	v.x = -x;
	v.y = -z;
	v.z = -y;

	D3DXMatrixRotationAxis(&w, &v, timeGetTime() / TIME_SET); // 任意の軸を回転軸にして回転する行列を作成する。 システム時刻をミリ秒単位で取得


	D3DXMatrixTranslation(&matView, x, y, z); // オフセットを指定して行列を作成する。

	matView = w*matView;
	g_pDEV->SetTransform(D3DTS_VIEW, &matView);

	//透視変換の設定
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.0f), 1.0f, -300, 300); // 視野に基づいて、左手座標系パースペクティブ射影行列を作成する
	g_pDEV->SetTransform(D3DTS_PROJECTION, &matProj);
	//描画設定
	g_pDEV->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		//反時計回りの面を消去
	g_pDEV->SetRenderState(D3DRS_LIGHTING, FALSE);         	//ライティングしない
	g_pDEV->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);    	//Zバッファ使わない
}

//描画処理
void Draw(void) {
	if (!g_pDEV)
		return;
	//黒で塗りつぶして消去
	g_pDEV->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0, 0);
	//描画の開始
	g_pDEV->BeginScene();

	//描画環境の設定

	SetupMatrices(80, 80, 400);
	//デバイスに使用する頂点フォーマットをセットする
	g_pDEV->SetFVF(D3DFVF_VERTEX);
	//ストリームを使わないで直接データを渡して描画する
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v1, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v2, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v3, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v4, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v5, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v6, sizeof(D3DVERTEXCOR));

	SetupMatrices(80, -80, 400);
	//ストリームを使わないで直接データを渡して描画する
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v1, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v2, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v3, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v4, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v5, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v6, sizeof(D3DVERTEXCOR));

	SetupMatrices(-80, 80, 400);
	//ストリームを使わないで直接データを渡して描画する
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v1, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v2, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v3, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v4, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v5, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v6, sizeof(D3DVERTEXCOR));

	SetupMatrices(-80, -80, 400);
	//ストリームを使わないで直接データを渡して描画する
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v1, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v2, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v3, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v4, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v5, sizeof(D3DVERTEXCOR));
	g_pDEV->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v6, sizeof(D3DVERTEXCOR));

	g_pDEV->EndScene();
	g_pDEV->Present(NULL, NULL, NULL, NULL);
}