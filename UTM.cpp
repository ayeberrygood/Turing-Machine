#include <Windows.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

struct taisykle
{
  string busenaDb;
  char simbolisDb;
  char simbolisDb2;
  char naujasSimbolis;
  char naujasSimbolis2;
  char kryptis;
  char kryptis2;
  string naujaBusena;
};

// Dokumento pavadinimas yra paduodamas kaip argumentas
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    cout << "Per mazai argumentu...";
    return 0;
  }

  string pav = argv[1];
  ifstream ivestis(pav);

  int juostuSk;
  ivestis >> juostuSk;

  string juosta;
  string juosta2;
  ivestis >> juosta;
  if (juostuSk == 2)
    ivestis >> juosta2;

  int poz;
  int poz2;
  ivestis >> poz;
  if (juostuSk == 2)
    ivestis >> poz2;

  vector<taisykle> TMrules;
  taisykle temp;

  // duomenu nuskaitymas is failo
  while (!ivestis.eof())
  {
    ivestis >> temp.busenaDb;

    ivestis >> temp.simbolisDb;

    if (juostuSk == 2)
      ivestis >> temp.simbolisDb2;
    ivestis >> temp.naujasSimbolis;
    if (juostuSk == 2)
      ivestis >> temp.naujasSimbolis2;
    ivestis >> temp.kryptis;
    if (juostuSk == 2)
      ivestis >> temp.kryptis2;
    ivestis >> temp.naujaBusena;
    TMrules.push_back(temp);
  }
  ivestis.close();

  string busenosSkaitliukas = "0"; // nuo kurio pradedam
  bool baigta = false;
  int p = poz - 1;
  int p2 = poz2 - 1;

  // cout << TMrules.size() << endl;
  cout << "\033[2J";

  while (!baigta)
  {
    bool rasta = false; // busenai ir taisyklei
    bool rastaBusena = false;

    string tarpaiPries(p, ' ');
    string tarpaiPo(juosta.size() - p, ' ');

    cout << "\033[H\033[2K" << juosta << " | busena: " << busenosSkaitliukas
         << "; taisykle simbolio: " << juosta[p] << endl
         << tarpaiPries << "^" << tarpaiPo << flush;

    if (juostuSk == 2)
    {
      string tarpaiPries2(p2, ' ');
      string tarpaiPo2(juosta.size() - p2, ' ');

      cout << endl
           << endl
           << juosta2 << " | busena: " << busenosSkaitliukas
           << "; taisykle simbolio: " << juosta2[p2] << endl
           << tarpaiPries2 << "^" << tarpaiPo2 << flush;
    }

    // stabdymas
    if (GetKeyState('P') & 0x8000 || GetKeyState('p') & 0x8000)
    {
      cout << endl;
      system("pause");
    }
    // sustojiams
    if (GetKeyState('B') & 0x8000 || GetKeyState('b') & 0x8000)
    {
      cout << endl
           << "Programa buvo nutraukta." << endl;
      return 0;
    }

    this_thread::sleep_for(chrono::milliseconds(1));

    for (int i = 0; i < TMrules.size(); i++)
    {
      if (TMrules[i].busenaDb != busenosSkaitliukas)
        continue;

      rastaBusena = true;
      // vienai juostai
      if (juostuSk == 1 && juosta[p] == TMrules[i].simbolisDb)
      {
        juosta[p] = TMrules[i].naujasSimbolis;
        busenosSkaitliukas = TMrules[i].naujaBusena; // pakeiciam skaitliuka cia tik jei
                                                     // viena juosta, kitaip veliau

        if (TMrules[i].kryptis == 'R')
        {
          p++;
          if (p >= juosta.length())
          {
            // pletimasis i desine
            juosta.push_back('$');
            // p--;
          }
        }
        else
        {
          p--;
          if (p < 0)
          {
            juosta.insert(juosta.begin(), '$');
            p++;
            // pletimasis i kaire
          }
        }
        rasta = true;
        break;
      }
      // dviems juosta
      if (juostuSk == 2 && juosta[p] == TMrules[i].simbolisDb &&
          juosta2[p2] == TMrules[i].simbolisDb2)
      {
        juosta[p] = TMrules[i].naujasSimbolis;
        busenosSkaitliukas = TMrules[i].naujaBusena; // pakeiciam skaitliuka cia tik jei
                                                     // viena juosta, kitaip veliau

        if (TMrules[i].kryptis == 'R')
        {
          p++;
          if (p >= juosta.length())
          {
            // pletimasis i desine
            juosta.push_back('$');
            // p--;
          }
        }
        else
        {
          p--;
          if (p < 0)
          {
            juosta.insert(juosta.begin(), '$');
            p++;
            // pletimasis i kaire
          }
        }

        juosta2[p2] = TMrules[i].naujasSimbolis2;
        // busenosSkaitliukas = TMrules[i].naujaBusena; // jei dvi juostos tik cia
        // pakeiciam busena
        if (TMrules[i].kryptis2 == 'R')
        {
          p2++;
          if (p2 >= juosta2.length())
          {
            // pletimasis i desine
            juosta2.push_back('$');
            // p--;
          }
        }
        else
        {
          p2--;
          if (p2 < 0)
          {
            juosta2.insert(juosta2.begin(), '$');
            p2++;
            // pletimasis i kaire
          }
        }
        rasta = true;
        break;
      }
    }

    if (!rastaBusena)
    {
      cout << endl
           << "Baigta.";
      return 0;
    }
    if (!rasta)
    {
      cout << endl
           << "Taisykle nerasta." << endl;
      return 0;
    }
    if (juosta.length() > 150 || juosta2.length() > 150)
    {
      cout << "Juosta tesis amzinai.";
      return 0;
    }
  }
}