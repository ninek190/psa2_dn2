#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include<complex>

#include "resitev.h"

vector<double> Resitev::izracunajVerjetnosti(const vector<double> &verjetnosti)
{    
    int dolzina_mn = verjetnosti.size();
    vector<double> Pk (dolzina_mn + 1);
    std::fill(Pk.begin(), Pk.end(), 1.0);

    vector<double> verj_prejsnja (dolzina_mn + 1);
    std::fill(verj_prejsnja.begin(), verj_prejsnja.end(), 1);

    for (int i = 0; i < dolzina_mn; i++){

        double p = verjetnosti[i];
        double q = 1 - verjetnosti[i];

        
        Pk[0] = verj_prejsnja[0] * q;
        Pk[dolzina_mn] = verj_prejsnja[dolzina_mn] * p;

        for (int j = 1; j < i + 1; j++){

            if(j == i){
                Pk[j] = verj_prejsnja[j-1] * p + verj_prejsnja[dolzina_mn] * q;
            } else{
                Pk[j] = verj_prejsnja[j-1] * p + verj_prejsnja[j] * q;
            }
        }
        verj_prejsnja = Pk;
    }
            

    return Pk;
}
/*
vector<double> Resitev::deli_in_vladaj(vector<double> mn_verj){

    int dolzina_mn = mn_verj.size();
    vector<double> verj (dolzina_mn + 1);
    std::fill(verj.begin(), verj.end(), 1);
    vector<double> verj_prejsnja (dolzina_mn + 1);
    std::fill(verj_prejsnja.begin(), verj_prejsnja.end(), 1);

    for (int i = 0; i < dolzina_mn + 1; i++){

        float p = mn_verj[i];
        float q = 1 - mn_verj[i];

        
        verj[0] = verj_prejsnja[0] * q;
        verj[-1] = verj_prejsnja[-1] * p;

        for (int j = 1; j < i + 1; j++){

            if(j == i){
                verj[j] = verj_prejsnja[j-1] * p + verj_prejsnja[-1] * q;
            } else{
                verj[j] = verj_prejsnja[j-1] * p + verj_prejsnja[j] * q;
            }
        }
        verj_prejsnja = verj;
    }

    return verj;
}*/