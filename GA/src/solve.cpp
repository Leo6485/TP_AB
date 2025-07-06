#include "solve.hpp"
#include "ag.hpp"
#include "build_csv.hpp"
#include "instance.hpp"

#include <bits/stdc++.h>

using namespace std;

void Solve::solve(ifstream &input_file, const int crossover_id,
                  const int selection_id){
    const int npop = 200;
    const int ngen = 20000;
    const int nelite = 6;
    const double pcrossover = 0.9;
    const double pwinner = 0.9;
    const double pmutation = 0.01;

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

    const int ngenes = instance.n;

    Ag *ag = new Ag(instance, npop, ngen, ngenes, nelite, pcrossover, pmutation, pwinner);

    ag->initPopulation();

    for(int generation = 0; generation < ag->ngen; ++generation){
        ag->evaluatePopulation();
        ag->twoOpt();
        vector<int> parents = ag->parentsSelection(selection_id);

        ag->crossover(parents, crossover_id);

        ag->mutation();
        ag->elitism();

        ag->copyPopulation();
        ag->printResults();
    }

    ag->printResults();

    delete ag;
}

void Solve::factorialTest(ifstream &input_file, ofstream &output_file,
                          const int crossover_id, const int selection_id){
    const int vnpop[] = {100, 150, 200};
    const int vngen[] = {100, 150, 200};
    const int vnelite[] = {2, 4, 6};
    const double vpcrossover[] = {0.8, 0.9, 1.0};
    const double pwinner = 0.9;
    const double vpmutation[] = {0.01, 0.05, 0.10};

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

    const int ngenes = instance.n;

    BuildCSV csv_builder;
    csv_builder.printFacIdfCSV(output_file);

    for(int ipop = 0; ipop < 3; ++ipop){
        for(int igen = 0; igen < 3; ++igen){
            for(int ielite = 0; ielite < 3; ++ielite){
                for(int icrossover = 0; icrossover < 3; ++icrossover){
                    for(int imutation = 0; imutation < 3; ++imutation){
                        solveFactorialTest(instance, vnpop[ipop], vngen[igen], ngenes,
                                           vnelite[ielite], vpcrossover[icrossover],
                                           vpmutation[imutation], pwinner,
                                           output_file, crossover_id, selection_id);
                    }
                }
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

        vector<int> parents = ag->parentsSelection(selection_id);
        ag->crossover(parents, crossover_id);

        ag->mutation();
        ag->elitism();

        ag->copyPopulation();
    }

    csv_builder.printFacResultsCSV(ag, output_file);
    delete ag;
}

void Solve::generationsTest(ifstream &input_file, ofstream &output_file,
                            const int crossover_id, const int selection_id){
    const int npop = 150;
    const int ngen = 150;
    const int nelite = 6;
    const double pcrossover = 0.9;
    const double pwinner = 0.9;
    const double pmutation = 0.01;

    Instance instance;
    instance.readInstance(input_file);
    input_file.close();

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

        vector<int> parents = ag->parentsSelection(selection_id);
        ag->crossover(parents, crossover_id);

        ag->mutation();
        ag->elitism();

        ag->copyPopulation();

        csv_builder.printGenResultsCSV(ag, output_file, generation);
    }
    delete ag;
}
