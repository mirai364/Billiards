#include "Header.h"

// ビルボード回転行列取得関数
//   BillPos    : ビルボードの制御点の位置
//   TargetPos  : ビルボードが向く注視点
//   Rpt        : 回転行列
void GetBillBoardRotation(D3DXVECTOR3 *BillPos, D3DXVECTOR3 *TargetPos, D3DXMATRIX *Rot)
{
	D3DXMatrixIdentity(Rot);
	D3DXMatrixLookAtLH(Rot, TargetPos, BillPos, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(Rot, NULL, Rot);
	Rot->_41 = 0.0f;   // オフセットを切る（回転行列だけにしてしまう）
	Rot->_42 = 0.0f;
	Rot->_43 = 0.0f;
}