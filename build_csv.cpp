#include "build_csv.hpp"

void BuildCSV::printGenIdfCSV(const Aco *aco, ofstream &gen_file){
    gen_file << "Geracao";
    for(int i = 1; i <= aco->npop; ++i){
        gen_file << ",Individuo " << i;
    }
    gen_file << '\n';
}

void BuildCSV::printFacIdfCSV(ofstream &fac_file){
    fac_file << "Geracoes,";
    fac_file << "Tamanho da Populacao,";
    fac_file << "Q,";
    fac_file << "Alfa,";
    fac_file << "Beta,";
    fac_file << "p,";
    fac_file << "Melhor Aptidao,";
    fac_file << "Aptidao Media" << "\n";
}

void BuildCSV::printFacResultsCSV(Aco *aco, ofstream &fac_file){
    fac_file << aco->ngen << ',';
    fac_file << aco->npop << ',';
    fac_file << fixed << setprecision(2);
    fac_file << aco->Q << ',';
    fac_file << aco->alfa << ',';
    fac_file << aco->beta << ',';
    fac_file << 1 - aco->evaporation << ',';
    fac_file << aco->min_makespan << ',';
    fac_file << aco->sum_costs/(aco->npop * aco->ngen);
    fac_file << '\n';
}