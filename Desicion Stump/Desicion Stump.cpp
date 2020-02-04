// Desicion Stump.cpp: определяет точку входа для консольного приложения.
//

#include "Stump.h"


int main()
{
	string line;
	vector<vector<int>> data = {};
	vector<int> str = {};
	string word = "";

	ifstream file("D:\\Machine Learning\\autoPrice.arff");

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
				data.push_back(str);
			}	
		}
	}
	else
		cout << "File is not found";
	file.close();


	Stump stump(data);
	stump.learn();
	

	cout << "\n\nruntime = " << clock() / 1000.0 << endl; // время работы программы  

	_getch();

    return 0;
}

