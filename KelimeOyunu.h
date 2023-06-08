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

