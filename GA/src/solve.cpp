#include "solve.hpp"
#include "ag.hpp"
#include "build_csv.hpp"
#include "instance.hpp"

#include <bits/stdc++.h>

using namespace std;

void Solve::solve(ifstream &input_file, const int crossover_id,
                  const int selection_id, ofstream &output_file){
    // Parâmetros do algoritmo
    const int npop = 600;
    const int nelite = 4;
    const double pcrossover = 1.0;
    const double pwinner = 0.95;
    const double pmutation = 0.01;

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

    int ngen = instance.n * instance.m * 2;

    const int ngenes = instance.n;

    Ag *ag = new Ag(instance, npop, ngen, ngenes, nelite, pcrossover, pmutation, pwinner);

    ag->initPopulation();

    for(int generation = 0; generation < ag->ngen; ++generation){
        ag->evaluatePopulation();
/*
        if(ag->count_gen_local_min > sqrt(ngen) * 5){
        
            
        }        
*/
        if(ag->count_gen_local_min > sqrt(ngen) * 5 + sqrt(sqrt(ngen))) break; 

        ag->twoOpt();
        ag->threeOpt(); 
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
    const int npop = 800;
    const int vnelite[] = {2, 6, 10, 20, 40};
    const double vpcrossover[] = {1.0, 0.9};
    const double pwinner = 0.9;
    const double vpmutation[] = {0.01, 0.1, 0.2, 0.3};

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

    int ngen = instance.n * instance.m * 2;

    const int ngenes = instance.n;

    BuildCSV csv_builder;
    csv_builder.printFacIdfCSV(output_file);

    for (int ielite = 0; ielite < 5; ++ielite){
        for (int icrossover = 0; icrossover < 2; ++icrossover){
            for (int imutation = 0; imutation < 4; ++imutation){
                solveFactorialTest(instance, npop, ngen, ngenes,
                                   vnelite[ielite], vpcrossover[icrossover],
                                   vpmutation[imutation], pwinner,
                                   output_file, crossover_id, selection_id);
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

        if(ag->count_gen_local_min > 100){
            if(ag->count_gen_local_min > 150) break; 
            ag->twoOpt();
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
    const int npop = 800;
    const int nelite = 4;
    const double pcrossover = 1.0;
    const double pwinner = 0.95;
    const double pmutation = 0.01;

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

    int ngen = instance.n * instance.m * 2;
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
            if(ag->count_gen_local_min > sqrt(ngen) * 5 + sqrt(sqrt(ngen))) break; 
            //ag->twoOpt();
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
