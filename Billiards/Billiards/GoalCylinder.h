#pragma once

#include "Header.h"

struct GoalCylinder {
	LPD3DXMESH              pMesh;
	D3DXVECTOR3				Pos;						// 円柱座標
	D3DMATERIAL9			d3dxMaterials;
public:
	GoalCylinder() {			/* constructor	*/
		pMesh = NULL;
	}
	BOOL LoadData(D3DXVECTOR3 _Pos, D3DXVECTOR3 _Color, float x, float y, float h) {
		D3DXCreateCylinder(g_pd3dDevice, x, y, h, 30, 30, &pMesh, NULL);
		Pos = _Pos;

		pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];

		// マテリアルなし
		d3dxMaterials.Diffuse.r = _Color.x;
		d3dxMaterials.Diffuse.g = _Color.y;
		d3dxMaterials.Diffuse.b = _Color.z;
		d3dxMaterials.Diffuse.a = 128.0f;
		d3dxMaterials.Ambient = d3dxMaterials.Diffuse;

		return TRUE;
	}
	BOOL CleanupMesh() {
		if (pMesh != NULL)
			pMesh->Release();

		return TRUE;
	}
	BOOL RenderCylinder(float angle, D3DXVECTOR3 Rot) {
		D3DXMATRIXA16 matWorld, matPosition, matRotation;
		D3DXMatrixIdentity(&matWorld);

		// モデルの回転
		//D3DXVECTOR3		Rot = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		D3DXMatrixRotationAxis(&matRotation, &Rot, angle / 180.0f * M_PI);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotation);

		// モデルの移動
		D3DXMatrixTranslation(&matPosition, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//myd3d->SetMaterial(Color.x, Color.y, Color.z);

		// テクスチャのブレンディング方法を定義する
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// 引数の成分を乗算する
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pd3dDevice->SetMaterial(&d3dxMaterials);
		pMesh->DrawSubset(0);
		return TRUE;
	}

}Goal[6], Ray[2];