#include "Puyo.h"
#include "Define.h"

Puyo::Puyo() {
	mainPuyo = rand() % PUYO_COLOR_MAX;
	subPuyo = rand() % PUYO_COLOR_MAX;
}
