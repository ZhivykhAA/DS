// Desicion Stump.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Stump.h"


int main()
{
	Stump stump;
	TStump St;
	
	stump = reading_data();

	St = learn(stump.getSet());
	cout << "stump" << "\nC: " << St.c << "\nA: " << St.a << "\nB: " << St.b << "\nRMSE: " << St.min_err << endl << endl;

	double cross_err;
	int k;
	cout << "Cross-Validation: count of folds - ";
	cin >> k;

	cross_err = cross(k, stump.getSet());
	cout << "cross_err : " << cross_err;

	cout << "\n\nruntime = " << clock() / 1000.0 << endl; // running time

	_getch();

    return 0;
}

