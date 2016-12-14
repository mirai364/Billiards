#pragma once

#include "Header.h"
#include "ExistsMoveBall.h"

//-----------------------------------------------------------------
//    Game Scene.
//-----------------------------------------------------------------
void GameScene() {
	// クリックした場合の処理
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
	if (!ExistsMoveBall()) {

		// 白玉から壁までのレイの表示
		BOOL pHit;
		DWORD pFaceIndex, pCountOfHits;
		float pU, pV, pDist, pDist2;
		LPD3DXBUFFER ppALLHits;

		D3DXVECTOR3 pRayDir = D3DXVECTOR3(1.0f*cos(theta - M_PI), 0.0f, 1.0f*sin(theta - M_PI));

		D3DXIntersect(Table.pMesh, &hand.Pos, &pRayDir,&pHit,&pFaceIndex,&pU,&pV,&pDist,&ppALLHits,&pCountOfHits);
		Ray[0].LoadData(hand.Pos + (pDist * pRayDir) / 2, D3DXVECTOR3(255, 255, 0), 0.005f, 0.005f, pDist);
		Ray[0].RenderCylinder((theta * 180 / M_PI) - 90.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f));

		/*
		// 壁から壁までのレイの表示
		D3DXVECTOR3 pRayDir2 = D3DXVECTOR3(1.0f*cos(theta / 2.0f), 0.0f, 1.0f*sin(theta/2.0f));

		D3DXIntersect(Table.pMesh, &(hand.Pos + (pDist * pRayDir)), &pRayDir2, &pHit, &pFaceIndex, &pU, &pV, &pDist2, &ppALLHits, &pCountOfHits);

		Ray[1].LoadData((hand.Pos + (pDist * pRayDir)) + (pDist2 * pRayDir2) / 2, D3DXVECTOR3(1.0f, 0.5f, 0.5f), 0.005f, 0.005f, pDist2);
		Ray[1].RenderCylinder(((theta) / 2.0f * 180 / M_PI) - 90.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
		*/
		
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
			if (D3DXVec3Length(&(Goal[j].Pos - Ball[i].Pos)) <= (Ball[i].Ball_Radius + 0.08f)) {
				isFALL[i] = TRUE;
			}
		}
	}
	// Goalとhand同士のあたり判定(当たったらリザルトへ)
	for (int j = 0; j < 6; j++) {
		if (D3DXVec3Length(&(Goal[j].Pos - hand.Pos)) <= (hand.Ball_Radius + 0.08f)) {
			GameToResult();
		}
	}
	// ボールの再配置
	for (int i = 0; i < 9; i++) {
		if (isFALL[i]) {
			Ball[i].Goal(60.0f * (i + 1), 50.0f);
		}
		else {
			Ball[i].DrawingData();
		}
	}
	hand.DrawingData();

	// 次のボールを選択
	for (int i = 0; i < 9; i++) {
		if (!isFALL[i]) {
			NextBallNumber = i;
			break;
		}
		// すべてゴール済みでリザルトへ
		if (isFALL[i] && i == 8) {
			GameToResult();
		}
	}


	// handとBall同士のあたり判定
	for (int i = 0; i < 9; i++) {
		if (D3DXVec3Length(&(hand.Pos - Ball[i].Pos)) <= (Ball[i].Ball_Radius + hand.Ball_Radius) && !isFALL[i]) {
			CalcParticleColliAfterPos(&hand.Pos, &hand.Speed, &Ball[i].Pos, &Ball[i].Speed, hand.Ball_Weight, Ball[i].Ball_Weight, hand.Coefficient_Restitution, Ball[i].Coefficient_Restitution, 0.001f, &hand.Pos, &hand.Speed, &Ball[i].Pos, &Ball[i].Speed);
			Ball[i].SetRotate();
			// 間違った番号に当たった場合リザルトへ
			if (!(NextBallNumber == i) && !isFirstConflict) {
				GameToResult();
			}
			isFirstConflict = true;
		}
	}
	// BallとBall同士のあたり判定
	for (int i = 0; i < 9; i++) {
		for (int j = i + 1; j < 9; j++) {
			if (D3DXVec3Length(&(Ball[i].Pos - Ball[j].Pos)) <= (Ball[i].Ball_Radius + Ball[j].Ball_Radius) && !isFALL[i] && !isFALL[j]) {
				CalcParticleColliAfterPos(&Ball[i].Pos, &Ball[i].Speed, &Ball[j].Pos, &Ball[j].Speed, Ball[i].Ball_Weight, Ball[j].Ball_Weight, Ball[i].Coefficient_Restitution, Ball[j].Coefficient_Restitution, 0.001f, &Ball[i].Pos, &Ball[i].Speed, &Ball[j].Pos, &Ball[j].Speed);
				Ball[j].SetRotate();
			}
		}
	}
}