///////////////////////////////////////////////////
// �p�[�e�B�N���Փˌ㑬�x�ʒu�Z�o�֐�
//   pColliPos_A : �Փ˒��̃p�[�e�B�N��A�̒��S�ʒu
//   pVelo_A     : �Փ˂̏u�Ԃ̃p�[�e�B�N��A�̑��x
//   pColliPos_B : �Փ˒��̃p�[�e�B�N��B�̒��S�ʒu
//   pVelo_B     : �Փ˂̏u�Ԃ̃p�[�e�B�N��B�̑��x
//   weight_A    : �p�[�e�B�N��A�̎���
//   weight_B    : �p�[�e�B�N��B�̎���
//   res_A       : �p�[�e�B�N��A�̔�����
//   res_B       : �p�[�e�B�N��B�̔�����
//   time        : ���ˌ�̈ړ�����
//   pOut_pos_A  : �p�[�e�B�N��A�̔��ˌ�ʒu
//   pOut_velo_A : �p�[�e�B�N��A�̔��ˌ㑬�x�x�N�g��
//   pOut_pos_B  : �p�[�e�B�N��B�̔��ˌ�ʒu
//   pOut_velo_B : �p�[�e�B�N��B�̔��ˌ㑬�x�x�N�g��

bool CalcParticleColliAfterPos(
	D3DXVECTOR3 *pColliPos_A, D3DXVECTOR3 *pVelo_A,
	D3DXVECTOR3 *pColliPos_B, D3DXVECTOR3 *pVelo_B,
	FLOAT weight_A, FLOAT weight_B,
	FLOAT res_A, FLOAT res_B,
	FLOAT time,
	D3DXVECTOR3 *pOut_pos_A, D3DXVECTOR3 *pOut_velo_A,
	D3DXVECTOR3 *pOut_pos_B, D3DXVECTOR3 *pOut_velo_B
	)
{
	FLOAT TotalWeight = weight_A + weight_B; // ���ʂ̍��v
	FLOAT RefRate = (1 + res_A*res_B); // ������
	D3DXVECTOR3 C = *pColliPos_B - *pColliPos_A; // �Փˎ��x�N�g��
	D3DXVec3Normalize(&C, &C);
	FLOAT Dot = D3DXVec3Dot(&(*pVelo_A - *pVelo_B), &C); // ���ώZ�o
	D3DXVECTOR3 ConstVec = RefRate*Dot / TotalWeight * C; // �萔�x�N�g��

														  // �Փˌ㑬�x�x�N�g���̎Z�o
	*pOut_velo_A = -weight_B * ConstVec + *pVelo_A;
	*pOut_velo_B = weight_A * ConstVec + *pVelo_B;

	// �Փˌ�ʒu�̎Z�o
	*pOut_pos_A = *pColliPos_A + time * (*pOut_velo_A);
	*pOut_pos_B = *pColliPos_B + time * (*pOut_velo_B);

	if (D3DXVec3Length(&(*pOut_pos_A - *pOut_pos_B)) < (0.02855 * 2 - IKD_EPSIRON)) {
		D3DXVECTOR3 A = *pOut_velo_A;		D3DXVec3Normalize(&A, &A); //�@�P�ʑ��x�x�N�g��
		D3DXVECTOR3 B = *pOut_velo_B;		D3DXVec3Normalize(&B, &B); //�@�P�ʑ��x�x�N�g��
		*pOut_pos_A = *pColliPos_A + 0.02855 * A;
		*pOut_pos_B = *pColliPos_B + 0.02855 * B;
	}

	return true;
}