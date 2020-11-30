#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <random>
using namespace std;

const int MODE = 2; //1 - graph coloring; 2- instance generator
int main() {

	if (MODE == 2) {
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
		while (true) { //wczytujemy krawêdzie
			cin >> a >> b;
			if (a == 9999 && b == 9999) { //warunek kiedy przestajemy wczytywaæ
				break;
			}
			vertices[a].push_back(b);
			vertices[b].push_back(a);
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
		cout << "Do pokolorowania grafu uzyto " << colorsNumber << " kolorow";
	}	

	return 0;
}