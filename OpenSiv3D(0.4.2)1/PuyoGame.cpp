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
#include"PuyoGame.h"

using namespace std;

PuyoGame::PuyoGame(vector<Puyo> Tsumo) {
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		cells[y][0] = CELL_WALL;
		cells[y][FIELD_WIDTH - 1] = CELL_WALL;
	}

	for (int x = 0; x < FIELD_WIDTH; x++) {
		cells[FIELD_HEIGHT - 1][x] = CELL_WALL;
	}

	puyoManager = new PuyoManager(Tsumo);
	nowPuyo = puyoManager->popPuyo();
	state = FREE;
	score = 0;
	chain = 0;
	puyoX = PUYO_START_X;
	puyoY = PUYO_START_Y;

	Scene::SetBackground(ColorF(0.3, 0.6, 1.0));
	Font font(30);
}

void PuyoGame::display(Font font) {
	memcpy(displayBuffer, cells, sizeof cells);

	int subX = puyoX + puyoSubPosition[puyoAngle][0];
	int subY = puyoY + puyoSubPosition[puyoAngle][1];

	if (state == FREE) {
		displayBuffer[puyoY][puyoX] = nowPuyo.mainPuyo + CELL_PUYO_0;
		displayBuffer[subY][subX] = nowPuyo.subPuyo + CELL_PUYO_0;
	}


	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			//printf("%s", cellNames[displayBuffer[y][x]]);
			if (displayBuffer[y][x] >= CELL_PUYO_0 && displayBuffer[y][x] <= CELL_PUYO_3) {
				Circle(100 + 20 * x, 100 + 20 * y, 9).draw(cellColors[displayBuffer[y][x]]);
			}
			else if (displayBuffer[y][x] == CELL_WALL) {
				Rect(Arg::center(100 + 20 * x, 100 + 20 * y), 15).draw(cellColors[displayBuffer[y][x]]);
			}
		}
	}

	// Death
	Rect(Arg::center(100 + 20 * DEATH_X, 100 + 20 * DEATH_Y), 15).draw(Color(100));

	// NextPuyo
	Circle(100 + 20 * (FIELD_WIDTH + 1), 100 + 20 * 1, 9).draw(cellColors[puyoManager->showNextPuyo().mainPuyo + CELL_PUYO_0]);
	Circle(100 + 20 * (FIELD_WIDTH + 1), 100 + 20 * 0, 9).draw(cellColors[puyoManager->showNextPuyo().subPuyo + CELL_PUYO_0]);

	// DoubleNextPuyo
	Circle(100 + 20 * (FIELD_WIDTH + 2), 100 + 20 * 2, 9).draw(cellColors[puyoManager->showDNextPuyo().mainPuyo + CELL_PUYO_0]);
	Circle(100 + 20 * (FIELD_WIDTH + 2), 100 + 20 * 1, 9).draw(cellColors[puyoManager->showDNextPuyo().subPuyo + CELL_PUYO_0]);

	// Score
	font(score).draw(100 + 20 * (FIELD_WIDTH + 2), 100 + 20 * FIELD_HEIGHT);
}

// ぷよが接地したかどうか
bool PuyoGame::intersectPuyoToField(int _puyoX, int _puyoY, int _puyoAngle) {
	int subX = _puyoX + puyoSubPosition[_puyoAngle][0];
	int subY = _puyoY + puyoSubPosition[_puyoAngle][1];
	if (cells[_puyoY][_puyoX] != CELL_NONE)return true;
	if (cells[subY][subX] != CELL_NONE)return true;

	return false;
}

//　連結しているぷよを数える
int PuyoGame::getPuyoConnectedCount(int _x, int _y, int _color, int _count) {
	if (checked[_y][_x] || cells[_y][_x] != _color)return _count;
	_count++;
	checked[_y][_x] = true;
	for (int i = 0; i < PUYO_ANGLE_MAX; i++) {
		int x = _x + puyoSubPosition[i][0];
		int y = _y + puyoSubPosition[i][1];
		_count = getPuyoConnectedCount(x, y, _color, _count);
	}
	return _count;
}

// ぷよを消す処理
void PuyoGame::erasePuyo(int _x, int _y, int _color) {
	if (cells[_y][_x] != _color)return;
	cells[_y][_x] = CELL_NONE;
	for (int i = 0; i < PUYO_ANGLE_MAX; i++) {
		int x = _x + puyoSubPosition[i][0];
		int y = _y + puyoSubPosition[i][1];
		erasePuyo(x, y, _color);
	}
}

// スコア計算
int PuyoGame::calcScore(int chainNum, int connectedCount) {
	return max(10, connectedCount * (connectedBonus[connectedCount] + chainBonus[chainNum]) * 10);
}

