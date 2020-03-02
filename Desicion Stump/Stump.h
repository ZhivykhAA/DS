#pragma once
#include "stdafx.h"
#include "fstream"
#include "string"
#include "conio.h"
#include "algorithm"
#include "iostream"
#include "vector"
#include "math.h"
#include <ctime>

using namespace std;


class Data {
	vector<vector<int>> data;
public:
	Data() {};
	~Data() {};

	void setData(vector<int> str);
	Data reading_data(string file_adress);

	const vector<vector<int>> &getData() { return data; };
	const int sizeD() { return data.size(); };
	const int sizeD_i() { return data[0].size(); };
	const int el(int i, int j) { return data[i][j]; };

	void sortD(int pr);
	void shuffleD();
	void eraseD(int s1, int s2);
};


class Stump {
public:
	double a;
	double b;
	double c;
	double best_pr;
	double min_err;

	Stump() {};
	~Stump() {};

	void setVal(double ai, double bi, double ci, double pr, double err);
	double errAB(double sum_pow, double sum, int count);
	double err(double err_a, double err_b, int count);
};

class Learn {
public:
	const Stump learnDS(Data set);
};


double cross_val(int k, Data set);