#pragma once

// �t�B�[���h�̑傫��
#define FIELD_WIDTH 8
#define FIELD_HEIGHT 15

// �f�t�H���g�̂Ղ�̏����ʒu
#define PUYO_START_X 3
#define PUYO_START_Y 1

// �����ɒu���Ǝ���
#define DEATH_X 3
#define DEATH_Y 2

// �Ղ�̐F��
#define PUYO_COLOR_MAX 4

// ����Ń��[�v���邩
#define ROOP_SIZE 128

// GA�̃p�����[�^
#define POPULATION_SIZE 300
#define GENE_LEN 30
#define MAX_GENERATION 50
#define ELITE_NUM 3
#define CROSSOVER_RATE 0.90
#define MUTATION_RATE 0.10
#define MUTATION_CHRONO_RATE 0.05

// �t�B�[���h�̃}�X�̏��
enum {
	CELL_NONE,
	CELL_WALL,
	CELL_PUYO_0,
	CELL_PUYO_1,
	CELL_PUYO_2,
	CELL_PUYO_3,
	CELL_MAX
};

// �Ղ�̉�]�p�x
enum {
	PUYO_ANGLE_0,
	PUYO_ANGLE_90,
	PUYO_ANGLE_180,
	PUYO_ANGLE_270,
	PUYO_ANGLE_MAX
};

// �Q�[���̏��
enum {
	FREE,
	FALL,
	CHAIN
};