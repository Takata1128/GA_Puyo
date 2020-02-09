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

	// 主ぷよの初期位置
	int puyoMainPosition[6][2] = {
		{1,1},
		{1,2},
		{1,3},
		{1,4},
		{1,5},
		{1,6}
	};

	// サブぷよの主ぷよからの相対位置
	int puyoSubPosition[4][2] = {
		{0,-1},//PUYO_ANGLE_0,
		{-1,0},//PUYO_ANGLE_90,
		{0,1},//PUYO_ANGLE_180,
		{1,0}//PUYO_ANGLE_270,
	};
	
	// フィールド配列
	int cells[FIELD_HEIGHT][FIELD_WIDTH];
	// 表示用配列
	int displayBuffer[FIELD_HEIGHT][FIELD_WIDTH];
	// 連鎖確認用配列
	int checked[FIELD_HEIGHT][FIELD_WIDTH];

	// 各マスの色情報
	vector<Color> cellColors = {
		Color(0,0,0), //NONE
		Color(0,0,0), //WALL
		Color(255,0,0), // RED
		Color(255,255,0), // YERROW
		Color(0,255,0), // GREEN
		Color(0,0,255) // BLUE
	};

	// 連鎖ボーナス点
	vector<int> chainBonus = { 0,0,8,16,32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512 };
	// 連結ボーナス点
	vector<int> connectedBonus = { 0,0,0,0,0,2,3,4,5,6,7,10 };

	// 主ぷよの座標
	int puyoX, puyoY;
	// 現在のぷよの色情報
	Puyo nowPuyo;
	// ぷよの回転数
	int puyoAngle;
	// 現在の状態
	int state;
	// スコア
	int score;
	// 現在の連鎖数
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