// 実際にプレイするときのキー入力(未使用)
void PuyoGame::keyInput() {
	int x = puyoX;
	int y = puyoY;
	int angle = puyoAngle;
	if (KeyLeft.down()) {
		x--;
	}
	else if (KeyRight.down()) {
		x++;
	}
	else if (KeyDown.pressed()) {
		y++;
	}
	else if (KeyA.down()) {
		angle = (++angle) % PUYO_ANGLE_MAX;
	}
	else if (KeyS.down()) {
		angle = (angle + PUYO_ANGLE_MAX - 1) % PUYO_ANGLE_MAX;
	}
	if (!intersectPuyoToField(x, y, angle)) {
		puyoX = x;
		puyoY = y;
		puyoAngle = angle;
	}
}

// ゲームのシミュレーション
int PuyoGame::simulate(vector<int> gene) {
	clock_t t = 0;
	int operationCur = 0;
	score = 0;
	puyoManager->cur = 0;
	int subX, subY;
	bool setFlag = true;

	while (1) {
		/* 全部さばいたら終了 */
		if (operationCur == gene.size())break;
		
		subX = puyoX + puyoSubPosition[puyoAngle][0];
		subY = puyoY + puyoSubPosition[puyoAngle][1];

		if (setFlag) {
			int operation = gene[operationCur];
			int mainPos = operation / 10;
			int subPos = operation % 10;
			puyoAngle = subPos % 4;
			puyoX = puyoMainPosition[mainPos][1];
			puyoY = puyoMainPosition[mainPos][0];
			subX = puyoX + puyoSubPosition[puyoAngle][0];
			subY = puyoY + puyoSubPosition[puyoAngle][1];
			setFlag = false;
		}

		//ロック状態でなければ、自由落下する。地面についたらロック状態に

		if (state == FREE) {
			if (!intersectPuyoToField(puyoX, puyoY + 1, puyoAngle))puyoY++;
			else {
				cells[puyoY][puyoX] = CELL_PUYO_0 + nowPuyo.mainPuyo;
				cells[subY][subX] = CELL_PUYO_0 + nowPuyo.subPuyo;
				state = FALL;
			}
		}

		//ロック状態ならば、ぷよの落下処理と消す処理

		if (state == FALL) {
			while (1) {
				int fallCount = 0;
				for (int y = FIELD_HEIGHT - 3; y >= 0; y--) {
					for (int x = 1; x < FIELD_WIDTH - 1; x++) {
						if ((cells[y][x] != CELL_NONE) && (cells[y + 1][x] == CELL_NONE)) {
							cells[y + 1][x] = cells[y][x];
							cells[y][x] = CELL_NONE;
							fallCount++;
						}
					}
				}
				/* ぷよがこれ以上落ちなくなったら、連鎖処理へ */
				if (!fallCount) {
					state = CHAIN;
					break;
				}
			}
		}

		if (state == CHAIN) {
			memset(checked, 0, sizeof checked);
			chain++;
			bool chained = false;
			for (int y = 0; y < FIELD_HEIGHT - 1; y++) {
				for (int x = 1; x < FIELD_WIDTH - 1; x++) {
					if (cells[y][x] != CELL_NONE) {
						int connectedCount = getPuyoConnectedCount(x, y, cells[y][x], 0);
						if (connectedCount >= 4) {
							erasePuyo(x, y, cells[y][x]);
							score += calcScore(chain, min(connectedCount,11));
							chained = true;
						}
					}
				}
			}
			if (chained)state = FALL;
			else {
				puyoX = PUYO_START_X;
				puyoY = PUYO_START_Y;
				puyoAngle = PUYO_ANGLE_0;
				nowPuyo = puyoManager->popPuyo();
				state = FREE;
				chain = 0;
			}
			//死亡判定
			if (cells[DEATH_Y][DEATH_X] != CELL_NONE)break;
			operationCur++;
			setFlag = true;
		}
	}
	return score;
}

