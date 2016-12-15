#pragma once

#include "Header.h"


void GameToResult() {
	isResult = true;
	isTitleClick = false;
	isTutorial = false;
	Sleep(1000);
}

void ResultToTitle() {
	isTitleClick = false;
	isResult = false;
	isTutorial = false;
	InitParameter();
	Sleep(1000);
}

void TitleToGame() {
	isTitleClick = true;
	isTutorial = true;
	isResult = false;
	Sleep(1000);
}

void TutorialToGame() {
	isTitleClick = true;
	isResult = false;
	isTutorial = false;
	Sleep(1000);
}