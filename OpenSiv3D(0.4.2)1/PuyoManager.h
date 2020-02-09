#pragma once
#include <vector>
#include "Puyo.h"

using namespace std;

class PuyoManager
{
public:
	PuyoManager(vector<Puyo> Tsumo);
	// �Ղ�z��
	vector<Puyo> Tsumo;
	// ���݂̎Q�ƈʒu
	int cur;

	Puyo popPuyo();
	Puyo showNextPuyo();
	Puyo showDNextPuyo();
};
