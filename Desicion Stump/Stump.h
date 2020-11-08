#pragma once
#include "stdafx.h"
#include "fstream"
#include "string"
#include "conio.h"
#include "algorithm"
#include <random>      
#include "iostream"
#include "vector"
#include "math.h"
#include <ctime>
#include <sstream>

using namespace std;


class Data {
	vector<vector<int>> data;
public:
	Data() {};
	~Data() {};

	void setData(const vector<int>& str);
	Data reading_data(const string& file_adress);

	vector<vector<int>> getData() const { return data; }; // изменить
	int sizeD() const { return data.size(); };
	int sizeD_i() const { return data[0].size(); };
	int el(int i, int j) const  { return data[i][j]; };

	void sortD(int pr);
	void randomshuffleD();
	void shuffleD();
	void eraseD(int s1, int s2);
	vector<int> beginTest(int i, int k, const Data& set);
};


class Model {
public:
	double a;
	double b;
	double c;
	double best_pr;
	double min_rmse;
	double min_mae;

	Model() {};
	~Model() {};

	void setVal(double ai, double bi, double ci, double pr, double rmse);
	double rmserrAB(double sum_pow, double sum, int count);
	double rmserr(double err_a, double err_b, int count);
	double maerr(const Data& data);
};

class Learn {
public:
	const Model learnDS(Data set);
};


double* cross_val(int k, Data set, int n);
void printInFile(const char*  fileAdress, Data data);

//void crossValidation(int n, Data data);