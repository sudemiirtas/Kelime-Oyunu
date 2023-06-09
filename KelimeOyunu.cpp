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
//get methodu,  saniye dondurur
double Zaman::GetSaniye() const {
return saniye;
}

// set methodu, sets dakika ve saniye
void Zaman::SetZaman(const short& dakika, const double& saniye) {
this->dakika = dakika;
this->saniye = saniye;
}

// saniyeleri dakika cinsine dodnurme
void Zaman::IncreaseZaman(const short& m_inc, const double& s_inc) {
dakika += m_inc;
saniye += s_inc;
if (saniye >= 60.0) {
saniye -= 60.0;
dakika++;
}
}

// zamanı guncellemek icin
bool Zaman::UpdateZaman(const clock_t& before, const clock_t& after) {
saniye -= double(after - before) / double(CLOCKS_PER_SEC);
if (saniye < 0.0) {
saniye += 60.0;
if ((--dakika) < 0)
return false;
}
return true;
}
