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

// Ÿ‚Ì‚Õ‚æ‚ğæ‚èo‚·
Puyo PuyoManager::popPuyo() {
	Puyo ret = Tsumo.at(cur);
	cur = (cur + 1) % ROOP_SIZE;
	return ret;
}

// Ÿ‚Ì‚Õ‚æ‚ğŒ©‚é
Puyo PuyoManager::showNextPuyo() {
	return Tsumo.at((cur) % ROOP_SIZE);
}

//@Ÿ‚ÌŸ‚Ì‚Õ‚æ‚ğŒ©‚é
Puyo PuyoManager::showDNextPuyo() {
	return Tsumo.at((cur+1) % ROOP_SIZE);
}