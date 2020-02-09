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
	int N; // 個体数
	int gene_len; //遺伝子長
	int elite_num; // エリート選択するものの数
	double crossover_rate; // 交叉率
	double mutation_rate; // 突然変異率
	double mutation_gene_rate; // 遺伝子の突然変異率
	vector<vector<int>> population; // 集団
	vector<int> score; // スコア
	vector<vector<int>> new_gene_population; //次の集団
	vector<vector<int>> elite; // エリート

	int gen_count; // 現在の世代数

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