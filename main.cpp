#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>

#include "resitev.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

vector<double> razcleniVrstico(const string &vrstica)
{
    vector<double> verjetnosti;
    std::istringstream iss(vrstica);
    double p;
    while (iss >> p)
    {
        verjetnosti.push_back(p);
    }
    return verjetnosti;
}

vector<vector<double>> preberiVrstice(const string &pot)
{
    vector<vector<double>> vrstice;

    std::fstream datoteka;
    datoteka.open(pot, std::ios::in);
    if (datoteka.is_open())
    {
        string vrstica;
        while (getline(datoteka, vrstica))
        {
            vrstice.push_back(razcleniVrstico(vrstica));
        }
        datoteka.close();
    }
    else
    {
        cout << "Nisem mogel prebrati datoteke " << pot << "\nLep pozdrav, tvoj c++" << endl;
    }
    return vrstice;
}

bool preveriPravilnostEne(int i, double eps, const vector<double> &resitev, const vector<double> &odgovor, long long trajanje, long long maxTrajanje)
{
    bool dovoljHitro = trajanje <= maxTrajanje;
        string zacetek = dovoljHitro ? " sem" : "a nisem";       
        cout << "   - odgovor" << zacetek << " dobil dovolj hitro (po " << trajanje << " ms,"
             << " dovoljeno: " << maxTrajanje << " ms); " << endl;
    int k0 = -1;
    if (resitev.size() != odgovor.size())
    {
        cout << "   - odgovor ni pravilen, saj se dolzina resitve (" << resitev.size() 
             << ") in odgovora (" << odgovor.size() << ") ne ujemata." << endl;
        return false;
    }
    for (int k = 0; k < resitev.size(); k++)
    {
        if (std::fabs(odgovor[k] - resitev[k]) > eps)
        {
            k0 = k;
            break;
        }
    }
    if (k0 >= 0)
    {
        cout << "   - odgovor ni pravilen. Prva napaka se zgodi pri verjetnosti za k = " 
             << k0 << ". Pravilen odgovor = " << resitev[k0] << " != " << odgovor[k0] << endl;
        return false;
    }
    cout << "   - odgovor je pravilen." << endl;
    return dovoljHitro;
}

long long izberiCas(vector<long long> &maxTrajanjaMS, int velikost)
{
    vector<int> mejeVelikosti{2048, 65536, 131072};
    int iTrajanje = 0;
    while (iTrajanje < mejeVelikosti.size() && velikost > mejeVelikosti[iTrajanje])
    {
        iTrajanje++;
    }
    if (iTrajanje >= mejeVelikosti.size())
    {
        throw "Nepravilna velikost problema.";
    }
    return maxTrajanjaMS[iTrajanje];
}

void preveri(const string &naloga, vector<long long> maxTrajanjaMS)
{
    cout << "Preverjam " << naloga << endl;
    auto podatki = preberiVrstice(naloga + ".in");
    auto resitve = preberiVrstice(naloga + ".out");
    int iPoizvedba = 0;
    double eps = 1e-10;
    for (const auto &verjetnosti : podatki)
    {
        cout << "Poizvedba " << iPoizvedba << ":" <<endl;
        long long maxTrajanjeMS = izberiCas(maxTrajanjaMS, verjetnosti.size());
        auto t0 = std::chrono::high_resolution_clock::now();
        auto odgovor = Resitev::izracunajVerjetnosti(verjetnosti);
        auto t1 = std::chrono::high_resolution_clock::now();
        auto trajanje = (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)).count() / 1000;
        auto jeOK = preveriPravilnostEne(iPoizvedba, eps, resitve[iPoizvedba], odgovor, trajanje, maxTrajanjeMS);
        if (!jeOK)
        {
            cout << "Prekinjam preverjanje" << endl;
            break;
        }
        iPoizvedba++;
    }
}

long long enoStopanje(int meja)
{
    auto t0 = std::chrono::high_resolution_clock::now();
    int vsota = 0;
    for (int i = 0; i < meja; i++)
    {
        for (int j = i; j >= -i; j--)
        {
            vsota += (i + j) / 2;
        }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    auto trajanje = (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)).count() / 1000;
    cout << "Casovna meja: " << trajanje << endl;
    return trajanje;
}

vector<long long> izracunajMaxT()
{
    cout << "Poganjam for-zanke za maksimalna dovoljena trajanja. To zna nekoliko trajati. "
         << "Med razvojem lahko nadomestite klice funkcije enoStopanje z ustreznimi konstantami, "
         << "da ne bo treba ob vsakem zagonu izgubiti dobre minute ..." << endl;
    vector<long long> casi(3);
    casi[0] = 1000; // kratke
    casi[1] = 14933; // --> casi[1] = 20000; // enoStopanje(87000) 
    casi[2] = 33377;
    return casi;
}

int main()
{
    auto maxT = izracunajMaxT();
    vector<string> naloge = {"nalogaA0", "nalogaA1"};
    for (auto const &naloga : naloge)
    {
        preveri(naloga, maxT);
        cout << endl;
    }
    return 0;
}