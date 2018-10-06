#include<iostream>
#include<fstream>
using namespace std;

/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

/*
u nastavku je prikazan primjer ispitnog zapisnika (izvještaj o održanom ispitu) sa detaljima/stavkama

Datum ispita: 07.09.2017
Predmet: Programiranje II
IZVJESTAJ O ODRZANOM ISPITU
Br indeksa Ime prezime Komisijsko*** Ocjena
16001	A. Joldic	DA	5
15251	D. Music	NE	7 Ponisteno
16014	E. Babovic	DA	8
UKUPNO POLOZILO:	2
PROSJECNA OCJENA:	6.67
*** komisijski ispit: ako student pristupa četvrti put i dalje (računajući i poništene rezultate)
*/

const char * crt = "\n-------------------------------------------\n";
struct Datum {
	int _dan;
	int _mjesec;
	int _godina;
	void Unos(int d, int m, int g) {
		_dan = d;
		_mjesec = m;
		_godina = g;
	}
	void Ispis() {
		cout << _dan << "." << _mjesec << "." << _godina << " ";
	}
};
struct Student {
	char _imePrezime[20];
	int _brojIndeksa;
	int _polozioECTS;

	void Unos(const char* naziv, float cijena) {
		strcpy_s(_imePrezime, naziv);
		_brojIndeksa = cijena;
		_polozioECTS = 0;
	}
	void DodajECTS(float ects) {
		_polozioECTS += ects;
	}
	void UmanjiECTS(float ects) {
		_polozioECTS -= ects;
	}
	void Ispis() {
		cout << _imePrezime << ", Indeks: " << _brojIndeksa << ", ECTS: " << _polozioECTS << " " << endl;
	}
};

struct ZapisnikStavka {
	int _ocjena;
	Student* _student; //ZapisnikStavka će pokazivati na objekat tipa student koji se nalazi u instanci tipa Fakultet, pa nije potrebno vršiti nove alokacije
	bool _ponisteno;
	bool _komisijskoPolaganje;

	void Unos(Student* proizvod, int ocjena, bool komisijskoPolaganje) {
		_ocjena = ocjena;
		_student = proizvod;
		_komisijskoPolaganje = komisijskoPolaganje;
		_ponisteno = false;
	}

	void Ispis() {
		cout << _student->_imePrezime << "\t" << _ocjena << "\t" << _student->_brojIndeksa << "\t";
		if (_ponisteno)
			cout << " Ponisteno";
		cout << endl;
	}
};
struct IspitniZapisnik {
	Datum _datum;
	ZapisnikStavka ** _stavke;//svaki put se dodaje po jedna stavka
	int _brojStavki;
	int _ects;
	char _predmet[20];

	void DodajStavku(Student *student, const int &ocjena)
	{
		ZapisnikStavka ** t = new ZapisnikStavka*[_brojStavki + 1];
		for (int i = 0; i < _brojStavki; i++)
		{
			t[i] = _stavke[i];
			_stavke[i] = nullptr;
		}
		delete[] _stavke;
		_stavke = t;

		_stavke[_brojStavki] = new ZapisnikStavka;
		_stavke[_brojStavki]->Unos(student, ocjena, false);

		_brojStavki++;
	}

	void Unos(Datum datum, const char* predmet, int ects) {
		_datum = datum;
		_brojStavki = 0;
		_stavke = nullptr;
		_ects = ects;
		strcpy_s(_predmet, predmet);
	}
	/*Prosjecnu ocjenu izračunati koristeći lambda funkciju koja se poziva koristeći objekat
	tipa auto. u prosjek računati i poništene rezultate (stavke)*/
	float GetProsjecnaOcjena() {
		return 0;
	}
};
const int Max = 100;

/*unutar objekta tipa Fakultet ce se cuvati podaci o svim studentima i ispitnim zapisnicima, a dodaju se pomocu globalnih funkcija*/
struct Fakultet {
	//umjesto brojaca koristiti default.ne vrijednosti
	Student* Studenti[Max] = { NULL };
	IspitniZapisnik* Zapisnici[Max] = { NULL };

	void Dealociraj() {
		for (size_t i = 0; i < Max; i++) {
			delete Studenti[i]; Studenti[i] = NULL;
		}
		for (size_t i = 0; i < Max; i++) {
			delete Zapisnici[i]; Zapisnici[i] = NULL;
		}
	}
};

/*dodaje podatke o novom studentu u fakultet */
//fakultet, imePrezime, broj indeksa
void dodaj_student(Fakultet &fakultet, const char*imePrezime, int brojIndeksa)
{
	for (int i = 0; i < Max; i++)
	{
		if (fakultet.Studenti[i] == nullptr)
		{
			fakultet.Studenti[i] = new Student;
			fakultet.Studenti[i]->Unos(imePrezime, brojIndeksa);
			return;
		}
		else if (fakultet.Studenti[i]->_brojIndeksa == brojIndeksa)
		{
			cout << "Student " << fakultet.Studenti[i]->_imePrezime << " vec postoji." << endl;
			return;
		}
	}
	cout << "Sva su mjesta popunjena!" << endl;
}

