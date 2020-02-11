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

class Stump
{
	vector<vector<int>> data; 
public:
	Stump();
	~Stump();
	void setSet(vector<int> str);
	const vector<vector<int>> &getSet() { return data; };
};

struct TStump {
	double a;
	double b;
	double c;
	double best_pr;
	double min_err;
};

const TStump &learn(vector<vector<int>> set);

double cross(int k, vector<vector<int>> set);