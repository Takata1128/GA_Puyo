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
#include"PuyoGame.h"

class GA {
public:
	int N; // �̐�
	int gene_len; //��`�q��
	int elite_num; // �G���[�g�I��������̂̐�
	double crossover_rate; // ������
	double mutation_rate; // �ˑR�ψٗ�
	double mutation_gene_rate; // ��`�q�̓ˑR�ψٗ�
	vector<vector<int>> population; // �W�c
	vector<int> score; // �X�R�A
	vector<vector<int>> new_gene_population; //���̏W�c
	vector<vector<int>> elite; // �G���[�g

	int gen_count; // ���݂̐��㐔

	GA(int _N, int _gene_len, int _elite_num, double _crossover_rate, double mr, double mcr);

	int make_operation();

	vector<int> make_gene(int len);

	void simulate(int max_gen, vector<Puyo> Tsumo);

	void generation(vector<Puyo> Tsumo);

	void eval(vector<Puyo> Tsumo);

	void print_info();

	void select();

	void crossover();

	void mutation();

	void next_prepare();

	void show_game(vector<Puyo> Tsumo);

	void play_game(vector<Puyo> Tsumo, int interval);
};