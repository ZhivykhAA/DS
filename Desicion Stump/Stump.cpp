#include "stdafx.h"
#include "Stump.h"


/* --   Data   --  */

void Data::setData(vector<int> str) {
	data.push_back(str);
}

// reading data from file
Data Data::reading_data(string file_adress) {

	Data st;
	string line;
	string word = "";
	vector<int> str = {};

	ifstream file(file_adress);

	if (file.is_open()) {
		while (getline(file, line)) {
			if (!line.empty() && line[0] != '%' && line[0] != '@') {
				str = {};
				for (int i = 0; i < line.length(); i++) {

					if (line[i] != ',') {
						word += line[i];
					}
					else {
						str.push_back(stoi(word));
						word = "";
					}

					if (i == line.length() - 1) {
						str.push_back(stoi(word));
						word = "";
					}
				}
				st.setData(str);
			}
		}
	}
	else
		cout << "File is not found";
	file.close();

	return st;
}

void Data::sortD(int pr) { 
	sort(data.begin(), data.end(), [&pr](vector<int> a, vector<int> b) mutable {return a[pr] < b[pr]; }); 
};

void Data::shuffleD() { 
	random_shuffle(data.begin(), data.end()); 
};

void Data::eraseD(int s1, int s2) { 
	data.erase(data.begin() + s1, data.begin() + s2); 
};


/* --   Stump   -- */

void Stump::setVal(double ai, double bi, double ci, double pr, double err) {
	a = ai;
	b = bi;
	c = ci;
	best_pr = pr;
	min_err = err;
};

double Stump::errAB(double sum_pow, double sum, int count) {
	return sum_pow - pow(sum, 2) / count;
};
double Stump::err(double err_a, double err_b, int count) {
	return sqrt((err_a + err_b) / count);
};


/* --   Learn   -- */

// learn desicion stump
const Stump Learn::learnDS(Data set) {
	Stump S;

	int count_all = set.sizeD();
	double ci = 0, ai = 0, bi = 0;
	double err = 0, err_a = 0, err_b = 0;
	double sum_pow_a = 0, sum_a = 0, count_a = 0, sum_pow_b = 0, sum_b = 0, count_b = 0;
	double sum_pow_all = 0, sum_all = 0;

	// index of y
	int y = set.sizeD_i() - 1;

	// calculate sum of y and sum of y^2
	for (int l = 0; l < count_all; l++) {
		sum_all += set.el(l, y);
		sum_pow_all += pow(set.el(l, y), 2);
	}

	err_a = 0;
	err_b = S.errAB(sum_pow_all, sum_all, count_all);

	S.min_err = err_a + err_b;


	for (int pr = 0; pr < y; pr++) {

		set.sortD(pr);

		// min C
		ci = set.el(0, pr) - 1;

		sum_a = 0;
		sum_pow_a = 0;
		count_a = 0;
		err_a = 0;

		sum_b = sum_all;
		sum_pow_b = sum_pow_all;
		count_b = count_all;
		err_b = S.errAB(sum_pow_b, sum_b, count_b);

		err = S.err(err_a, err_b, count_all);

		if (err < S.min_err) {
			bi = sum_b / count_b;
			S.setVal(0.0, bi, ci, pr, err);
		}

		// average C
		for (int i = 0; i < count_all - 1; i++) {
			if (set.el(i, pr) != set.el(i + 1, pr)) {

				ci = (set.el(i, pr) + set.el(i + 1, pr)) * 1.0 / 2;

				sum_a += set.el(i, y);
				sum_pow_a += pow(set.el(i, y), 2);
				count_a++;
				err_a = S.errAB(sum_pow_a, sum_a, count_a);

				sum_b -= set.el(i, y);
				sum_pow_b -= pow(set.el(i, y), 2);
				count_b--;
				err_b = S.errAB(sum_pow_b, sum_b, count_b);

				err = S.err(err_a, err_b, count_all);

				if (err < S.min_err) {
					ai = sum_a / count_a;
					bi = sum_b / count_b;
					S.setVal(ai, bi, ci, pr, err);
				}
			}
			else {
				sum_a += set.el(i, y);
				sum_pow_a += pow(set.el(i, y), 2);
				count_a++;

				sum_b -= set.el(i, y);
				sum_pow_b -= pow(set.el(i, y), 2);
				count_b--;
			}
		}

		// max C
		ci = set.el(count_all - 1, pr) + 1;

		sum_a = sum_all;
		sum_pow_a = sum_pow_all;
		count_a = count_all;
		err_a = S.errAB(sum_pow_a, sum_a, count_a);

		sum_b = 0;
		sum_pow_b = 0;
		count_b = 0;
		err_b = 0;

		err = S.err(err_a, err_b, count_all);

		if (err < S.min_err) {
			ai = sum_a / count_a;
			bi = sum_b / count_b;
			S.setVal(ai, bi, ci, pr, err);
		}
	}

	return S;
}


// cross-validation
double cross_val(int k, Data set) {

	int k_test = set.sizeD() / k;
	int k_ost = set.sizeD() - k * k_test;
	double err_a, err_b, cross_err = 0, min_err;

	Data data;
	Stump St;
	Learn L;
	int begin_test = 0;

	set.shuffleD();

	// run data K times, by changing learn and test dataset
	for (int i = 0; i < k; i++) {

		data = set;
		St.a = NULL;
		St.b = NULL;
		St.c = NULL;
		St.best_pr = NULL;
		min_err = NULL;


		// распределение остатка равномерно по участкам датасета
		int siz;
		if (k_ost > 0) {
			siz = k_test + 1;
			k_ost--;
		}
		else {
			siz = k_test;
		}

		// create learn dataset
		data.eraseD(begin_test, begin_test + siz);
		int a;
		// learn
		St = L.learnDS(data);

		// test
		err_a = 0;
		err_b = 0;
		for (int j = begin_test; j < begin_test + siz; j++) {
			if (set.el(j, St.best_pr) < St.c) {
				err_a += pow(set.el(j, set.sizeD_i() - 1) - St.a, 2);
			}
			else {
				err_b += pow(set.el(j, set.sizeD_i() - 1) - St.b, 2);
			}
		}
		cross_err += pow(sqrt((err_a + err_b) / siz), 2);

		begin_test += siz;
	}

	cross_err = sqrt(cross_err / k);

	return cross_err;
}

