#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include <vector>

using std::vector;


class Resitev{

public:
    /*
    Metoda sprejme vektor verjetnosti p_i in vrne vektor verjetnosti P_k,
    ki je za ena daljši od vhodnega vektorja.
    */
    static vector<double> izracunajVerjetnosti(const vector<double> & verjetnosti);
    static vector<double> deli_in_vladaj(vector<double> mn_verj);
};