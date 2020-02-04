#include "stdafx.h"
#include "Stump.h"


Stump::Stump() {
}

Stump::Stump(vector<vector<int>> data) {
	set = data;
}

Stump::~Stump()
{
}


void Stump::learn() {

	double best_a, best_b, best_c, best_pr;

	double sum_pow_a = 0, sum_a = 0, count_a = 0, sum_pow_b = 0, sum_b = 0, count_b = 0;
	double sum_pow_all = 0, sum_all = 0, count_all = set.size();
	double min_err = 0, err = 0, err_a = 0, err_b = 0;

	// calculate sum of y and sum of y^2
	for (int l = 0; l < count_all; l++) {
		sum_all += set[l][set[l].size() - 1];
		sum_pow_all += pow(set[l][set[l].size() - 1], 2);
	}
		
	err_b = sum_pow_all - pow(sum_all, 2) / count_all;
	min_err = sqrt((err_a + err_b) / count_all);

	// index of y
	int y = set[0].size() - 1;
	
	for (int pr = 0; pr < y; pr++) {
		
		sort(set.begin(), set.end(), [&pr](vector<int> a, vector<int> b) mutable {return a[pr] < b[pr];});

		// min C
		c = set[0][pr] - 1; 
		
		sum_a = 0;
		sum_pow_a = 0;
		count_a = 0;
		err_a = 0;

		sum_b = sum_all;
		sum_pow_b = sum_pow_all;
		count_b = count_all;
		err_b = sum_pow_b - pow(sum_b, 2) / count_b;

		err = sqrt((err_a + err_b) / count_all);

		if (err < min_err) {
			min_err = err;
			best_a = 0;
			best_b = sum_b / count_b;
			best_c = c;
			best_pr = pr;
		}

		// average C
		for (int i = 0; i < count_all - 1; i++) {
			if (set[i][pr] != set[i + 1][pr]) {

				c = (set[i][pr] + set[i + 1][pr]) * 1.0 / 2;

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
					best_a = sum_a / count_a;
					best_b = sum_b / count_b;
					best_c = c;
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
		c = set[count_all - 1][pr] + 1;  

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
			best_a = sum_a / count_a;
			best_b = sum_b / count_b;
			best_c = c;
			best_pr = pr;
		}
	}

	cout << "pr: " << best_pr + 1 << "\nC: " << best_c << "\nA: " << best_a << "\nB: " << best_b << "\nRMSE: " << min_err;

 	return;
}