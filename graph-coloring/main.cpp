#include <iostream>
#include <vector>

using namespace std;

int main() {
	int v, a = 0, b = 0;
	cin >> v;
	vector<vector<int>>vertices(v + 1); //tworzymy tablic� list s�siedztwa
	vector<int>colors(v + 1, -1); //tablica kt�ra m�wi jaki kolor ma i-ty wierzcho�ek
	while (true) { //wczytujemy kraw�dzie
		cin >> a >> b;
		if (a == 9999 && b == 9999) { //warunek kiedy przestajemy wczytywa�
			break;
		}
		vertices[a].push_back(b);
		vertices[b].push_back(a);
	}
	colors[1] = 0; // wierzcho�ki indeksujemy od 1
	for (int i = 2; i <= v; i++) {
		vector<bool>color(v, true); //tablica pomocnicza m�wi�ca o dost�pno�ci kolor�w
		for (int j = 0; j < vertices[i].size(); j++) { //przegl�damy listy s�siad�w: i - numer wierzcho�ka, vertices[i][j] numer j-tego s�siada wierzcho�ka i
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
	vector<bool>color(v, true); // tablica do zliczania kolor�w
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