#pragma once

#include "Header.h"

//-----------------------------------------------------------------
//    Result Scene.
//-----------------------------------------------------------------
void ResultScene() {
	RECT rect;
	if (MState.lButton) {
		ResultToTitle();
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

	// �{�[���̍Ĕz�u
	for (int i = 0; i < 9; i++) {
		if (isFALL[i]) {
			Ball[i].Goal(150.0f * (i + 1) + (SCREEN_WIDTH - 150.0f * 9) / 3.0f, SCREEN_HEIGHT / 2.0f);
		}
		else {
			Ball[i].DrawingData();
		}
	}
	//hand.DrawingData();

	// �����o��
	SetRect(&rect, 0.0f, SCREEN_HEIGHT / 4.0f * 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT / 4.0f);//�`��ʒu
	g_pFont->DrawText(NULL, "RESULT", -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	// �����o��
	SetRect(&rect, 0.0f, SCREEN_HEIGHT / 4.0f * 3.0f, SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f);//�`��ʒu
	g_pFont->DrawText(NULL, "Return title", -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

}