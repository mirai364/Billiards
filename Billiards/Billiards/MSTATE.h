#pragma once

#include "Header.h"

struct MSTATE {
	RECT    moveRect;     // ��ʏ�œ�����͈�
	int     x;            // X���W
	int     y;            // Y���W
	bool    lButton;      // ���{�^��
	bool    rButton;      // �E�{�^��
	bool    cButton;      // �^�񒆃{�^��
	int     moveAdd;      // �ړ���
}MState;