// Desicion Stump.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Stump.h"


int main()
{
	string line;
	string word = "";
	Stump stump;
	TStump St;
	vector<int> str = {};

	ifstream file("D:\\Machine Learning\\Weka-3-8-4\\data\\cpu.arff");

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
				stump.setSet(str);
			}	
		}
	}
	else
		cout << "File is not found";
	file.close();


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

