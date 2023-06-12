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
// terminalde güncelleme icin
void Zaman::UpdateZamanDisplay(clock_t& begin, const clock_t& after, const short& x, const short& y) const {
	if (double(after - begin) / double(CLOCKS_PER_SEC) >= 0.9) {		//  her 0.9 saniyede zamanı günceller
		ImlecTasiXY(x, y);
		std::cout << "     ";
		ImlecTasiXY(x, y);
		std::cout << dakika << ":" << std::setfill('0') << std::setw(2) << short(saniye);
		begin = after;
	}
}



// KelimeOyunu constructor
KelimeOyunu::KelimeOyunu(const short& dakika, const double& saniye, const short& tahmin_m, const double& tahmin_s) : current_time(dakika, saniye), tahmin(tahmin_m, tahmin_s) {	}

// oyun baslar
void KelimeOyunu::Basla() {
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	for (unsigned short i = MIN_HARF_SAYISI; i <= MAX_HARF_SAYISI; i++)
		for (unsigned short j = 0; j < HARF_BASI_SORU_S; j++)
			SoruyuOku(i);
	PrintOpeningScreen();
	PrintLoadingScreen();
	GetNextQuestion();
	Update();
}

} while (current_time.UpdateZaman(before, after));
current_time.SetZaman(0, 0.0);
	OyunKapanis();
	SkoruKaydet();
}
// yapıyı gunceller
void KelimeOyunu::Update() {
	clock_t begin = clock(), before, after = clock();
	do {
		current_time.UpdateZamanDisplay(begin, after, 16, 14);
		if (_kbhit())
			switch (toupper(_getch())) {
			case TAHMIN:					// eger  oyuncu tahmin (T )tusuna basasarsa, zaman durdur
				ZamaniDurdur(before, after);	// ve oyuncuya cevaplamsı icin 30 saniye baslar
				break;
			case HARF_ALAYIM:				// eger oyuncu harf almak isterse 'H', bir harf acılır
				HarfAl();				
				break;
			case DURDURMA_TUSU: {				// oyunu durdur
				clock_t begin_temp = clock(), after_temp;
				char key;
				do {
					key = _getch();
					if (key == CIKIS_TUSU)
						exit(0);
				} while ((key = toupper(key)) != DURDURMA_TUSU);
				after_temp = clock();
				double past_time = double(after_temp - begin_temp) / double(CLOCKS_PER_SEC);
				current_time.IncreaseZaman(short(past_time) / 60, fmod(past_time, 60.0));	// oyun duraklatıldığında geçen süreyi zamanlayıcıya ekler
				break; }
			case CIKIS_TUSU:
				std::cout << "\n\n\n\n";
				exit(0);
			}
		before = after;
		after = clock();
	} while (current_time.UpdateZaman(before, after));
	current_time.SetZaman(0, 0.0);
	OyunKapanis();
	SkoruKaydet();
}