// ゲームのシミュレーション（見せる用）
int PuyoGame::simulate(vector<int> gene, int interval) {
	Font font(30);
	clock_t t = 0;
	score = 0;
	int operationCur = 0;
	puyoManager->cur = 0;
	int subX, subY;
	bool setFlag = true;

	while (System::Update()) {
		/* 全部さばいたら終了 */
		if (operationCur == gene.size())break;

		subX = puyoX + puyoSubPosition[puyoAngle][0];
		subY = puyoY + puyoSubPosition[puyoAngle][1];

		if (setFlag) {
			/*
		 mainPos: 0~5
		 subPos: 0~3
		 01,53はダメ
		*/
			int operation = gene[operationCur];
			int mainPos = operation / 10;
			int subPos = operation % 10;
			puyoAngle = subPos % 4;
			puyoX = puyoMainPosition[mainPos][1];
			puyoY = puyoMainPosition[mainPos][0];
			subX = puyoX + puyoSubPosition[puyoAngle][0];
			subY = puyoY + puyoSubPosition[puyoAngle][1];
			setFlag = false;
		}

		display(font);

		if (t + interval < clock()) {
			t = clock();//１秒ごとに再描画

			//ロック状態でなければ、自由落下する。地面についたらロック状態に

			if (state == FREE) {
				if (!intersectPuyoToField(puyoX, puyoY + 1, puyoAngle))puyoY++;
				else {
					cells[puyoY][puyoX] = CELL_PUYO_0 + nowPuyo.mainPuyo;
					cells[subY][subX] = CELL_PUYO_0 + nowPuyo.subPuyo;
					state = FALL;
				}
			}

			//ロック状態ならば、ぷよの落下処理と消す処理

			if (state == FALL) {
				while (1) {
					int fallCount = 0;
					for (int y = FIELD_HEIGHT - 3; y >= 0; y--) {
						for (int x = 1; x < FIELD_WIDTH - 1; x++) {
							if ((cells[y][x] != CELL_NONE) && (cells[y + 1][x] == CELL_NONE)) {
								cells[y + 1][x] = cells[y][x];
								cells[y][x] = CELL_NONE;
								fallCount++;
							}
						}
					}
					/* ぷよがこれ以上落ちなくなったら、連鎖処理へ */
					if (!fallCount) {
						state = CHAIN;
						break;
					}
				}
			}

			if (state == CHAIN) {
				memset(checked, 0, sizeof checked);
				chain++;
				bool chained = false;
				for (int y = 0; y < FIELD_HEIGHT - 1; y++) {
					for (int x = 1; x < FIELD_WIDTH - 1; x++) {
						if (cells[y][x] != CELL_NONE) {
							int connectedCount = getPuyoConnectedCount(x, y, cells[y][x], 0);
							if (connectedCount >= 4) {
								erasePuyo(x, y, cells[y][x]);
								score += calcScore(chain, min(11,connectedCount));
								chained = true;
							}
						}
					}
				}
				if (chained)state = FALL;
				else {
					puyoX = PUYO_START_X;
					puyoY = PUYO_START_Y;
					puyoAngle = PUYO_ANGLE_0;
					nowPuyo = puyoManager->popPuyo();
					state = FREE;
					chain = 0;
				}
				//死亡判定
				if (cells[DEATH_Y][DEATH_X] != CELL_NONE)break;
				operationCur++;
				setFlag = true;
			}
		}
	}
	return score;
}

// 実際にプレイ（未使用）
void PuyoGame::play(int interval) {
	Font font(30);
	clock_t t = 0;
	int operationCur = 0;

	int subX, subY;
	bool setFlag = true;

	while (System::Update()) {
		keyInput();
		subX = puyoX + puyoSubPosition[puyoAngle][0];
		subY = puyoY + puyoSubPosition[puyoAngle][1];
		if (setFlag) {
			puyoAngle = 0;
			puyoX = PUYO_START_X;
			puyoY = PUYO_START_Y;
			subX = puyoX + puyoSubPosition[puyoAngle][0];
			subY = puyoY + puyoSubPosition[puyoAngle][1];
			setFlag = false;
		}

		display(font);

		if (t + interval < clock()) {
			t = clock();//１秒ごとに再描画

			//ロック状態でなければ、自由落下する。地面についたらロック状態に

			if (state == FREE) {
				if (!intersectPuyoToField(puyoX, puyoY + 1, puyoAngle))puyoY++;
				else {
					cells[puyoY][puyoX] = CELL_PUYO_0 + nowPuyo.mainPuyo;
					cells[subY][subX] = CELL_PUYO_0 + nowPuyo.subPuyo;
					state = FALL;
				}
			}

			//ロック状態ならば、ぷよの落下処理と消す処理

			if (state == FALL) {
				while (1) {
					int fallCount = 0;
					for (int y = FIELD_HEIGHT - 3; y >= 0; y--) {
						for (int x = 1; x < FIELD_WIDTH - 1; x++) {
							if ((cells[y][x] != CELL_NONE) && (cells[y + 1][x] == CELL_NONE)) {
								cells[y + 1][x] = cells[y][x];
								cells[y][x] = CELL_NONE;
								fallCount++;
							}
						}
					}
					/* ぷよがこれ以上落ちなくなったら、連鎖処理へ */
					if (!fallCount) {
						state = CHAIN;
						break;
					}
				}
			}

			if (state == CHAIN) {
				memset(checked, 0, sizeof checked);
				chain++;
				bool chained = false;
				for (int y = 0; y < FIELD_HEIGHT - 1; y++) {
					for (int x = 1; x < FIELD_WIDTH - 1; x++) {
						if (cells[y][x] != CELL_NONE) {
							int connectedCount = getPuyoConnectedCount(x, y, cells[y][x], 0);
							if (connectedCount >= 4) {
								erasePuyo(x, y, cells[y][x]);
								score += calcScore(chain, connectedCount);
								chained = true;
							}
						}
					}
				}
				if (chained)state = FALL;
				else {
					puyoX = PUYO_START_X;
					puyoY = PUYO_START_Y;
					puyoAngle = PUYO_ANGLE_0;
					nowPuyo = puyoManager->popPuyo();
					state = FREE;
					chain = 0;
				}
				//死亡判定
				if (cells[DEATH_Y][DEATH_X] != CELL_NONE)break;
				operationCur++;
				setFlag = true;
			}
		}
	}
}