/*kraira novi zapisnik*/
//fakultet, datum, predmet, ects krediti
void kreiraj_zapisnik(Fakultet &fakultet, int dan, int mjesec, int godina, const char*nazivPredmeta, int ects)
{
	for (int i = 0; i < Max; i++)
	{
		if (fakultet.Zapisnici[i] == nullptr)
		{
			fakultet.Zapisnici[i] = new IspitniZapisnik;
			fakultet.Zapisnici[i]->Unos({ dan,mjesec,godina }, nazivPredmeta, ects);
			return;
		}
	}
	cout << "Sve je popunjeno, nije moguce kreirati novi zapisnik!" << endl;
}


/*dodaje stavku na posljednji kreiran zapisnik, a ects studenta uveća ako je student položio ispit. */

bool dodaj_stavku_na_zapisnik(Fakultet &fakultet, int brojIndeksa, int ocjena)
{
	for (int i = 0; i < Max; i++)
	{
		if (fakultet.Zapisnici[i] == nullptr)
		{
			i--;
			for (int j = 0; j < fakultet.Zapisnici[i]->_brojStavki; j++)
			{
				if (fakultet.Zapisnici[i]->_stavke[j]->_student->_brojIndeksa == brojIndeksa)
				{
					if (ocjena > 5 && fakultet.Zapisnici[i]->_stavke[j]->_ocjena == 5)
						fakultet.Zapisnici[i]->_stavke[j]->_student->DodajECTS(fakultet.Zapisnici[i]->_ects);
					fakultet.Zapisnici[i]->_stavke[j]->_ocjena = ocjena;
					return false;
				}
			}

			Student *t = nullptr;
			for (int x = 0; x < Max; x++)
			{
				if (fakultet.Studenti[x] != nullptr)
				{
					if (fakultet.Studenti[x]->_brojIndeksa == brojIndeksa)
					{
						t = fakultet.Studenti[x];
						break;
					}
				}
			}

			if (t == nullptr)
				return false;

			fakultet.Zapisnici[i]->DodajStavku(t, ocjena);
			fakultet.Zapisnici[i]->_stavke[fakultet.Zapisnici[i]->_brojStavki-1]->_student->DodajECTS(fakultet.Zapisnici[i]->_ects);
			return true;
		}
	}
}

/*u posljednjem zapisniku stavku za studenta 16001 ponistiti, a studentu umanjiti ects bodove.
funkcija vraca false u slucaju da se student ne nalazi na posljednjem zapisniku*/

bool ponisti_stavku_rezultat(const Fakultet &fakultet, int brojIndeksa)
{
	if (fakultet.Zapisnici[0] == nullptr)
		return false;

	int x = Max;
	for (int i = 0; i < Max; i++)
	{
		if (fakultet.Zapisnici[i] == nullptr)
		{
			x = i;
			break;
		}
	}
	x--;

	for (int j = 0; j < fakultet.Zapisnici[x]->_brojStavki; j++)
	{
		if (fakultet.Zapisnici[x]->_stavke[j]->_student->_brojIndeksa == brojIndeksa)
		{
			fakultet.Zapisnici[x]->_stavke[j]->_ponisteno = true;
			fakultet.Zapisnici[x]->_stavke[j]->_student->UmanjiECTS(fakultet.Zapisnici[x]->_ects);
			return true;
		}
	}
	return false;


}

