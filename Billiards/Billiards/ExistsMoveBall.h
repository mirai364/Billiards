#pragma once

#include "Header.h"
#include "BallData.h"
#include "Level.h"

BOOL ExistsMoveBall() {
	// �{�[���̍Ĕz�u
	for (int i = 0; i < 9; i++) {
		if (Ball[i].ValidateSpeed()) { return true; }
	}
	if (hand.ValidateSpeed()) { return true; }

	// �ŏI�I�ɂǂ̃{�[���ɂ�������Ȃ������烊�U���g
	if (!isFirstConflict && isClickAfter) {
		GameToResult();
	}

	isClickAfter = false;
	isFirstConflict = false;
	return false;
}
