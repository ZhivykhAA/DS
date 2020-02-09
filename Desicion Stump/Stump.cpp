#include "stdafx.h"
#include "Stump.h"


Stump::Stump() {
}

Stump::~Stump(){
}


void Stump::learn() {

	double ci;

	double sum_pow_a = 0, sum_a = 0, count_a = 0, sum_pow_b = 0, sum_b = 0, count_b = 0;
	double sum_pow_all = 0, sum_all = 0, count_all = set.size();
	double err = 0, err_a = 0, err_b = 0;

	// calculate sum of y and sum of y^2
	for (int l = 0; l < count_all; l++) {
		sum_all += set[l][set[l].size() - 1];
		sum_pow_all += pow(set[l][set[l].size() - 1], 2);
	}
		
	err_b = sum_pow_all - pow(sum_all, 2) / count_all;
	min_err = err_a + err_b;

	// index of y
	int y = set[0].size() - 1;
	
	for (int pr = 0; pr < y; pr++) {
		
		sort(set.begin(), set.end(), [&pr](vector<int> a, vector<int> b) mutable {return a[pr] < b[pr];});

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

		if (err < min_err) {
			min_err = err;
			a = 0;
			b = sum_b / count_b;
			c = ci;
			best_pr = pr;
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

				if (err < min_err) {
					min_err = err;
					a = sum_a / count_a;
					b = sum_b / count_b;
					c = ci;
					best_pr = pr;
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

		if (err < min_err) {
			min_err = err;
			a = sum_a / count_a;
			b = sum_b / count_b;
			c = ci;
			best_pr = pr;
		}
	}

	//cout << "pr: " << best_pr + 1 << "\nC: " << c << "\nA: " << a << "\nB: " << b << "\nRMSE: " << min_err;

 	return;
}



double Stump::cross() {
	int k = 5;
	int k_test = set.size() / k;
	int k_ost = set.size() - k * k_test;
	double err_a, err_b, cross_err = 0;
	
	Stump data, test;

	// проходим K раз по данным, меняя обучающую и тестовую выборки
	for (int i = 0; i < k; i++) {

		data.set = set;
		test.set.erase(test.set.begin(), test.set.end());

		int siz;
		if (k_ost > 0) {
			siz = k_test + 1;
			k_ost--;
		}
		else {
			siz = k_test;
		}
		test.set.resize(siz);

		// создание тестовой выборки
		move(data.set.begin() + siz * i, data.set.begin() + siz * (i + 1), test.set.begin());
		// создание обучающей выборки
		data.set.erase(data.set.begin() + siz * i, data.set.begin() + siz * (i + 1));

		// обучаемся
		data.learn();

		err_a = 0;
		err_b = 0;
		// тестируем
		for (int j = 0; j < test.set.size(); j++) {
			if (test.set[j][test.best_pr] < test.c) {
				err_a += pow(test.set[j][test.set[0].size() - 1] - data.a, 2);
			}
			else {
				err_b += pow(test.set[j][test.set[0].size() - 1] - data.b, 2);
			}
		}		
		cross_err += pow(sqrt((err_a + err_b) / test.set.size()), 2);

		cout << "i = " << i << "  cross = " << cross_err << "\nRMSE: " << data.min_err << endl;
		
		//cout << "i = " << i << "\nC: " << data.c << "\nA: " << data.a << "\nB: " << data.b << "\npr: " << data.best_pr << "\nRMSE: " << data.min_err << endl;
		//cout << data.set.size() << endl << siz * (i) << endl << test.set.size() << endl;
	}
	cross_err = sqrt(cross_err/ k);

	return cross_err;
}