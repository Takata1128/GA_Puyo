#pragma once

// フィールドの大きさ
#define FIELD_WIDTH 8
#define FIELD_HEIGHT 15

// デフォルトのぷよの初期位置
#define PUYO_START_X 3
#define PUYO_START_Y 1

// ここに置くと死ぬ
#define DEATH_X 3
#define DEATH_Y 2

// ぷよの色数
#define PUYO_COLOR_MAX 4

// 何手でループするか
#define ROOP_SIZE 128

// GAのパラメータ
#define POPULATION_SIZE 300
#define GENE_LEN 30
#define MAX_GENERATION 50
#define ELITE_NUM 3
#define CROSSOVER_RATE 0.90
#define MUTATION_RATE 0.10
#define MUTATION_CHRONO_RATE 0.05

// フィールドのマスの状態
enum {
	CELL_NONE,
	CELL_WALL,
	CELL_PUYO_0,
	CELL_PUYO_1,
	CELL_PUYO_2,
	CELL_PUYO_3,
	CELL_MAX
};

// ぷよの回転角度
enum {
	PUYO_ANGLE_0,
	PUYO_ANGLE_90,
	PUYO_ANGLE_180,
	PUYO_ANGLE_270,
	PUYO_ANGLE_MAX
};

// ゲームの状態
enum {
	FREE,
	FALL,
	CHAIN
};