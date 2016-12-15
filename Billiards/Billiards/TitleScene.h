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

	// ���f���̔z�u
	D3DXMATRIXA16 matWorld, matPosition;
	D3DXMatrixIdentity(&matWorld);
	// ���f���̈ړ�
	D3DXMatrixTranslation(&matPosition, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
	// ���[���h�}�g���b�N�X��DirectX�ɐݒ�
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	Shop.RenderMesh();
	Table.RenderMesh();

	// �{�[���̔z�u
	for (int i = 0; i < 9; i++) {
		if (isFALL[i]) {
			Ball[i].Goal(60.0f * (i + 1), 50.0f);
		}
		else {
			Ball[i].DrawingData();
		}
	}
	hand.DrawingData();


	// �����o��
	SetRect(&rect, 0.0f, SCREEN_HEIGHT / 4.0f * 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT / 4.0f);//�`��ʒu
	g_pFont->DrawText(NULL, "Billiards", -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	// �����o��
	SetRect(&rect, 0.0f, SCREEN_HEIGHT / 4.0f * 3.0f, SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f);//�`��ʒu
	g_pFont->DrawText(NULL, "Please click to start", -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB((timeGetTime() / 3) % 255, 0, 0, 0));
}