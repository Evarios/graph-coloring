#include <iostream>
#include <vector>

using namespace std;

int main() {
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
		while (!color[j]){ //szukamy pierwszego wolnego koloru
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


	return 0;
}