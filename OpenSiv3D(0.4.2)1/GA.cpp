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
#include"GA.h"

GA::GA(int _N, int _gene_len,int _elite_num, double _crossover_rate, double mr, double mcr) :N(_N), gene_len(_gene_len), elite_num(_elite_num), crossover_rate(_crossover_rate), mutation_rate(mr), mutation_gene_rate(mcr) {
	population.resize(N);
	new_gene_population.resize(N);
	score.resize(N);
	elite.resize(elite_num);
	gen_count = 1;
	for (int i = 0; i < N; i++) {
		population[i] = make_gene(gene_len);
	}
}

// 遺伝子をランダムで生成
int GA::make_operation() {
	int one, two, ret = 1;
	while (ret == 1 || ret == 53) {
		one = rand() % 6;
		two = rand() % 4;
		ret = one * 10 + two;
	}
	return ret;
}

// 個体を生成
vector<int> GA::make_gene(int len) {
	vector<int> ret(len);
	for (int i = 0; i < len; i++) {
		ret[i] = make_operation();
	}
	return ret;
}

// 固定配ぷよでGAをシミュレート
void GA::simulate(int max_gen, vector<Puyo> Tsumo) {
	srand((unsigned int)time(NULL));
	while (gen_count <= max_gen) {
		generation(Tsumo);
	}
}

// １世代の流れ
void GA::generation(vector<Puyo> Tsumo) {
	eval(Tsumo);
	print_info();
	if(gen_count == 1 ||gen_count==MAX_GENERATION)show_game(Tsumo);
	select();
	crossover();
	mutation();
	next_prepare();
}

// 評価
void GA::eval(vector<Puyo> Tsumo) {
	for (int i = 0; i < N; i++) {
		PuyoGame Game(Tsumo);
		score[i] = Game.simulate(population[i]);
	}
}

// 情報を表示
void GA::print_info() {
	int max_score = 0;
	int sum = 0;
	for (int i = 0; i < N; i++) {
		max_score = max(max_score, score[i]);
		sum += score[i];
	}
	Console << gen_count;
	Console << max_score;
	Console << sum / N;
}

//　選択
void GA::select() {
	// エリート選択
	vector<pair<int, vector<int>>> vp(N);
	for (int i = 0; i < N; i++)vp[i] = make_pair(score[i], population[i]);
	sort(vp.begin(), vp.end());
	reverse(vp.begin(), vp.end());
	for (int i = 0; i < elite_num; i++) {
		elite[i] = vp[i].second;
	}

	// ルーレット選択
	int total = 0;
	for (int i = 0; i < N; i++)total += score[i];
	for (int i = 0; i < N; i++) {
		double rulet_cf = (double)(rand() % 10000) / 10000.0 * total;
		int rulet_value = 0;
		int rulet_num = 0;
		for (int j = 0; j < N; j++) {
			rulet_value += score[i];
			if (rulet_value > rulet_cf) {
				rulet_num = j;
				break;
			}
		}
		new_gene_population[i] = population[rulet_num];
	}
}

//　交叉
void GA::crossover() {
	for (int i = 0; i < N; i += 2) {
		double p_cross = (double)(rand() % 101) / 100.0;
		if (p_cross < crossover_rate) {
			int pos = (rand() % gene_len);
			for (int j = pos; j < gene_len; j++) {
				swap(new_gene_population[i][j], new_gene_population[i + 1][j]);
			}
		}
	}
}

// 突然変異
void GA::mutation() {
	for (int i = 0; i < N; i++) {
		double p_mutate = (double)(rand() % 101) / 100.0;
		// 全部変える
		if (p_mutate < mutation_gene_rate) {
			for (int j = 0; j < gene_len; j++) {
				new_gene_population[i][j] = make_operation();
			}
		}
		//　確率で変える
		if (p_mutate < mutation_rate) {
			for (int j = 0; j < gene_len; j++) {
				p_mutate = (double)(rand() % 101) / 100.0;
				if (p_mutate < mutation_gene_rate) {
					new_gene_population[i][j] = make_operation();
				}
			}
		}
	}
}

//　次の世代に移る準備
void GA::next_prepare() {
	for (int i = 0; i < elite_num; i++) {
		population[i] = elite[i];
	}
	for (int i = elite_num; i < N; i++) {
		population[i] = new_gene_population[i];
	}
	gen_count++;
}

//　その世代で最良の結果のシミュレーションを見せる
void GA::show_game(vector<Puyo> Tsumo) {
	PuyoGame Game(Tsumo);
	int max_score = 0;
	int max_idx = 0;
	for (int i = 0; i < N; i++) {
		if (score[i] > max_score) {
			max_score = score[i];
			max_idx = i;
		}
	}
	Game.simulate(population[max_idx], 100);
}

// 実際にプレイ（未使用)
void GA::play_game(vector<Puyo> Tsumo,int interval) {
	PuyoGame Game(Tsumo);
	Game.play(interval);
}

// エントリーポイント
void Main() {
	srand((unsigned int)time(NULL));
	GA simulation(POPULATION_SIZE,GENE_LEN,ELITE_NUM,CROSSOVER_RATE,MUTATION_RATE,MUTATION_CHRONO_RATE);
	vector<Puyo> Tsumo(128);
	for (int i = 0; i < 128; i++) {
		Tsumo[i] = Puyo();
	}
	simulation.simulate(MAX_GENERATION,Tsumo);
	while (System::Update()) {
		if (KeyQ.down())break;
	}
}