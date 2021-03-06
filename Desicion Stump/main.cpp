#include "stdafx.h"
#include "Stump.h"


int main()
{
	Data data;
	Model St;
	Learn L;
	string file_adress = "D:\\Machine Learning\\Weka-3-8-4\\data\\cpu.arff";

	data = data.reading_data(file_adress);

	St = L.learnDS(data);
	//cout << "stump" << "\nC: " << St.c << "\nA: " << St.a << "\nB: " << St.b << "\nRMSE: " << St.min_err << endl << endl;
	cout << "MMAX <= " << St.c << " : " << St.a << endl;
	cout << "MMAX > " << St.c << " : " << St.b << endl;
	cout << "\n=== Summary ===\n\n";
	cout << "Mean absolute error       " << St.min_mae << endl;
	cout << "Root mean squared error   " << St.min_rmse << endl;

	int k;
	cout << "\nCross-Validation: count of folds - ";
	cin >> k;

	int n = 10;
	double err_abs = 0, err_sqr = 0;
	double* errors = new double[2];
	for (size_t i = 0; i < n; i++) {
		errors = cross_val(k, data, i + 1);
		err_abs += errors[0];
		err_sqr += errors[1];
	}

	cout << "\n=== Cross-validation ===\n=== Summary === \n\n";
	cout << "Mean absolute error       " << err_abs / n << endl;
	cout << "Root mean squared error   " << err_sqr / n << endl;

	cout << "\n\nruntime = " << clock() / 1000.0 << endl; // running time
	
	_getch();

    return 0;
}

