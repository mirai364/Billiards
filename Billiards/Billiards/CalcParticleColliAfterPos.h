///////////////////////////////////////////////////
// パーティクル衝突後速度位置算出関数
//   pColliPos_A : 衝突中のパーティクルAの中心位置
//   pVelo_A     : 衝突の瞬間のパーティクルAの速度
//   pColliPos_B : 衝突中のパーティクルBの中心位置
//   pVelo_B     : 衝突の瞬間のパーティクルBの速度
//   weight_A    : パーティクルAの質量
//   weight_B    : パーティクルBの質量
//   res_A       : パーティクルAの反発率
//   res_B       : パーティクルBの反発率
//   time        : 反射後の移動時間
//   pOut_pos_A  : パーティクルAの反射後位置
//   pOut_velo_A : パーティクルAの反射後速度ベクトル
//   pOut_pos_B  : パーティクルBの反射後位置
//   pOut_velo_B : パーティクルBの反射後速度ベクトル

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
	FLOAT TotalWeight = weight_A + weight_B; // 質量の合計
	FLOAT RefRate = (1 + res_A*res_B); // 反発率
	D3DXVECTOR3 C = *pColliPos_B - *pColliPos_A; // 衝突軸ベクトル
	D3DXVec3Normalize(&C, &C);
	FLOAT Dot = D3DXVec3Dot(&(*pVelo_A - *pVelo_B), &C); // 内積算出
	D3DXVECTOR3 ConstVec = RefRate*Dot / TotalWeight * C; // 定数ベクトル

														  // 衝突後速度ベクトルの算出
	*pOut_velo_A = -weight_B * ConstVec + *pVelo_A;
	*pOut_velo_B = weight_A * ConstVec + *pVelo_B;

	// 衝突後位置の算出
	*pOut_pos_A = *pColliPos_A + time * (*pOut_velo_A);
	*pOut_pos_B = *pColliPos_B + time * (*pOut_velo_B);

	if (D3DXVec3Length(&(*pOut_pos_A - *pOut_pos_B)) < (0.02855 * 2 - IKD_EPSIRON)) {
		D3DXVECTOR3 A = *pOut_velo_A;		D3DXVec3Normalize(&A, &A); //　単位速度ベクトル
		D3DXVECTOR3 B = *pOut_velo_B;		D3DXVec3Normalize(&B, &B); //　単位速度ベクトル
		*pOut_pos_A = *pColliPos_A + 0.02855 * A;
		*pOut_pos_B = *pColliPos_B + 0.02855 * B;
	}

	return true;
}