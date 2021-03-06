#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <new>


using namespace std;

struct dato {
	string clase;
	vector<vector<int>> valores;
};

struct value {
	string clase;
	vector<float> standarDeviation;
	vector<float> arithmeticMean;
};

vector<dato> readCSV(string ruta); //Lee el archivo de excel con extension csv. Regresa un vector con los valores de EMG
void printVector(vector<dato> raws); //Funcion para imprimir el vector de vectores
vector<dato> changeType(vector<vector<string>> raws); //Recibe un vector con vectores de cadenas para regresar un vector con vectores de enteros
vector<value> doMath(vector<dato> vals); //Funcion que realiza los calculos de las desvaiciones estandar para cada una de las clases
vector<float> arithmeticMean(vector<vector<int>> vals); //Funcion para calcular la media aritmetica para una clase
vector<float> standardDeviation(vector<vector<int>> vals, vector<float> mean); //Funcion que calcula la desviacion estandar para una clase
void printVector(vector<value> raws); // Funcion para imprimir el vector de datos
vector<vector<float>> euclideanDistance(vector<value> data);
void printVector(vector<vector<float>> raws);

int main() {
	vector<dato> vals;
	vector<value> values;
	vector<vector<float>> matrix;
	vals = readCSV("excel/valores-final.csv");
	values = doMath(vals);
	printVector(values);
	matrix = euclideanDistance(values);
	printVector(matrix);
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

vector<value> doMath(vector<dato> vals) {
	vector<value> out;
	value aux;
	for (int clase = 0; clase < vals.size(); clase++) {
		aux.clase = vals[clase].clase;
		aux.arithmeticMean = arithmeticMean(vals[clase].valores);
		aux.standarDeviation = standardDeviation(vals[clase].valores, aux.arithmeticMean);
		out.push_back(aux);
	}
	return out;
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

vector<float> standardDeviation(vector<vector<int>> vals, vector<float> mean) {
	float out[8] = { 0 };
	vector<float> salida;
	for (int i = 0; i < vals.size(); i++) {
		for (int j = 0; j < vals[i].size(); j++) {
			out[j] = out[j] + pow(abs(vals[i][j] - mean[j]),2);
		}
	}
	for (int i = 0; i < (sizeof(out) / sizeof(*out)); i++) {
		out[i] = sqrt(out[i] / vals.size());
		salida.push_back(out[i]);
	}
	return salida;
}

void printVector(vector<value> raws) {
	for (int i = 0; i < raws.size(); i++) {
		cout << "-> " << raws[i].clase << '\t';
		for (int j = 0; j < raws[i].standarDeviation.size(); j++) {
			cout << raws[i].standarDeviation[j] << '\t';
		}
		cout << endl;
	}
}

vector<vector<float>> euclideanDistance(vector<value> data) {
	vector<vector<float>> out;
	vector<float> aux;
	float val = 0;
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < data.size(); j++) {
			for (int k = 0; k < data[i].standarDeviation.size(); k++) {
				val = val + pow(data[i].standarDeviation[k] - data[j].standarDeviation[k],2);
			}
			val = sqrt(val);
			aux.push_back(val);
			val = 0;
		}
		out.push_back(aux);
		aux.clear();
	}
	return out;
}

void printVector(vector<vector<float>> raws) {
	for (int i = 0; i < raws.size(); i++) {
		for (int j = 0; j < raws[i].size(); j++) {
			cout << raws[i][j] << '\t';
		}
		cout << endl;
	}
}

/*
ROLES

Usuario: cliente
		 miembro
		 consumidor
		 beneficiario
		 beneficiado
		 comprador
		 interesado
*/