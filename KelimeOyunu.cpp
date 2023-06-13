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
// bu fonksiyonla harf ortaya cıkar (Harf alayım)
// harf alınca 100 puan eksilir
void KelimeOyunu::HarfAl() {
	unsigned short letter_index;
	do {
		letter_index = GetRandomNumber<unsigned short>(0, current_word.length() - 1);
	} while (word_on_display[letter_index] != '_');
	for (unsigned short i = 0; i < 30; i++) {		//harfi göstermeden önce 30 random harfini hızlıca göster
		word_on_display[letter_index] = GetRandomNumber<char>('A', 'Z');
		PrintSveCScreen();
		Sleep(20);
	}
	word_on_display[letter_index] = current_word[letter_index];
	PrintSveCScreen();
	if ((--no_of_undisplayed_letters) <= 0) {
		Sleep(1000);
		current_time.IncreaseZaman(0, 1.1);
		GetNextQuestion();
		return;
	}
	current_time.IncreaseZaman(0, 0.6);
}

// zamanı durdur (Tahmin), oyuncuya düşünmesi için zaman verir
// hancak bunu yaparak, oyuncu artık harf isteyemez ve açıklanmayan harf sayısı kadar puan kaybeder.
void KelimeOyunu::ZamaniDurdur(clock_t& before1, clock_t& after1) {
	tahmin.SetZaman(TAHMIN_SURESI / 60, TAHMIN_SURESI % 60);
	ImlecTasiXY(16, 19);
	std::cout << tahmin.GetDakika() << ":" << std::setfill('0') << std::setw(2) << short(tahmin.GetSaniye()) << std::endl;
	std::cout << "\t\tTahmininizi giriniz: ";
	clock_t begin = clock(), before, after = clock();
	do {
		tahmin.UpdateZamanDisplay(begin, after, 16, 19);
		if (_kbhit()) {
			ImlecTasiXY(37, 20);
			std::string guess;
			getline(std::cin, guess);
			if (CevaplariKarsilastir(guess)) {
				ZamaniDurdurUtil(before, after, before1, after1, true);
				return;
			}
			else {
				ImlecTasiXY(37, 20);
				std::cout << std::string(guess.length(), ' ');
				ImlecTasiXY(37, 20);
			}
		}
		before = after;
		after = clock();
	} while (tahmin.UpdateZaman(before, after));
	ZamaniDurdurUtil(before, after, before1, after1, false);
}
// "ZamaniDurdur" fonksiyonu, yanıtı görüntüler, oyuncunun puanını günceller ve bir sonraki soruyu alır
void KelimeOyunu::ZamaniDurdurUtil(clock_t& before, clock_t& after, clock_t& before1, clock_t& after1, const bool& guessed_correct) {
	CevabiGoster();
	ImlecTasiXY(46, 14);
	std::cout << std::string(std::to_string(score).length(), ' ');
	ImlecTasiXY(46, 14);
	std::cout << (score += (guessed_correct ? 1 : -1) * no_of_undisplayed_letters * HARF_BASI_PUAN);
	Sleep(1000);
	before = after;
	after = clock();
	tahmin.UpdateZaman(before, after);
	short dakika = TAHMIN_SURESI / 60 - tahmin.GetDakika();
	double saniye = double(TAHMIN_SURESI % 60) - tahmin.GetSaniye();
	if (saniye < 0.0) {
		saniye += 60.0;
		dakika--;
	}
	current_time.IncreaseZaman(dakika, saniye);
	before1 = after1;
	after1 = clock();
	current_time.UpdateZaman(before1, after1);
	GetNextQuestion();
}

