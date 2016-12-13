#include "Header.h"

// �r���{�[�h��]�s��擾�֐�
//   BillPos    : �r���{�[�h�̐���_�̈ʒu
//   TargetPos  : �r���{�[�h�����������_
//   Rpt        : ��]�s��
void GetBillBoardRotation(D3DXVECTOR3 *BillPos, D3DXVECTOR3 *TargetPos, D3DXMATRIX *Rot)
{
	D3DXMatrixIdentity(Rot);
	D3DXMatrixLookAtLH(Rot, TargetPos, BillPos, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(Rot, NULL, Rot);
	Rot->_41 = 0.0f;   // �I�t�Z�b�g��؂�i��]�s�񂾂��ɂ��Ă��܂��j
	Rot->_42 = 0.0f;
	Rot->_43 = 0.0f;
}