#include "stdafx.h"
#include "Stump.h"

int main()
{
	Data data;
	Stump St;
	Learn L;
	string file_adress = "D:\\Machine Learning\\Weka-3-8-4\\data\\cpu.arff";

	data = data.reading_data(file_adress);

	St = L.learnDS(data);
	cout << "stump" << "\nC: " << St.c << "\nA: " << St.a << "\nB: " << St.b << "\nRMSE: " << St.min_err << endl << endl;

	
	double cross_err;
	int k;
	cout << "Cross-Validation: count of folds - ";
	cin >> k;

	cross_err = cross_val(k, data);
	cout << "cross_err : " << cross_err;

	cout << "\n\nruntime = " << clock() / 1000.0 << endl; // running time
	
	_getch();

    return 0;
}

