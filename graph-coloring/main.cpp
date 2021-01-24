#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <random>
#include <cmath>
#include <algorithm>
#include <deque>
#include <utility>
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
	int s = verticesList[vertex].size();
	for (int i = 0; i < s; i++) {
		if (colors[vertex] == colors[verticesList[vertex][i]]) {
			conflict++;
		}
	}
	return conflict;
}

int calculateTotalConflict(vector<vector<int>>verticesMatrixOrginal, vector<int>colors, int verticesNumber) {
	int totalConflict = 0;
	vector<vector<int>>verticesMatrix = verticesMatrixOrginal;
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
void jolt(vector<int>& colors, vector<int>verticesConflict, vector<vector<int>>verticesList, int avaliableColors) {
	mt19937 mt_rand(time(0));
	vector<pair<int, int>>conflictList;
	int avaliable80= ceil(0.8 * avaliableColors);
	for (int i = 0; i < verticesConflict.size(); i++) {
		conflictList.push_back(make_pair(verticesConflict[i], i));
	}
	int s = conflictList.size();
	sort(conflictList.rbegin(), conflictList.rend());
	int i = 0;
	while ((double)i / (double)s < 0.10) {
		int vertexNum = conflictList[i].second;
		for (int j = 0; j < verticesList[vertexNum].size(); j++) {
			colors[j] = mt_rand() % avaliable80;
		}
		i++;
	}
	return;
}
class ant {
public:
	int tabuListSize;
	deque<int>recentlyVisited;
	int currentVertex = -1;
	ant(int, int);
	int colorCurrentVertex(vector<int>&colors, vector<vector<int>>verticesList, int avaliableColors);
	void updateNeighbourConflict(vector<int>&verticesConflict, vector<vector<int>>verticesList, vector<int>colors, int prevColor);
	void addCurrentVertexToTabu();
	void move(vector<vector<int>>verticesList, vector<int>verticesConflict);
};

ant::ant(int size, int vertex) {
	tabuListSize = size;
	currentVertex = vertex;
}

int ant::colorCurrentVertex(vector<int>&colors, vector<vector<int>>verticesList, int avaliableColors) {
	mt19937 mt_rand(time(0));
	int startColor = colors[currentVertex];
	int minIndex, minConflict, minConflictColor, minConflictIndex;
	vector<pair<int,int>>conflictNumbers;
	vector<int>neighbourColors(avaliableColors, 0);
	int s = verticesList[currentVertex].size();
	for (int i = 0; i < s; i++) {
		neighbourColors[colors[verticesList[currentVertex][i]]]++;
	}
	int currentConflict = 0;
	for (int i = 0; i < avaliableColors; i++) {
		conflictNumbers.push_back(make_pair(neighbourColors[i], i));
	}

	sort(conflictNumbers.begin(), conflictNumbers.end());
	minIndex = 0;
	minConflict = conflictNumbers[0].first;
	for (int i = 0; i < avaliableColors; i++) {
		if (conflictNumbers[i].first > minConflict) {
			minIndex = i;
			break;
		}
	}
	minConflictIndex = mt_rand() % minIndex;
	minConflictColor = conflictNumbers[minConflictIndex].second;
	if (!recentlyVisited.empty()) {
		if (minConflictColor = colors[recentlyVisited.back()])
		{
			conflictNumbers.erase(conflictNumbers.begin() + minConflictIndex);
		}
	}
	minConflictIndex = mt_rand() % minIndex;
	minConflictColor = conflictNumbers[minConflictIndex].second;
	colors[currentVertex] = minConflictColor;
	return startColor;
}

void ant::updateNeighbourConflict(vector<int>&verticesConflict, vector<vector<int>>verticesList, vector<int>colors, int prevColor) {
	int currentColor = colors[currentVertex];
	verticesConflict[currentVertex] = calculateConflict(verticesList, colors, currentVertex);
	int l = verticesList[currentVertex].size();
	for (int i = 0; i < l; i++) {
		if (currentColor != colors[verticesList[currentVertex][i]] && prevColor == colors[verticesList[currentVertex][i]]) {
			verticesConflict[verticesList[currentVertex][i]]--;
		}
		else if (currentColor != colors[verticesList[currentVertex][i]] && prevColor != colors[verticesList[currentVertex][i]]) {

		}
		else {
			verticesConflict[verticesList[currentVertex][i]]++;
		}
	}
	return;
}

void ant::addCurrentVertexToTabu() {
	if (recentlyVisited.size() >= tabuListSize) {
		recentlyVisited.pop_front();
	}
	recentlyVisited.push_back(currentVertex);
	return;
}

void ant::move(vector<vector<int>>verticesList, vector<int>verticesConflict) {
	mt19937 mt_rand(time(0));
	vector<int>conflictNumbers;
	int maxIndex, maxConflict;
	int prevVertex = currentVertex;
	int k;
	int s = verticesList[currentVertex].size();
	if (s != 0) {
		int z = 0;
		do {
			k = mt_rand() % s;
			currentVertex = verticesList[prevVertex][k];
			int l = verticesList[prevVertex].size();

			for (int i = 0; i < l; i++) {
				conflictNumbers.push_back(verticesConflict[verticesList[prevVertex][i]]);
			}
			maxIndex = 0; maxConflict = conflictNumbers[0];
			for (int i = 0; i < conflictNumbers.size(); i++) {
				if (conflictNumbers[i] > maxConflict) {
					maxConflict = conflictNumbers[i];
					maxIndex = i;
				}
			}
			currentVertex = verticesList[prevVertex][maxIndex];
			z++;
		} while ((find(recentlyVisited.begin(), recentlyVisited.end(), currentVertex) != recentlyVisited.end()) && z <20);
		
	}	
	return;
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
		const float alpha = 0.88, beta = 0.5, gamma = 0.7;
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
		int cyclesNotImproved = 0;
		//generowanie mrówek
		vector<ant>ants;
		for (int i = 0; i < nAnts; i++) {
			ant tempAnt(R_SIZE_LIMIT, (mt_rand() % v) + 1);
			ants.push_back(tempAnt);
		}
		for (int cycle = 0; cycle < nCycles; cycle++) {
			for (int antNum = 0; antNum < nAnts; antNum++) {
				for (int move = 0; move < nMoves; move++) {
					int prevColor;
					prevColor = ants[antNum].colorCurrentVertex(colors, vertices, availableColors);
					ants[antNum].updateNeighbourConflict(verticesConflict, vertices, colors, prevColor);
					ants[antNum].addCurrentVertexToTabu();
					ants[antNum].move(vertices, verticesConflict);
				}
			}
			totalConflict = calculateTotalConflict(verticesMatrix, colors, v);
			if (totalConflict == 0 && bestNumColors > availableColors) {
				vector<bool>newBestColoring(v, true);
				int newBestColorsNumber = 0;
				cout << endl << "####################### ZNALEZIONO LEPSZE POKOLOROWANIE    ###########################" << endl << availableColors << "kolorow " << endl;
				bestColoring = colors;
				bestNumColors = availableColors;
				availableColors = availableColors - 1;
				cyclesNotImproved = 0;
				for (int i = 1; i <= v; i++) {
					cout << "wierzcholek " << i << " jest pokolorwany kolorem " << bestColoring[i] << endl;
					if (newBestColoring[bestColoring[i]]) {
						newBestColoring[bestColoring[i]] = false;
						newBestColorsNumber++;
					}
				}
				cout << "Do pokolorowania grafu uzyto " << newBestColorsNumber << " kolorow" << endl;
				for (int i = 1; i <= v; i++) {
					if (colors[i] > availableColors) {
						colors[i] = mt_rand()%bestNumColors;
					}
				}
			}
			else {
				cyclesNotImproved++;
			}
			if (cyclesNotImproved >= nChangeCycle && availableColors <bestNumColors) {
				availableColors += 1;
			}
			if (cyclesNotImproved >= nJoltCycles) {
				jolt(colors, verticesConflict, vertices, availableColors);
			}
			if (cyclesNotImproved >= nBreakCycles) {
				break;
			}
		}


		vector<bool>color2(v, true); // tablica do zliczania kolorów
		int colorsNumber2 = 0;
		for (int i = 1; i <= v; i++) {
			cout <<endl<< "wierzcholek " << i << " jest pokolorwany kolorem " << bestColoring[i] << endl;
			if (color2[bestColoring[i]]) {
				color2[bestColoring[i]] = false;
				colorsNumber2++;
			}
		}
		cout << "Do pokolorowania grafu uzyto " << colorsNumber2 << " kolorow" << endl;
		cout << "XD";


	}	
	

	
	return 0;
}