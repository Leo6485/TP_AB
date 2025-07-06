#include <bits/stdc++.h>

#include "build_csv.hpp"
#include "ag.hpp"

using namespace std;

void BuildCSV::printGenIdfCSV(Ag *ag, ofstream &gen_file){
    gen_file << "Geracao";
    for(int i = 1; i <= ag->npop; ++i)
        gen_file << ",Individuo " << i;
    gen_file << "\n";
}

void BuildCSV::printFacIdfCSV(ofstream &fac_file){
    fac_file << "Probabilidade de Cruzamento,";
    fac_file << "Elites,";
    fac_file << "Probabilidade de Mutacao,";
    fac_file << "Geracoes,";
    fac_file << "Tamanho da Populacao,";
    fac_file << "Melhor Aptidao,";
    fac_file << "Aptidao Media,";
    fac_file << "Desvio Padrao" << '\n';
}

void BuildCSV::printGenResultsCSV(Ag *ag, ofstream &gen_file, const int gen){
    gen_file << gen;
    for(int i = 0; i < ag->npop; ++i){
        gen_file << ',' << ag->fitness[i];
    }
    gen_file << '\n';
}

void BuildCSV::printFacResultsCSV(Ag *ag, ofstream &fac_file){
    fac_file << fixed << setprecision(2) << ag->pcrossover << ',';
    fac_file << ag->nelite << ',';
    fac_file << fixed << setprecision(2) << ag->pmutation << ',';
    fac_file << ag->ngen << ',';
    fac_file << ag->npop << ',';
    fac_file << *min_element(ag->fitness.begin(), ag->fitness.end()) << ',';

    double average_fit = accumulate(ag->fitness.begin(), ag->fitness.end(), 0.0);
    average_fit /= ag->npop;

    double dp = 0.0;
    for(int fit : ag->fitness){
        dp += pow(fit - average_fit, 2);
    }
    dp = sqrt(dp/ag->npop);

    fac_file << fixed << setprecision(6) << average_fit << ',';
    fac_file << fixed << setprecision(6) << dp << '\n';
}
