#pragma once
#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<iostream>
#include<time.h>
#include<conio.h>
#include"PuyoManager.h"
#include"Define.h"

class PuyoGame {

	// ��Ղ�̏����ʒu
	int puyoMainPosition[6][2] = {
		{1,1},
		{1,2},
		{1,3},
		{1,4},
		{1,5},
		{1,6}
	};

	// �T�u�Ղ�̎�Ղ悩��̑��Έʒu
	int puyoSubPosition[4][2] = {
		{0,-1},//PUYO_ANGLE_0,
		{-1,0},//PUYO_ANGLE_90,
		{0,1},//PUYO_ANGLE_180,
		{1,0}//PUYO_ANGLE_270,
	};
	
	// �t�B�[���h�z��
	int cells[FIELD_HEIGHT][FIELD_WIDTH];
	// �\���p�z��
	int displayBuffer[FIELD_HEIGHT][FIELD_WIDTH];
	// �A���m�F�p�z��
	int checked[FIELD_HEIGHT][FIELD_WIDTH];

	// �e�}�X�̐F���
	vector<Color> cellColors = {
		Color(0,0,0), //NONE
		Color(0,0,0), //WALL
		Color(255,0,0), // RED
		Color(255,255,0), // YERROW
		Color(0,255,0), // GREEN
		Color(0,0,255) // BLUE
	};

	// �A���{�[�i�X�_
	vector<int> chainBonus = { 0,0,8,16,32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512 };
	// �A���{�[�i�X�_
	vector<int> connectedBonus = { 0,0,0,0,0,2,3,4,5,6,7,10 };

	// ��Ղ�̍��W
	int puyoX, puyoY;
	// ���݂̂Ղ�̐F���
	Puyo nowPuyo;
	// �Ղ�̉�]��
	int puyoAngle;
	// ���݂̏��
	int state;
	// �X�R�A
	int score;
	// ���݂̘A����
	int chain;
	PuyoManager* puyoManager;

public:
	PuyoGame(vector<Puyo> Tsumo);
	int simulate(vector<int> gene);
	int simulate(vector<int> gene, int interval);
	void play(int interval);

private:
	void display(Font font);
	bool intersectPuyoToField(int _puyoX, int _puyoY, int _puyoAngle);
	int getPuyoConnectedCount(int _x, int _y, int _color, int _count);
	void erasePuyo(int _x, int _y, int _color);
	int calcScore(int chainNum, int connectedCount);
	void keyInput();
};