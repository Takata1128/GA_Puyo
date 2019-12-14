
#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<vector>
#include<iostream>
#include<time.h>
#include<conio.h>

using namespace std;

#define FIELD_WIDTH 8
#define FIELD_HEIGHT 14

#define PUYO_START_X 3
#define PUYO_START_Y 1

#define PUYO_COLOR_MAX 4

enum {
	CELL_NONE,
	CELL_WALL,
	CELL_PUYO_0,
	CELL_PUYO_1,
	CELL_PUYO_2,
	CELL_PUYO_3,
	CELL_MAX
};

enum {
	PUYO_ANGLE_0,
	PUYO_ANGLE_90,
	PUYO_ANGLE_180,
	PUYO_ANGLE_270,
	PUYO_ANGLE_MAX
};

enum {
	FREE,
	FALL,
	CHAIN
};

int puyoSubPosition[][2] = {
	{0,-1},//PUYO_ANGLE_0,
	{-1,0},//PUYO_ANGLE_90,
	{0,1},//PUYO_ANGLE_180,
	{1,0}//PUYO_ANGLE_270,
};

int cells[FIELD_HEIGHT][FIELD_WIDTH];
int displayBuffer[FIELD_HEIGHT][FIELD_WIDTH];
int checked[FIELD_HEIGHT][FIELD_WIDTH];


const vector<string> states = { "FREE","FALL","CHAIN","COOL" };

vector<Color> cellColors = {
	Color(0,0,0), //NONE
	Color(0,0,0), //WALL
	Color(255,0,0), // RED
	Color(255,255,0), // YERROW
	Color(0,255,0), // GREEN
	Color(0,0,255) // BLUE
};

int puyoX = PUYO_START_X, puyoY = PUYO_START_Y;
int puyoColor;
int subPuyoColor;
int puyoAngle;
int state = FREE;



void display() {
	memcpy(displayBuffer, cells, sizeof cells);

	int subX = puyoX + puyoSubPosition[puyoAngle][0];
	int subY = puyoY + puyoSubPosition[puyoAngle][1];

	if (state == FREE) {
		displayBuffer[puyoY][puyoX] = puyoColor + CELL_PUYO_0;
		displayBuffer[subY][subX] = subPuyoColor + CELL_PUYO_0;
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
}

bool intersectPuyoToField(int _puyoX, int _puyoY, int _puyoAngle) {
	int subX = _puyoX + puyoSubPosition[_puyoAngle][0];
	int subY = _puyoY + puyoSubPosition[_puyoAngle][1];
	if (cells[_puyoY][_puyoX] != CELL_NONE)return true;
	if (cells[subY][subX] != CELL_NONE)return true;

	return false;
}

int getPuyoConnectedCount(int _x, int _y, int _color, int _count) {
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

void erasePuyo(int _x, int _y, int _color) {
	if (cells[_y][_x] != _color)return;
	cells[_y][_x] = CELL_NONE;
	for (int i = 0; i < PUYO_ANGLE_MAX; i++) {
		int x = _x + puyoSubPosition[i][0];
		int y = _y + puyoSubPosition[i][1];
		erasePuyo(x, y, _color);
	}
}

void keyInput() {
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
		angle = (angle+PUYO_ANGLE_MAX-1) % PUYO_ANGLE_MAX;
	}
	if (!intersectPuyoToField(x, y, angle)) {
		puyoX = x;
		puyoY = y;
		puyoAngle = angle;
	}
}

void Main() {
	srand((unsigned int)time(NULL));

	for (int y = 0; y < FIELD_HEIGHT; y++) {
		cells[y][0] = CELL_WALL;
		cells[y][FIELD_WIDTH - 1] = CELL_WALL;
	}

	for (int x = 0; x < FIELD_WIDTH; x++) {
		cells[FIELD_HEIGHT - 1][x] = CELL_WALL;
	}

	puyoColor = rand() % PUYO_COLOR_MAX;
	subPuyoColor = rand() % PUYO_ANGLE_MAX;

	time_t t = 0;

	Scene::SetBackground(ColorF(0.3, 0.6, 1.0));

	while (System::Update()) {
		keyInput();
		display();
		if (t < time(NULL)) {
			t = time(NULL);//１秒ごとに再描画

			//ロック状態でなければ、自由落下する。地面についたらロック状態に

			if (state == FREE) {
				if (!intersectPuyoToField(puyoX, puyoY + 1, puyoAngle))puyoY++;
				else {
					int subX = puyoX + puyoSubPosition[puyoAngle][0];
					int subY = puyoY + puyoSubPosition[puyoAngle][1];
					cells[puyoY][puyoX] = CELL_PUYO_0 + puyoColor;
					cells[subY][subX] = CELL_PUYO_0 + subPuyoColor;
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
				bool chained = false;
				for (int y = 0; y < FIELD_HEIGHT - 1; y++) {
					for (int x = 1; x < FIELD_WIDTH - 1; x++) {
						if (cells[y][x] != CELL_NONE) {
							if (getPuyoConnectedCount(x, y, cells[y][x], 0) >= 4) {
								erasePuyo(x, y, cells[y][x]);
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
					puyoColor = rand() % PUYO_COLOR_MAX;
					subPuyoColor = rand() % PUYO_COLOR_MAX;
					state = FREE;
				}
			}
		}
	}
	
}
