#pragma once

#include "Header.h"
#include "Level.h"
#include "Input.h"
#include "MSTATE.h"
#include "MeshData.h"
#include "GoalCylinder.h"
#include "BallData.h"

BOOL isTutorialClick = false;

//-----------------------------------------------------------------
//    Tutorial Scene.
//-----------------------------------------------------------------
void TutorialScene() {
	RECT rect = { 0, 0, 1680, 768 };
	D3DXVECTOR3 vec3Center(0, 0, 0);
	D3DXVECTOR3 vec3Position(0, 0, 0);


	g_pSprite->Begin(NULL);

	if (!isTutorialClick){
		g_pSprite->Draw(g_pTexture, &rect, &vec3Center, &vec3Position, 0xFFFFFFFF);
	}else{
		g_pSprite->Draw(g_pTexture2, &rect, &vec3Center, &vec3Position, 0xFFFFFFFF);
	}


	if (MState.lButton && !isTutorialClick) {
		isTutorialClick = true;
		Sleep(1000);
	}else if (MState.lButton && isTutorialClick) {
		TutorialToGame();
		Sleep(1000);
	}


	g_pSprite->End();
}