// oyunun nasıl oynanacağına dair kuralları ve talimatları verir
void KelimeOyunu::PrintOpeningScreen() {
	std::cout << " .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------." << std::endl;
	std::cout << "| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |" << std::endl;
	std::cout << "| |  ___  ____   | || |  _________   | || |   _____      | || |     __O__    | || | ____    ____ | || |  _________   | |" << std::endl;
	std::cout << "| | |_  ||_  _|  | || | |_   ___  |  | || |  |_   _|     | || |    |_   _|   | || ||_   \\  /   _|| || | |_   ___  |  | |" << std::endl;
	std::cout << "| |   | |_/ /    | || |   | |_  \\_|  | || |    | |       | || |      | |     | || |  |   \\/   |  | || |   | |_  \\_|  | |" << std::endl;
	std::cout << "| |   |  __'.    | || |   |  _|  _   | || |    | |   _   | || |      | |     | || |  | |\\  /| |  | || |   |  _|  _   | |" << std::endl;
	std::cout << "| |  _| |  \\ \\_  | || |  _| |___/ |  | || |   _| |__/ |  | || |     _| |_    | || | _| |_\\/_| |_ | || |  _| |___/ |  | |" << std::endl;
	std::cout << "| | |____||____| | || | |_________|  | || |  |________|  | || |    |_____|   | || ||_____||_____|| || | |_________|  | |" << std::endl;
	std::cout << "| |              | || |              | || |              | || |              | || |              | || |              | |" << std::endl;
	std::cout << "| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |" << std::endl;
	std::cout << " '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'" << std::endl;
	std::cout << "          .----------------.  .----------------.  .----------------.  .-----------------. .----------------. " << std::endl;
	std::cout << "         | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |" << std::endl;
	std::cout << "         | |     ____     | || |  ____  ____  | || | _____  _____ | || | ____  _____  | || | _____  _____ | |" << std::endl;
	std::cout << "         | |   .'    `.   | || | |_  _||_  _| | || ||_   _||_   _|| || ||_   \\|_   _| | || ||_   _||_   _|| |" << std::endl;
	std::cout << "         | |  /  .--.  \\  | || |   \\ \\  / /   | || |  | |    | |  | || |  |   \\ | |   | || |  | |    | |  | |" << std::endl;
	std::cout << "         | |  | |    | |  | || |    \\ \\/ /    | || |  | '    ' |  | || |  | |\\ \\| |   | || |  | '    ' |  | |" << std::endl;
	std::cout << "         | |  \\  `--'  /  | || |    _|  |_    | || |   \\ `--' /   | || | _| |_\\   |_  | || |   \\ `--' /   | |" << std::endl;
	std::cout << "         | |   `.____.'   | || |   |______|   | || |    `.__.'    | || ||_____|\\____| | || |    `.__.'    | |" << std::endl;
	std::cout << "         | |              | || |              | || |              | || |              | || |              | |" << std::endl;
	std::cout << "         | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |" << std::endl;
	std::cout << "          '----------------'  '----------------'  '----------------'  '----------------'  '----------------' " << std::endl;
	std::cout << "\n\n\n\nKelime Oyunu'na hos geldiniz!";
	std::cout << "\n\n\nKurallar:";
	std::cout << "\n\n-> " << HARF_BASI_SORU_S << "'er taneden ";
	const unsigned short soru_no = HARF_BASI_SORU_S * (MAX_HARF_SAYISI - MIN_HARF_SAYISI + 1);
	unsigned short toplam_harf = 0;
	for (unsigned short i = MIN_HARF_SAYISI; i <= MAX_HARF_SAYISI; i++) {
		toplam_harf += i * HARF_BASI_SORU_S;
		std::cout << i << (i == MAX_HARF_SAYISI - 1 ? " ve " : ", ");
	}
	std::cout << "\b\b harfli; toplam " << soru_no << " kelime ya da kelime grubunu verilen tanimlara gore ";
	if (OYUN_SURESI / 60 > 0)
		std::cout << OYUN_SURESI / 60 << " dakika ";
	if (OYUN_SURESI % 60 > 0)
		std::cout << OYUN_SURESI % 60 << " saniye ";
	std::cout << "icersinde bilmeye calisacaksiniz.";
	std::cout << "\n\n-> Bu " << soru_no << " kelimeden (" << toplam_harf << " harf) bildiginiz her harf icin " << HARF_BASI_PUAN << " puan kazanacaksiniz.";
	std::cout << "\n\n-> Yardim icin '" << HARF_ALAYIM << "' tusuna basarak harf alabilir ama aldiginiz harflerden puan alamazsiniz, sureyi '";
	std::cout << TAHMIN << "' tusuna (Tahmin) basarak durdurabilir ama durdurduktan sonra harf alamazsiniz.";
	std::cout << "\n\n-> Sureyi durdurduktan itibaren kelimeyi bilmeniz icin";
	if (TAHMIN_SURESI / 60 > 0)
		std::cout << " " << TAHMIN_SURESI / 60 << " dakika";
	TAHMIN_SURESI % 60 > 0 ? std::cout << " " << TAHMIN_SURESI % 60 << " saniyeniz" : std::cout << "niz";
	std::cout << " olacak, bu sure icinde kelimeyi bilemezseniz bilemediginiz her harf icin " << HARF_BASI_PUAN << " puan kaybedeceksiniz.";
	std::cout << "\n\n-> Oyunu istediginiz zaman durdurmak icin '" << DURDURMA_TUSU << "' tusuna, cikmak icin de 'Escape' tusuna basabilirsiniz.";
	std::cout << "\n\n\Dilediginiz herhangi bir tusuna basarak oyunu baslatabilirsiniz.";
	if (_getch() == CIKIS_TUSU)
		exit(0);
	system("cls");
}