/* funkcija printaj prikazuje posljednji ispitni zapisnik u formatu prikazanom na pocetku zadatka.
* pored prikazivanja, identican sadrzaj ispitnog zapisnika je potrebno je pasiti u tekstualni fajl.
* naziv fajla treba biti Predmet_datum.txt, npr. ProgramiranjeII_2017-09-01.txt. zapisnik se nece
prikazati niti sacuvati u fajl ukoliko ne posjeduje niti jednu stavku ili su sve stavke ponistene*/
bool printaj(Fakultet &fakultet)
{
	if (fakultet.Zapisnici[0] == nullptr)
		return false;

	int x = Max;
	for (int i = 0; i < Max; i++)
	{
		if (fakultet.Zapisnici[i] == nullptr)
		{
			x = i;
			break;
		}
	}
	x--;

	int vel = strlen(fakultet.Zapisnici[x]->_predmet) + 1;
	char dan[3], mjesec[3], godina[5];

	_itoa_s(fakultet.Zapisnici[x]->_datum._dan, dan, 10);
	_itoa_s(fakultet.Zapisnici[x]->_datum._mjesec, mjesec, 10);
	_itoa_s(fakultet.Zapisnici[x]->_datum._godina, godina, 10);

	vel += strlen(dan);
	vel += strlen(mjesec);
	vel += strlen(godina);
	vel += 7;

	char *naziv = new char[vel];
	strcpy_s(naziv, vel, fakultet.Zapisnici[x]->_predmet);
	strcat_s(naziv, vel, "_");
	strcat_s(naziv, vel, godina);
	strcat_s(naziv, vel, "-");
	strcat_s(naziv, vel, mjesec);
	strcat_s(naziv, vel, "-");
	strcat_s(naziv, vel, dan);
	strcat_s(naziv, vel, ".txt");
	
	ofstream upis(naziv);

	upis << fakultet.Zapisnici[x]->_predmet << endl;
	upis << fakultet.Zapisnici[x]->_datum._dan << "." << fakultet.Zapisnici[x]->_datum._mjesec << "." << fakultet.Zapisnici[x]->_datum._godina << " -> " << fakultet.Zapisnici[x]->_ects << crt;
	for (int i = 0; i < fakultet.Zapisnici[x]->_brojStavki; i++)
	{
		upis << fakultet.Zapisnici[x]->_stavke[i]->_student->_imePrezime << ", Indeks: " << fakultet.Zapisnici[x]->_stavke[i]->_student->_brojIndeksa << ", ECTS: " << fakultet.Zapisnici[x]->_stavke[i]->_student->_polozioECTS << " " << endl;
		upis << "Ocjena:" << fakultet.Zapisnici[x]->_stavke[i]->_ocjena;
		if (fakultet.Zapisnici[x]->_stavke[i]->_ponisteno)
			upis << " -> PONISTENA";
		upis << endl;
		if (fakultet.Zapisnici[x]->_stavke[i]->_komisijskoPolaganje)
			upis << "KOMISIJSKO POLAGANJE!" << endl;
	}
	upis.close();

	ifstream ispis(naziv);
	char znak;
	while (ispis.get(znak))
		cout << znak;
	ispis.close();

	delete[] naziv;
	naziv = nullptr;

	return true;
}


void main() {
	Fakultet fakultet;

	/*dodaje podatke o novom studentu u fakultet */
	//fakultet, imePrezime, broj indeksa
	dodaj_student(fakultet, "A. Joldic", 16001);
	dodaj_student(fakultet, "D. Music", 15251);
	dodaj_student(fakultet, "E. Babovic", 16014);
	dodaj_student(fakultet, "E. Junuz", 16015);
	dodaj_student(fakultet, "J. Azemovic", 16014);//ne dodaje studenta jer je broj indeksa zauzet
	
	/*kraira novi zapisnik*/
	//fakultet, datum, predmet, ects krediti
	kreiraj_zapisnik(fakultet, 6, 9, 2017, "Programiranje 2", 7);

	/*dodaje stavku na posljednji kreiran zapisnik, a ects studenta uveća ako je student položio ispit. */

	if (dodaj_stavku_na_zapisnik(fakultet, 16001, 5))
		cout << "Stavka (rezultat) uspjesno dodata!" << endl;

	/*ako je student vec dodat u zapisnik, onda se vrsi samo modifikacija njegove ocjene*/
	if (!dodaj_stavku_na_zapisnik(fakultet, 16001, 6))
		cout << "Student vec dodat na zapisnik. Ocjena je ispravljena!" << endl;

	dodaj_stavku_na_zapisnik(fakultet, 15251, 5);
	dodaj_stavku_na_zapisnik(fakultet, 16015, 6);
	dodaj_stavku_na_zapisnik(fakultet, 16014, 8);

	kreiraj_zapisnik(fakultet, 7, 9, 2017, "RacunarskeMreze", 5);
	//naredne stavke se dodaju na novi zapisnik
	dodaj_stavku_na_zapisnik(fakultet, 16015, 5);
	dodaj_stavku_na_zapisnik(fakultet, 16014, 10);


	/*u posljednjem zapisniku stavku za studenta 16001 ponistiti, a studentu umanjiti ects bodove.
	funkcija vraca false u slucaju da se student ne nalazi na posljednjem zapisniku*/

	if (ponisti_stavku_rezultat(fakultet, 16014))
		cout << "Stavka (rezultat) uspjesno ponisten!" << endl;

	if (!ponisti_stavku_rezultat(fakultet, 19001))
		cout << "Student nije pronadjen!" << endl;

	/* funkcija printaj prikazuje posljednji ispitni zapisnik u formatu prikazanom na pocetku zadatka.
	* pored prikazivanja, identican sadrzaj ispitnog zapisnika je potrebno je pasiti u tekstualni fajl.
	* naziv fajla treba biti Predmet_datum.txt, npr. ProgramiranjeII_2017-09-01.txt. zapisnik se nece
	prikazati niti sacuvati u fajl ukoliko ne posjeduje niti jednu stavku ili su sve stavke ponistene*/
	if (printaj(fakultet))
		cout << "Ispitni zapisnik uspjesno prikazan i spasen u fajl!" << endl;
	fakultet.Dealociraj();
	system("PAUSE");
}