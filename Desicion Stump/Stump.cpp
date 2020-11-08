#include "stdafx.h"
#include "Stump.h"


/* --   Data   --  */

void Data::setData(const vector<int>& str) {
	data.push_back(str);
}

// reading data from file
Data Data::reading_data(const string& file_adress) {

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

void Data::randomshuffleD() {
	random_shuffle(data.begin(), data.end());
};

void Data::shuffleD() { 
	shuffle(data.begin(), data.end(), default_random_engine(1)); 
};

void Data::eraseD(int s1, int s2) { 
	data.erase(data.begin() + s1, data.begin() + s2); 
};

vector<int> Data::beginTest(int i, int k, const Data& set) {
	vector<int> begT(2);

	int sizeT = set.sizeD() / k;
	int begin_test;
	int ost = set.sizeD() % k;

	if (i < ost) {  
		sizeT++;
		begin_test = i * sizeT;
	}
	else {
		begin_test = ost * (sizeT + 1) + (i - ost) * sizeT;
	}

	begT[0] = begin_test;
	begT[1] = sizeT;

	return begT;
};


/* --   Model   -- */

void Model::setVal(double ai, double bi, double ci, double pr, double rmse) {
	a = ai;
	b = bi;
	c = ci;
	best_pr = pr;
	min_rmse = rmse;
};

double Model::rmserrAB(double sum_pow, double sum, int count) {
	return sum_pow - pow(sum, 2) / count;
};


double Model::rmserr(double err_a, double err_b, int count) {
	return sqrt((err_a + err_b) / count);
};

double Model::maerr(const Data& data) {
	double mae = 0;
	int count = 0;

	for (int i = 0; i < data.sizeD(); i++) {
		count++;
		if (data.el(i, best_pr) < c) {
			mae += (abs(data.el(i, data.sizeD_i() - 1) - a) - mae) / count;
		}
		else {
			mae += (abs(data.el(i, data.sizeD_i() - 1) - b) - mae) / count;
		}
	}

	return mae;
}



/* --   Learn   -- */

// learn desicion stump
const Model Learn::learnDS(Data set) {
	Model Stump;

	int count_all = set.sizeD();
	double ci = 0, ai = 0, bi = 0;
	double rmserr = 0, rmserr_a = 0, rmserr_b = 0;
	double sum_pow_a = 0, sum_a = 0, count_a = 0, sum_pow_b = 0, sum_b = 0, count_b = 0;
	double sum_pow_all = 0, sum_all = 0;

	// index of y
	int y = set.sizeD_i() - 1;

	// calculate sum of y and sum of y^2
	for (int l = 0; l < count_all; l++) {
		sum_all += set.el(l, y);
		sum_pow_all += pow(set.el(l, y), 2);
	}

	rmserr_a = 0;
	rmserr_b = Stump.rmserrAB(sum_pow_all, sum_all, count_all);


	Stump.min_rmse = rmserr_a + rmserr_b;


	for (int pr = 0; pr < y; pr++) {

		set.sortD(pr);

		// min C
		ci = set.el(0, pr) - 1;

		sum_a = 0;
		sum_pow_a = 0;
		count_a = 0;
		rmserr_a = 0;

		sum_b = sum_all;
		sum_pow_b = sum_pow_all;
		count_b = count_all;
		rmserr_b = Stump.rmserrAB(sum_pow_b, sum_b, count_b);

		rmserr = Stump.rmserr(rmserr_a, rmserr_b, count_all);

		if (rmserr < Stump.min_rmse) {
			bi = sum_b / count_b;
			Stump.setVal(0.0, bi, ci, pr, rmserr);
		}

		// average C
		for (int i = 0; i < count_all - 1; i++) {
			if (set.el(i, pr) != set.el(i + 1, pr)) {

				ci = (set.el(i, pr) + set.el(i + 1, pr)) * 1.0 / 2;

				sum_a += set.el(i, y);
				sum_pow_a += pow(set.el(i, y), 2);
				count_a++;
				rmserr_a = Stump.rmserrAB(sum_pow_a, sum_a, count_a);

				sum_b -= set.el(i, y);
				sum_pow_b -= pow(set.el(i, y), 2);
				count_b--;
				rmserr_b = Stump.rmserrAB(sum_pow_b, sum_b, count_b);

				rmserr = Stump.rmserr(rmserr_a, rmserr_b, count_all);

				if (rmserr < Stump.min_rmse) {
					ai = sum_a / count_a;
					bi = sum_b / count_b;
					Stump.setVal(ai, bi, ci, pr, rmserr);
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
		rmserr_a = Stump.rmserrAB(sum_pow_a, sum_a, count_a);

		sum_b = 0;
		sum_pow_b = 0;
		count_b = 0;
		rmserr_b = 0;

		rmserr = Stump.rmserr(rmserr_a, rmserr_b, count_all);

		if (rmserr < Stump.min_rmse) {
			ai = sum_a / count_a;
			bi = sum_b / count_b;
			Stump.setVal(ai, bi, ci, pr, rmserr);
		}
	}

	Stump.min_mae = Stump.maerr(set);

	return Stump;
}


// cross-validation
double* cross_val(int k, Data set, int n) {

	double* errors = new double[2];

	int k_test = set.sizeD() / k;
	int k_ost = set.sizeD() - k * k_test;
	double err_a, err_b, err_a1, err_b1, cross_err_sqrt = 0, cross_err_abs = 0, min_err = 0;

	Data data, test;
	Model St;
	Learn L;
	int begin_test = 0;

	vector<int> bTest(2);

	
	set.randomshuffleD();

	string s;
	stringstream out;
	out << n;
	s = out.str();

	string str = "shufflecpu" + s + ".arff";
	char* ch = new char[str.size()];
	for (int p = 0; p < str.size(); ++p) {
		ch[p] = str[p];
	}

	const char* chstr = ch;

	printInFile(chstr, set);

	set.shuffleD();

	// run data K times, by changing learn and test dataset
	for (int i = 0; i < k; i++) {

		data = set;
		St.setVal(NULL, NULL, NULL, NULL, NULL);

		// dataTest start index and size 
		bTest = set.beginTest(i, k, set);

		// create learn dataset
		data.eraseD(bTest[0], bTest[0] + bTest[1]);

		// learn
		St = L.learnDS(data);

		// test
		err_a = 0;
		err_b = 0;
		err_a1 = 0;
		err_b1 = 0;
		for (int j = bTest[0]; j < bTest[0] + bTest[1]; j++) {
			if (set.el(j, St.best_pr) <= St.c) {
				err_a += pow(set.el(j, set.sizeD_i() - 1) - St.a, 2);
				err_a1 += abs(set.el(j, set.sizeD_i() - 1) - St.a);
			}
			else {
				err_b += pow(set.el(j, set.sizeD_i() - 1) - St.b, 2);
				err_b1 += abs(set.el(j, set.sizeD_i() - 1) - St.b);
			}
		}
		
		cross_err_sqrt += (err_a + err_b) / bTest[1];
		cross_err_abs += (err_a1 + err_b1) / bTest[1]; 
	}

	cross_err_abs /= k;
	cross_err_sqrt = sqrt(cross_err_sqrt / k);

	errors[0] = cross_err_abs;
	errors[1] = cross_err_sqrt;

	return errors;
}



void printInFile(const char* fileAdress, Data data) {

	FILE* file;
	fopen_s(&file, fileAdress, "w");

	fprintf(file, "@relation 'cpu'\n@attribute MYCT numeric\n@attribute MMIN numeric\n@attribute MMAX numeric\n@attribute CACH numeric\n@attribute CHMIN numeric\n@attribute CHMAX numeric\n@attribute class numeric\n@data\n");

	for (size_t i = 0; i < data.sizeD(); i++) {
		for (size_t j = 0; j < data.sizeD_i() - 1; j++) {
			fprintf(file, "%i,", data.el(i,j));
		}
		fprintf(file, "%i\n", data.el(i, data.sizeD_i() - 1));
	}
	fclose(file);

}



//
//void crossValidation(int n, Data data)	// cross validation method (n > 1)
//{
//	if (n < 2)
//	{
//		cout << "Number of folds must be more than 1" << endl;
//		return;
//	}
//	double sqroot = 0, abserror = 0;
//	int *folds;
//	folds = new int[n];
//	int h = data.sizeD() % n;
//	int kb = 0, ke = 0;
//	for (int i = 0; i < n; ++i)
//	{
//		folds[i] = (data.sizeD() / n) + (i < h ? 1 : 0);
//	}
//
//	data.shuffleD();      // shuffle all data for better random
//
//	for (int i = 0; i < n; ++i)
//	{
//		Data test, train;
//		kb = ke;
//		ke += folds[i];
//		for (int j = 0; j < data.sizeD(); ++j)
//		{
//			double *arr;
//			arr = new double[TS.size];
//			arr = TS.data[j];
//
//			if ((j >= kb) && (j < ke))
//			{
//				test.data.push_back(arr);
//			}
//			else
//			{
//				train.data.push_back(arr);
//			}
//		}
//
//		DecFunc DF(train);
//		DF.findFunc();
//		DF.printRes();
//		sqroot += DF.testSqrtSample(test);
//		abserror += DF.testAbsSample(test);
//
//	}
//	sqroot /= n;
//	abserror /= n;
//
//	cout << "Instances : " << TS.data.size() << endl;
//	cout << "Attributes : " << TS.size << endl;
//	cout << "Cross Validation.  Folds : " << n << endl;
//	cout << "=== SUMMARY ===" << endl;
//	cout << "Mean absolute error : " << abserror << endl;
//	cout << "Root mean squared error : " << sqroot << endl;
//	return;
//}