void KelimeOyunu::PrintLoadingScreen() {
	ImlecTasiXY(40, 10);
	std::cout << "Yukleniyor...";
	ImlecTasiXY(34, 11);
	for (unsigned short i = 0; i < 20; i++) {
		Sleep(50);
		std::cout << char(182);
	}
	system("cls");
}
void KelimeOyunu::PrintLoadingScreen() {
	ImlecTasiXY(40, 10);
	std::cout << "Yukleniyor...";
	ImlecTasiXY(34, 11);
	for (unsigned short i = 0; i < 20; i++) {
		Sleep(50);
		std::cout << char(182);
	}
	system("cls");
}

// soruyu, gizli yanıtı ve ayrıca puanı yazdırır
void KelimeOyunu::PrintSveCScreen() const {
	ImlecTasiXY(0, 0);
	std::cout << "\n\n\t\t************************************";
	std::cout << "\n\n\t\t....................................\n\n\n\t\t\t";
	for (const auto& letter : word_on_display)
		std::cout << letter << " ";
	std::cout << "\n\n\n\t\t....................................";
	std::cout << "\n\n\t\t************************************\n\n\t\t";
	std::cout << current_time.GetDakika() << ":" << std::setfill('0') << std::setw(2) << short(current_time.GetSaniye());
	std::cout << "\t\t\tSkor: " << score << "\n\n\t\t" << current_soru;
}

// oyun sonu ekranını yaz
void KelimeOyunu::OyunKapanis() const {
	system("cls");
	ImlecTasiXY(40, 10);
	std::cout << "Nihai skorunuz: " << score << std::endl;
	std::cout << "\t\t\t\t\Artirdiginiz sure: " << current_time.GetDakika() << " dakika " << short(current_time.GetSaniye()) << " saniye";
	if (_getch() == CIKIS_TUSU)
		exit(0);
	system("cls");
}

// oyuncu skorunu kaydeder
void KelimeOyunu::SkoruKaydet() {
	time_t currentTime = time(nullptr);
	char tarih[26];
	ctime_s(tarih, sizeof(tarih), &currentTime);
	const std::string dosya_adi = "rekorlar.txt";
	std::string oyuncu_ismi;
	ImlecTasiXY(40, 10);
	std::cout << "Isminizi giriniz: ";
	getline(std::cin, oyuncu_ismi);
	std::ofstream myfile(dosya_adi, std::ios::app);
	myfile << "Oyuncu ismi: " << oyuncu_ismi << std::string(35 - oyuncu_ismi.length(), ' ');
	std::cout << "Oyun tarihi: " << tarih << std::endl;
	myfile << "Skor: " << score << std::string(42 - std::to_string(score).length(), ' ');
	myfile << "Artirilan sure: " << current_time.GetDakika() << ":" << std::setfill('0') << std::setw(2) << short(current_time.GetSaniye()) << std::endl;
	myfile << "__________________________________________________" << std::endl;
	myfile.close();
	system("cls");
	ImlecTasiXY(32, 10);
	std::cout << "Eski kayitlari gormek icin '" << KAYIT_GOR << "' tusuna basabilirsiniz." << std::endl;
	std::cout << "\t\t\t\tTekrar oynamak icin '" << YENİDEN_BASLAT << "' tusuna basabilirsiniz." << std::endl;
	std::cout << "\t\t\t\tProgramdan cikmak icin ise dilediginiz herhangi bir tusa basabilirsiniz.";
	switch (toupper(_getch())) {
	case KAYIT_GOR:
		try {
			std::ifstream myfile(dosya_adi);
			system("cls");
			if (myfile.is_open()) {
				std::cout << myfile.rdbuf();
				myfile.close();
				exit(0);
			}
			throw;
		}
		catch (...) {
			std::cerr << "!ERROR!\t\tUnable to open \"" << dosya_adi << "\".\t\t!ERROR!" << std::endl;
			exit(1);
		}
	case YENİDEN_BASLAT:
		while (!SveC.empty())
			SveC.pop();
		score = 0;
		current_time.SetZaman(OYUN_SURESI / 60, OYUN_SURESI % 60);
		system("cls");
		Basla();
	}
}
