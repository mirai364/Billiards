#pragma once

#include "Header.h"

//-----------------------------------------------------------------
//    Title Scene.
//-----------------------------------------------------------------
void TitleScene() {
	RECT rect;

	if (MState.lButton) {
		TitleToGame();
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

	// ボールの配置
	for (int i = 0; i < 9; i++) {
		if (isFALL[i]) {
			Ball[i].Goal(60.0f * (i + 1), 50.0f);
		}
		else {
			Ball[i].DrawingData();
		}
	}
	hand.DrawingData();


	// 文字出力
	SetRect(&rect, 0.0f, SCREEN_HEIGHT / 4.0f * 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT / 4.0f);//描画位置
	g_pFont->DrawText(NULL, "Billiards", -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	// 文字出力
	SetRect(&rect, 0.0f, SCREEN_HEIGHT / 4.0f * 3.0f, SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f);//描画位置
	g_pFont->DrawText(NULL, "Please click to start", -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB((timeGetTime() / 3) % 255, 0, 0, 0));
}