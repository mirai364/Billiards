#pragma once

#include "Header.h"
#include "BallData.h"
#include "Level.h"

BOOL ExistsMoveBall() {
	// ボールの再配置
	for (int i = 0; i < 9; i++) {
		if (Ball[i].ValidateSpeed()) { return true; }
	}
	if (hand.ValidateSpeed()) { return true; }

	// 最終的にどのボールにも当たらなっかたらリザルト
	if (!isFirstConflict && isClickAfter) {
		GameToResult();
	}

	isClickAfter = false;
	isFirstConflict = false;
	return false;
}
