#ifndef WORD_GAME_H
#define WORD_GAME_H

#define TURKCE_K 1254
#define TAHMIN 'T'
#define HARF_ALAYIM 'H'
#define DURDURMA_TUSU 'P'
#define CIKIS_TUSU 27
#define YENİDEN_BASLAT 'R'
#define KAYIT_GOR 'Y'
#define OYUN_SURESI 240
#define TAHMIN_SURESI 30
#define MIN_HARF_SAYISI 4
#define MAX_HARF_SAYISI 10
#define HARF_BASI_SORU_S 2
#define HARF_BASI_PUAN 100

class Zaman;
class KelimeOyunu;

#include <iostream>
#include <iomanip>
#include <queue>
#include <time.h>
#include <windows.h>

// Zaman class tanımlama
class Zaman {
private:
short dakika;
double saniye;
public:
Zaman(const short& dakika, const double& saniye = 0.0);
short GetDakika() const;
double GetSaniye() const;
void SetZaman(const short& dakika, const double& saniye);
void IncreaseZaman(const short& m_inc, const double& s_inc);
bool UpdateZaman(const clock_t& before, const clock_t& after);
void UpdateZamanDisplay(clock_t& begin, const clock_t& after, const short& x, const short& y) const;
};
// KelimeOyunu class tanımlama
class KelimeOyunu {
private:
	int score = 0;
	unsigned short no_of_undisplayed_letters = MIN_HARF_SAYISI;
	std::string current_word, current_soru, word_on_display;
	std::queue<std::string> SveC;
	Zaman current_time, tahmin;
	void Update();
	void HarfAl();
	void ZamaniDurdur(clock_t& before1, clock_t& after1);
	void ZamaniDurdurUtil(clock_t& before, clock_t& after, clock_t& before1, clock_t& after1, const bool& guessed_correct);
	static void PrintOpeningScreen();
	static void PrintLoadingScreen();
	void PrintSveCScreen() const;
	void OyunKapanis() const;
	void SkoruKaydet();
	void GetNextQuestion();
	void CevabiGoster();
	void SoruyuOku(const unsigned short& no_of_letters);
	bool CevaplariKarsilastir(const std::string& guess) const;
	static char TR_toupper(const char& ch);
public:
	KelimeOyunu(const short& dakika = 4, const double& saniye = 0.0, const short& tahmin_m = 0, const double& tahmin_s = 30.0);
	void Basla();
};

// diger fonksiyonları tanımlama
void ImlecTasiXY(const short& x, const short& y);
template<class T> T GetRandomNumber(const T& min, const T& max);

#endif		//KELIME_OYUNU_H

