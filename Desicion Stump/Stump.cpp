#include "stdafx.h"
#include "Stump.h"

Stump::Stump() {
}

Stump::~Stump() {
}

void Stump::setSet(vector<int> str) {
	data.push_back(str);
}


const TStump &learn(vector<vector<int>> set) {
	TStump S;
	double ci;

	double sum_pow_a = 0, sum_a = 0, count_a = 0, sum_pow_b = 0, sum_b = 0, count_b = 0;
	double sum_pow_all = 0, sum_all = 0;
	int count_all = set.size();
	double err = 0, err_a = 0, err_b = 0;

	// calculate sum of y and sum of y^2
	for (int l = 0; l < count_all; l++) {
		sum_all += set[l][set[l].size() - 1];
		sum_pow_all += pow(set[l][set[l].size() - 1], 2);
	}

	err_b = sum_pow_all - pow(sum_all, 2) / count_all;
	S.min_err = err_a + err_b;

	// index of y
	int y = set[0].size() - 1;

	for (int pr = 0; pr < y; pr++) {

		sort(set.begin(), set.end(), [&pr](vector<int> a, vector<int> b) mutable {return a[pr] < b[pr]; });

		// min C
		ci = set[0][pr] - 1;

		sum_a = 0;
		sum_pow_a = 0;
		count_a = 0;
		err_a = 0;

		sum_b = sum_all;
		sum_pow_b = sum_pow_all;
		count_b = count_all;
		err_b = sum_pow_b - pow(sum_b, 2) / count_b;

		err = sqrt((err_a + err_b) / set.size());

		if (err < S.min_err) {
			S.min_err = err;
			S.a = 0;
			S.b = sum_b / count_b;
			S.c = ci;
			S.best_pr = pr;
		}

		// average C
		for (int i = 0; i < count_all - 1; i++) {
			if (set[i][pr] != set[i + 1][pr]) {

				ci = (set[i][pr] + set[i + 1][pr]) * 1.0 / 2;

				sum_a += set[i][y];
				sum_pow_a += pow(set[i][y], 2);
				count_a++;
				err_a = sum_pow_a - pow(sum_a, 2) / count_a;

				sum_b -= set[i][y];
				sum_pow_b -= pow(set[i][y], 2);
				count_b--;
				err_b = sum_pow_b - pow(sum_b, 2) / count_b;

				err = sqrt((err_a + err_b) / set.size());

				if (err < S.min_err) {
					S.min_err = err;
					S.a = sum_a / count_a;
					S.b = sum_b / count_b;
					S.c = ci;
					S.best_pr = pr;
				}
			}
			else {
				sum_a += set[i][y];
				sum_pow_a += pow(set[i][y], 2);
				count_a++;

				sum_b -= set[i][y];
				sum_pow_b -= pow(set[i][y], 2);
				count_b--;
			}
		}

		// max C
		ci = set[count_all - 1][pr] + 1;

		sum_a = sum_all;
		sum_pow_a = sum_pow_all;
		count_a = count_all;
		err_a = sum_pow_a - pow(sum_a, 2) / count_a;

		sum_b = 0;
		sum_pow_b = 0;
		count_b = 0;
		err_b = 0;

		err = sqrt((err_a + err_b) / set.size());

		if (err < S.min_err) {
			S.min_err = err;
			S.a = sum_a / count_a;
			S.b = sum_b / count_b;
			S.c = ci;
			S.best_pr = pr;
		}
	}

	return S;
}


double cross(int k, vector<vector<int>> set) {

	int k_test = set.size() / k;
	int k_ost = set.size() - k * k_test;
	double err_a, err_b, cross_err = 0, min_err;

	vector<vector<int>> data, test;
	TStump St;

	random_shuffle(set.begin(), set.end());

	// run data K times, by changing learn and test dataset
	for (int i = 0; i < k; i++) {

		data = set;
		St.a = NULL;
		St.b = NULL;
		St.c = NULL;
		St.best_pr = NULL;
		min_err = NULL;

		test.erase(test.begin(), test.end());

		int siz;
		if (k_ost > 0) {
			siz = k_test + 1;
			k_ost--;
		}
		else {
			siz = k_test;
		}
		test.resize(siz);

		// create test dataset
		move(data.begin() + siz * i, data.begin() + siz * (i + 1), test.begin());
		// create learn dataset
		data.erase(data.begin() + siz * i, data.begin() + siz * (i + 1));

		// learn
		St = learn(data);

		// test
		err_a = 0;
		err_b = 0;
		for (int j = 0; j < test.size(); j++) {
			if (test[j][St.best_pr] < St.c) {
				err_a += pow(test[j][test[0].size() - 1] - St.a, 2);
			}
			else {
				err_b += pow(test[j][test[0].size() - 1] - St.b, 2);
			}
		}
		cross_err += pow(sqrt((err_a + err_b) / test.size()), 2);
	}

	cross_err = sqrt(cross_err / k);

	return cross_err;
}