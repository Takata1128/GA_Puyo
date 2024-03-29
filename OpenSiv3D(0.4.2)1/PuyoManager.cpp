#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<iostream>
#include<time.h>
#include<conio.h>
#include "Puyo.h"
#include "PuyoManager.h"

PuyoManager::PuyoManager(vector<Puyo> Tsumo) {
	Tsumo.resize(ROOP_SIZE);
	this->Tsumo = Tsumo;
	cur = 0;
}

// 次のぷよを取り出す
Puyo PuyoManager::popPuyo() {
	Puyo ret = Tsumo.at(cur);
	cur = (cur + 1) % ROOP_SIZE;
	return ret;
}

// 次のぷよを見る
Puyo PuyoManager::showNextPuyo() {
	return Tsumo.at((cur) % ROOP_SIZE);
}

//　次の次のぷよを見る
Puyo PuyoManager::showDNextPuyo() {
	return Tsumo.at((cur+1) % ROOP_SIZE);
}