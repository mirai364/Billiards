#pragma once

#include "Header.h"
#include "ExistsMoveBall.h"

//-----------------------------------------------------------------
//    Game Scene.
//-----------------------------------------------------------------
void GameScene() {
	// �N���b�N�����ꍇ�̏���
	if (MState.lButton && !isClick && !ExistsMoveBall()) {
		isClick = true;
		ClickLength = 0.0f;
	}
	if (isClick && !ExistsMoveBall()) {
		ClickLength += 0.3f;
	}
	if (!MState.lButton && isClick && !ExistsMoveBall()) {
		hand.Speed.x += cos(theta - M_PI) / 100.0f * (ClickLength); hand.Speed.z += sin(theta - M_PI) / 100.0f * (ClickLength);
		isClick = false;
		ClickLength = 0.0f;
		isClickAfter = true;
	}

	// �����L�[�ɂ���Ď��_�ړ�
	if (g_diKeyState[DIK_LSHIFT] & 0x80) {
		// �V�t�g�L�[�����������L�[�ɂ���Ĕ{���_�ړ�
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

	// Goal��Ball���m�̂����蔻��
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 6; j++) {
			if (D3DXVec3Length(&(Goal[j].Pos - Ball[i].Pos)) <= (Ball[i].Ball_Radius + 0.08f)) {
				isFALL[i] = TRUE;
			}
		}
	}
	// Goal��hand���m�̂����蔻��(���������烊�U���g��)
	for (int j = 0; j < 6; j++) {
		if (D3DXVec3Length(&(Goal[j].Pos - hand.Pos)) <= (hand.Ball_Radius + 0.08f)) {
			GameToResult();
		}
	}
	// �{�[���̍Ĕz�u
	for (int i = 0; i < 9; i++) {
		if (isFALL[i]) {
			Ball[i].Goal(60.0f * (i + 1), 50.0f);
		}
		else {
			Ball[i].DrawingData();
		}
	}
	hand.DrawingData();

	// ���̃{�[����I��
	for (int i = 0; i < 9; i++) {
		if (!isFALL[i]) {
			NextBallNumber = i;
			break;
		}
		// ���ׂăS�[���ς݂Ń��U���g��
		if (isFALL[i] && i == 8) {
			GameToResult();
		}
	}


	// hand��Ball���m�̂����蔻��
	for (int i = 0; i < 9; i++) {
		if (D3DXVec3Length(&(hand.Pos - Ball[i].Pos)) <= (Ball[i].Ball_Radius + hand.Ball_Radius) && !isFALL[i]) {
			CalcParticleColliAfterPos(&hand.Pos, &hand.Speed, &Ball[i].Pos, &Ball[i].Speed, hand.Ball_Weight, Ball[i].Ball_Weight, hand.Coefficient_Restitution, Ball[i].Coefficient_Restitution, 0.001f, &hand.Pos, &hand.Speed, &Ball[i].Pos, &Ball[i].Speed);
			Ball[i].SetRotate();
			// �Ԉ�����ԍ��ɓ��������ꍇ���U���g��
			if (!(NextBallNumber == i) && !isFirstConflict) {
				GameToResult();
			}
			isFirstConflict = true;
		}
	}
	// Ball��Ball���m�̂����蔻��
	for (int i = 0; i < 9; i++) {
		for (int j = i + 1; j < 9; j++) {
			if (D3DXVec3Length(&(Ball[i].Pos - Ball[j].Pos)) <= (Ball[i].Ball_Radius + Ball[j].Ball_Radius) && !isFALL[i] && !isFALL[j]) {
				CalcParticleColliAfterPos(&Ball[i].Pos, &Ball[i].Speed, &Ball[j].Pos, &Ball[j].Speed, Ball[i].Ball_Weight, Ball[j].Ball_Weight, Ball[i].Coefficient_Restitution, Ball[j].Coefficient_Restitution, 0.001f, &Ball[i].Pos, &Ball[i].Speed, &Ball[j].Pos, &Ball[j].Speed);
				Ball[j].SetRotate();
			}
		}
	}
}