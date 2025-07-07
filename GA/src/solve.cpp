#include "solve.hpp"
#include "ag.hpp"
#include "build_csv.hpp"
#include "instance.hpp"

#include <bits/stdc++.h>

using namespace std;

void Solve::solve(ifstream &input_file, const int crossover_id,
                  const int selection_id, ofstream &output_file){
    // Parâmetros do algoritmo
    const int npop = 1000;
    const int nelite = 10;
    const double pcrossover = 0.9;
    const double pwinner = 0.95;
    const double pmutation = 0.01;

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

    int ngen = instance.n * instance.m * 2;
    ngen = min(ngen, 3000);

    const int ngenes = instance.n;

    Ag *ag = new Ag(instance, npop, ngen, ngenes, nelite, pcrossover, pmutation, pwinner);

    ag->initPopulation();

    for(int generation = 0; generation < ag->ngen; ++generation){
        ag->evaluatePopulation();

        if(ag->count_gen_local_min > sqrt(ngen) * 5){ 
            if(ag->count_gen_local_min > sqrt(ngen) * 10) break; 
            
            ag->threeOpt(); 
        }        

        vector<int> parents = ag->parentsSelection(selection_id);

        ag->crossover(parents, crossover_id);

        ag->mutation();
        ag->elitism();

        ag->copyPopulation();
    }

    ag->printResults(output_file);

    delete ag;
}

void Solve::factorialTest(ifstream &input_file, ofstream &output_file,
                          const int crossover_id, const int selection_id){
    const int npop = 1000;
    const int vnelite[] = {2, 6, 10, 20};
    const double pcrossover = 0.9;
    const double vpwinner[] = {0.9, 1.0};
    const double vpmutation[] = {0.01, 0.05, 0.1};

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

    
    int ngen = instance.n * instance.m * 2;
    ngen = min(ngen, 3000);
    
    const int ngenes = instance.n;

    BuildCSV csv_builder;
    csv_builder.printFacIdfCSV(output_file);

    int iteration = 0;
    for (int ielite = 0; ielite < 3; ++ielite){
        for (int imutation = 0; imutation < 3; ++imutation){
            for (int ivpwinner = 0; ivpwinner < 2; ++ivpwinner){
                cout << "Iteracao: " << iteration << endl;
                solveFactorialTest(instance, npop, ngen, ngenes,
                                   vnelite[ielite], pcrossover,
                                   vpmutation[imutation], vpwinner[ivpwinner],
                                   output_file, crossover_id, selection_id);
                iteration++;
            }
        }
    }

    output_file.close();
}

void Solve::solveFactorialTest(Instance &instance, int npop, int ngen, int ngenes,
                               int nelite, double pcrossover, double pmutation,
                               double pwinner, ofstream &output_file,
                               const int crossover_id, const int selection_id){
    Ag *ag = new Ag(instance, npop, ngen, ngenes, nelite, pcrossover, pmutation,
                       pwinner);

    BuildCSV csv_builder;

    ag->initPopulation();
    for(int generation = 0; generation < ag->ngen; ++generation){
        ag->evaluatePopulation();

        if(ag->count_gen_local_min > sqrt(ngen) * 5){ 
            //if(ag->count_gen_local_min > sqrt(ngen) * 5 + sqrt(sqrt(ngen))) break; 
            
            ag->threeOpt(); 
        }        
  
        vector<int> parents = ag->parentsSelection(selection_id);
        ag->crossover(parents, crossover_id);

        ag->mutation();

        if(ag->count_gen_local_min > 150) break;

        ag->elitism();

        ag->copyPopulation();
    }

    csv_builder.printFacResultsCSV(ag, output_file);
    delete ag;
}

void Solve::generationsTest(ifstream &input_file, ofstream &output_file,
                            const int crossover_id, const int selection_id){
    const int npop = 200;
    const int nelite = 10;
    const double pcrossover = 0.9;
    const double pwinner = 0.95;
    const double pmutation = 0.01;

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

    int ngen = instance.n * instance.m * 2;
    ngen = min(ngen, 1000);

    const int ngenes = instance.n;

    solveGenerationsTest(instance, npop, ngen, ngenes, nelite, pcrossover, pmutation,
                         pwinner, output_file, crossover_id, selection_id);

    output_file.close();
}

void Solve::solveGenerationsTest(Instance &instance, int npop, int ngen, int ngenes,
                                 int nelite, double pcrossover, double pmutation,
                                 double pwinner, ofstream &output_file,
                                 const int crossover_id, const int selection_id){
    Ag *ag = new Ag(instance, npop, ngen, ngenes, nelite, pcrossover, pmutation,
                       pwinner);

    BuildCSV csv_builder;

    csv_builder.printGenIdfCSV(ag, output_file);
    ag->initPopulation();
    for(int generation = 0; generation < ag->ngen; ++generation){
        ag->evaluatePopulation();

        if(ag->count_gen_local_min > sqrt(ngen) * 5){
            //if(ag->count_gen_local_min > sqrt(ngen) * 5 + sqrt(sqrt(ngen))) break; 
            ag->threeOpt(); 
        }        

        vector<int> parents = ag->parentsSelection(selection_id);
        ag->crossover(parents, crossover_id);

        ag->mutation();
        ag->elitism();

        ag->copyPopulation();

        csv_builder.printGenResultsCSV(ag, output_file, generation);
    }
    delete ag;
}
