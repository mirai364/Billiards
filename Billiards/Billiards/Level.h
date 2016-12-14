#pragma once

#include "Header.h"


void GameToResult() {
	isResult = true;
	isTitleClick = false;
	Sleep(1000);
}

void ResultToTitle() {
	isTitleClick = false;
	isResult = false;
	InitParameter();
	Sleep(1000);
}

void TitleToGame() {
	isTitleClick = true;
	isResult = false;
	Sleep(1000);
}