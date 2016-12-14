#pragma once

#include "Header.h"

struct BallData {
	MeshData		Ball;						// �{�[���\����
	D3DXVECTOR3		Pos;						// �{�[�����W
	D3DXVECTOR3		Rot;						// �{�[����]����
	double			Ball_Weight;				// �{�[���d��
	double			Ball_Radius;				// �{�[�����a
	D3DXVECTOR3		Speed;						// �{�[�����x
	double			Coefficient_Restitution;	// �����W��
	double			Attenuation_Coefficient;	// �����W��
	bool			isMoveBall;					// �{�[�����ړ���������

public:
	BallData() {			/* constructor	*/
		Pos.x = 0; Pos.y = 0; Pos.z = 0;
		Rot.x = 0; Rot.y = 0; Rot.z = 0;
		Speed.x = 0; Speed.y = 0; Speed.z = 0;
		Ball_Weight = 0.00170;		// 170g
		Ball_Radius = 0.035;
		Coefficient_Restitution = 0.99;
		Attenuation_Coefficient = 0.975;
		isMoveBall = false;
	}
	BOOL LoadData(char* file, D3DXVECTOR3 _Pos) {
		Ball.LoadMeshData(file);
		Pos = _Pos;
		Rot.x = 0; Rot.y = 0; Rot.z = 0;
		Speed.x = 0; Speed.y = 0; Speed.z = 0;
		return TRUE;
	}
	BOOL DrawingData() {
		if (Pos.x >= 1.65 || Pos.x <= -1.65)
			ReflectBallPosX();
		if (Pos.z >= 0.85 || Pos.z <= -0.85)
			ReflectBallPosZ();
		UpdateBallPos();

		// ���f���̔z�u
		D3DXMATRIXA16 matWorld, matPosition, matRotation;
		D3DXMatrixIdentity(&matWorld);

		// ���f���̉�]
		float theta = fmod((timeGetTime()) / 30.0f, 360.0f) * M_PI / 180.0f;
		D3DXVECTOR3		Rot_t;
		D3DXVec3Normalize(&Rot_t, &Rot);
		D3DXMatrixRotationAxis(&matRotation, &Rot_t, Ball_Radius * theta);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matRotation);

		// ���f���̈ړ�
		D3DXMatrixTranslation(&matPosition, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

		// ���[���h�}�g���b�N�X��DirectX�ɐݒ�
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		Ball.RenderMesh();
		return TRUE;
	}

	BOOL ValidateSpeed() {
		if (D3DXVec3Length(&Speed) <= IKD_EPSIRON) { isMoveBall = false; }
		else { isMoveBall = true; }

		return isMoveBall;

	}
	VOID UpdateBallPos() {
		//if (!(Speed.x == 0 && Speed.y == 0 && Speed.z == 0)) {
		Pos += Speed + Rot / 10.0f;
		Speed = Speed * Attenuation_Coefficient;
		Rot = Rot * Attenuation_Coefficient;
		//SetRotate();
		//}
	}
	VOID ReflectBallPosX() {
		Speed.x = -Speed.x * Coefficient_Restitution;
		Speed.z = Speed.z * Coefficient_Restitution;
		Rot.x = -Rot.x * 0.5f;
		Rot.z = Rot.z * Coefficient_Restitution;
		if (Pos.x >= 1.65) {
			Pos.x = 1.65;
		}
		else if (Pos.x <= -1.65) {
			Pos.x = -1.65;
		}
		//SetRotate();
	}
	VOID ReflectBallPosZ() {
		Speed.z = -Speed.z * Coefficient_Restitution;
		Speed.x = Speed.x * Coefficient_Restitution;
		Rot.z = -Rot.z * 0.5f;
		Rot.x = Rot.x * Coefficient_Restitution;
		if (Pos.z >= 0.85) {
			Pos.z = 0.85;
		}
		else if (Pos.z <= -0.85) {
			Pos.z = -0.85;
		}
		//SetRotate();
	}
	VOID SetRotate() {
		Rot.x = -Speed.z;
		Rot.y = Speed.y;
		Rot.z = Speed.x;
	}
	VOID Goal(float x, float y) {
		D3DXMATRIX TEMP;            // �̉�]�s��

		D3DXVECTOR3 pout;
		//ZeroMemory(&pout, sizeof(pout));  //pout������
		ScreenViewChanger(x, y, &pout);
		Pos = pout;
		GetBillBoardRotation(&Pos, &hand.Pos, &TEMP);   // �^�[�Q�b�g�̕���������]�s������肵�܂��B

														// ���f���̔z�u
		D3DXMATRIXA16 matWorld, matPosition;
		D3DXMatrixIdentity(&matWorld);

		// ���f���̉�]
		D3DXMatrixMultiply(&matWorld, &matWorld, &TEMP);

		// ���f���̈ړ�
		D3DXMatrixTranslation(&matPosition, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

		// ���[���h�}�g���b�N�X��DirectX�ɐݒ�
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		Ball.RenderMesh();

		Speed.x = 0; Speed.y = 0; Speed.z = 0;
	}
}Ball[9], hand;