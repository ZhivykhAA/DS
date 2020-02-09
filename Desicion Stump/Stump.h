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
public:
	vector<vector<int>> set; //запоминает data
	double a;
	double b;
	double c;
	double best_pr;
	double min_err;
	void learn();
	double cross();

	Stump();
	~Stump();
};

