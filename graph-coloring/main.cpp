#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <random>
#include <cmath>
#include <algorithm>
using namespace std;

int min2(int a, int b) {
	return a < b ? a : b;
}

int max2(int a, int b) {
	return a > b ? a : b;
}

bool isInVector(vector<int>values, int x) {
	bool found = false;
	for (int i = 0; i < values.size(); i++) {
		if (values[i] == x) {
			found = true;
			break;
		}
	}
	return found;
}

int calculateConflict(vector<vector<int>>verticesList, vector<int>colors, int vertex) {
	int conflict = 0;
	for (int i = 0; i < verticesList[vertex].size(); i++) {
		if (colors[vertex] == colors[verticesList[vertex][i]]) {
			conflict++;
		}
	}
	return conflict;
}

int calculateTotalConflict(vector<vector<int>>verticesMatrix, vector<int>colors, int verticesNumber) {
	int totalConflict = 0;
	for (int i = 1; i <= verticesNumber; i++) {
		for (int j = 1; j <= verticesNumber; j++) {
			if (verticesMatrix[i][j] == 1) {
				if (colors[i] == colors[j])
				{
					totalConflict++;
					verticesMatrix[i][j] = 0;
					verticesMatrix[j][i] = 0;
				}
			}
		}
	}
	return totalConflict;
}

class ant {
public:
	vector<int>recentlyVisited;
	int currentVertex;
	int colorCurrentVertex(vector<int>colors, int avaliableColors);
};

int ant::colorCurrentVertex(vector<int>colors, int avaliableColors) {

}

const int MODE = 1; //1 - graph coloring; 2- instance generator
int main() {

	if (MODE == 2) {
		//generowanie instancji
		mt19937 mt_rand(time(0));
		int n, v, e;
		cout << "Podaj ilosc instancji:" <<endl;
		cin >> n;
		cout << "Podaj ilosc wierzcholkow:" << endl;
		cin >> v;
		cout << "Podaj ilosc krawedzi (max " << (v * (v - 1) / 2) << ")" << endl;
		cin >> e;
		for (int i = 0; i < n; i++) {
			vector<vector<int>>matrix(v, vector<int>(v, 0));
			string filename = "instance" + to_string(i);
			ofstream instance(filename + ".txt", ofstream::trunc);
			instance << v << " " << e << endl;
			int v1 = 0, v2 = 0;
			int j = 0;
			while(j < e){

				v1 = mt_rand()%v;
				do {
					v2 = mt_rand()%v;
				} while (v1 == v2);
				if (matrix[v1][v2] == 0 && matrix[v2][v1] ==0) {
					matrix[v1][v2] = 1;
					matrix[v2][v1] = 1;
					instance << v1+1<< " " << v2+1 << endl;
					j++;
				}
			}
			instance.close();
		}
		
	}
	else if (MODE==1) {
		int v, a = 0, b = 0;
		cin >> v;
		vector<vector<int>>vertices(v + 1); //tworzymy tablicê list s¹siedztwa
		vector<int>colors(v + 1, -1); //tablica która mówi jaki kolor ma i-ty wierzcho³ek
		vector<vector<int>>verticesMatrix(v + 1, vector<int>(v + 1, 0)); // tworzymy pomocnicz¹ macierz s¹siedztwa to¿sam¹ z list¹
		while (true) { //wczytujemy krawêdzie
			cin >> a >> b;
			if (a == 9999 && b == 9999) { //warunek kiedy przestajemy wczytywaæ
				break;
			}
			vertices[a].push_back(b);
			vertices[b].push_back(a);
			verticesMatrix[a][b] = 1;
			verticesMatrix[b][a] = 1;
		}
		colors[1] = 0; // wierzcho³ki indeksujemy od 1
		for (int i = 2; i <= v; i++) {
			vector<bool>color(v, true); //tablica pomocnicza mówi¹ca o dostêpnoœci kolorów
			for (int j = 0; j < vertices[i].size(); j++) { //przegl¹damy listy s¹siadów: i - numer wierzcho³ka, vertices[i][j] numer j-tego s¹siada wierzcho³ka i
				if (colors[vertices[i][j]] != -1) {
					color[colors[vertices[i][j]]] = false; // oznaczamy kolor
				}
			}
			int j = 0;
			while (!color[j]) { //szukamy pierwszego wolnego koloru
				j++;
			}
			colors[i] = j;

		}
		vector<bool>color(v, true); // tablica do zliczania kolorów
		int colorsNumber = 0;
		for (int i = 1; i <= v; i++) {
			cout << "wierzcholek " << i << " jest pokolorwany kolorem " << colors[i] << endl;
			if (color[colors[i]]) {
				color[colors[i]] = false;
				colorsNumber++;
			}
		}
		cout << "Do pokolorowania grafu uzyto " << colorsNumber << " kolorow" << endl;
		cout << "calkowity konflikt: " << calculateTotalConflict(verticesMatrix, colors, v);

	//############################################################################################################
	//Powy¿ej wygenerowane rozwi¹zanie zach³anne, teraz przygotowanie heurystyki

		const int nAnts = 0.2 * v;
		const int nCycles = min2(6 * v, 4000);
		const float alpha = 0.8, beta = 0.5, gamma = 0.7;
		const int nMoves = nAnts < 100 ? v / 4 : 20 + v / nAnts;
		const int R_SIZE_LIMIT = nMoves / 3;
		const int nChangeCycle = 20;
		const int nJoltCycles = max2(v / 2, 600);
		const int nBreakCycles = max2(5 * v, 1600);
		//start algorytmu
		auto bestColoring = colors;
		int bestNumColors = colorsNumber;
		int availableColors = ceil(alpha * colorsNumber);
		int Bk = ceil(beta * colorsNumber);
		int Gk = ceil(gamma * colorsNumber);
		int totalConflict = 0;
		//wybieramy losowo Bk kolorów
		mt19937 mt_rand(time(0));
		vector<int>BkChosen;
		int i = 0;
		while (i < Bk) {
			int j = (mt_rand() % colorsNumber);
			if (!isInVector(BkChosen, j)) {
				BkChosen.push_back(j);
				i++;
			}
		}
		//wyczyszczenie wierzcho³ków, których kolory nie nale¿¹ od [0, Bk) i przekolorowanie grafu
		for (int i = 0; i < colors.size(); i++) {
			if (colors[i] >= Bk) {
				colors[i] = mt_rand()%Gk;
			}
		}
		vector<int>verticesConflict(v + 1, 0);
		//wyliczamy konflikt w ka¿dym wierzcho³ku
		for (int i = 1; i <= v; i++) {
			verticesConflict[i] = calculateConflict(vertices, colors, i);
		}
		totalConflict = calculateTotalConflict(verticesMatrix, colors, v);
		
		cout << "XD";







	}	
	


	return 0;
}