#pragma once

#include "Header.h"

struct MSTATE {
	RECT    moveRect;     // 画面上で動ける範囲
	int     x;            // X座標
	int     y;            // Y座標
	bool    lButton;      // 左ボタン
	bool    rButton;      // 右ボタン
	bool    cButton;      // 真ん中ボタン
	int     moveAdd;      // 移動量
}MState;