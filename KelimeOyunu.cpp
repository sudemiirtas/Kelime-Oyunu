#include "KelimeOyunu.h"
#include <fstream>
#include <vector>
#include <cmath>
#include <conio.h>
#include <string>

// imleci terminalde farklı bir (x, y) konumuna taşır
void ImlecTasiXY(const short& x, const short& y) {
	COORD kordinatlar = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), kordinatlar);
}

// min ve max sayıları arasında döndürür
template<class T> T GetRandomNumber(const T& min, const T& max) {
	return min + T(double(rand()) / double(RAND_MAX + 1.0) * (max - min + 1));
}


// Zaman constructor
Zaman::Zaman(const short& dakika, const double& saniye) : dakika(dakika), saniye(saniye) {	}

// get methodu,  dakika döndürür
short Zaman::GetDakika() const {
	return dakika;
}
