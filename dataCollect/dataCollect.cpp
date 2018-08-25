#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>


using namespace std;

struct dato {
	string clase;
	vector<vector<int>> valores;
};

struct value {
	string clase;
	float* standarDeviation;
	vector<float> arithmeticMean;
};

vector<dato> readCSV(string ruta); //Lee el archivo de excel con extension csv. Regresa un vector con los valores de EMG
void printVector(vector<dato> raws); //Funcion para imprimir el vector de vectores
vector<dato> changeType(vector<vector<string>> raws); //Recibe un vector con vectores de cadenas para regresar un vector con vectores de enteros
void doMath(vector<dato> vals);
vector<float> arithmeticMean(vector<vector<int>> vals);
float* standardDeviation(vector<vector<int>> vals, vector<float> mean);

int main() {
	vector<dato> vals;
	vector<int> sigma;
	vals = readCSV("excel/valores-final.csv");
	doMath(vals);
	return 0;
}

vector<dato> readCSV(string ruta) {
	ifstream  data(ruta);
	vector<string> raw;
	vector<vector<string> > raws;

	string line;
	while (getline(data, line))
	{
		stringstream  lineStream(line);
		string        cell;
		while (getline(lineStream, cell, ','))
		{
			raw.push_back(cell);
		}
		raws.push_back(raw);
		raw.clear();
	}
	return changeType(raws);
}

void printVector(vector<dato> raws) {
	for (int i = 0; i < raws.size(); i++) {
		cout << raws[i].clase << endl;
		for (int j = 0; j < raws[i].valores.size(); j++) {
			for (int k = 0; k < raws[i].valores[j].size(); k++) {
				cout << raws[i].valores[j][k] << '\t';
			}
			cout << endl;
		}
		cout << endl;
	}
}

vector<dato> changeType(vector<vector<string>> raws) {
	vector<dato> vals;
	vector<int> val;
	dato aux;
	int x = 0;
	bool in;
	for (int i = 1; i < raws.size(); i++) {
		in = false;
		if (vals.size() == 0) {
			for (int j = 0; j < raws[i].size() - 1; j++) {
				stringstream stream(raws[i][j]);
				stream >> x;
				val.push_back(x);
			}
			aux.clase = raws[i][raws[i].size() - 1];
			aux.valores.push_back(val);
			vals.push_back(aux);
			val.clear();
			aux = dato();
		}
		else {
			for (int j = 0; j < raws[i].size() - 1; j++) {
				stringstream stream(raws[i][j]);
				stream >> x;
				val.push_back(x);
			}
			for (int j = 0; j < vals.size(); j++) {
				if (vals[j].clase == raws[i][raws[i].size() - 1]) {
					vals[j].valores.push_back(val);
					val.clear();
					in = true;
					break;
				}
			}
			if (in == false) {
				aux.clase = raws[i][raws[i].size() - 1];
				aux.valores.push_back(val);
				vals.push_back(aux);
				val.clear();
				aux = dato();
			}
		}
	}
	return vals;
}

void doMath(vector<dato> vals) {
	vector<value> out;
	value aux;
	for (int clase = 0; clase < vals.size(); clase++) {
		aux.clase = vals[clase].clase;
		aux.arithmeticMean = arithmeticMean(vals[clase].valores);
		aux.standarDeviation = standardDeviation(vals[clase].valores, aux.arithmeticMean);
	}
}

vector<float> arithmeticMean(vector<vector<int>> vals) {
	float temp[8] = { 0 };
	vector<float> out;
	for (int i = 0; i < vals.size(); i++) {
		for (int j = 0; j < vals[i].size(); j++) {
			temp[j] = temp[j] + vals[i][j];
		}
	}
	for (int i = 0; i < (sizeof(temp) / sizeof(*temp)); i++) {
		out.push_back(temp[i]/vals.size());
	}
	return out;
}

float* standardDeviation(vector<vector<int>> vals, vector<float> mean) {
	float out[8] = { 0 };
	for (int i = 0; i < vals.size(); i++) {
		for (int j = 0; j < vals[i].size(); j++) {
			out[j] = out[j] + pow(abs(vals[i][j] - mean[j]),2);
		}
	}
	for (int i = 0; i < (sizeof(out) / sizeof(*out)); i++) {
		out[i] = sqrt(out[i] / vals.size());
		cout << out[i] << endl;
	}
	cout << endl << endl;
	return out;
}