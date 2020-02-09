#pragma once
#include <vector>
#include "Puyo.h"

using namespace std;

class PuyoManager
{
public:
	PuyoManager(vector<Puyo> Tsumo);
	// Ç’ÇÊîzóÒ
	vector<Puyo> Tsumo;
	// åªç›ÇÃéQè∆à íu
	int cur;

	Puyo popPuyo();
	Puyo showNextPuyo();
	Puyo showDNextPuyo();
};
