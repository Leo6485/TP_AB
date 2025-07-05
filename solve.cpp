#include "solve.hpp"
#include "build_csv.hpp"
#include "io.hpp"
#include "graph.hpp"

#define dbg(x) cout << #x << " == " << x << endl

void Solve::solve(ifstream &input_file, ofstream &output_file){
    const int npop = 200;
    const int ngen = 200;
    const double Q = 100.0;
    const double alfa = 1.0;
    const double beta = 5.0;
    const double p = 0.3;

    Graph *graph = new Graph();
    graph->readGraph(input_file);
    input_file.close(); 

    Aco *aco = new Aco(graph, npop, ngen, alfa, beta, Q, p);

    vector<int> ants_start(aco->npop);
    aco->buildAntsStart(ants_start);

    for(int generation = 0; generation < 200; ++generation){   
        Graph used_costs;
        used_costs.initGraph(graph->getNumRows(), graph->getNumRows(), 0.0);

        for(int ant = 0; ant < aco->npop; ++ant){
            auto sequence_info = aco->buildSequence(ants_start[ant]);

            dbg(sequence_info.second);

            aco->updateUsedCosts(used_costs, sequence_info);

            if(sequence_info.second < aco->min_makespan){
                aco->min_makespan = sequence_info.second;
                aco->best_sequence = sequence_info.first;
            }
        }

        aco->updatePheromones(used_costs);
        aco->buildAntsStart(ants_start);
    }

    // Imprimir resultados
    Io io;
    io.printResults(aco->best_sequence, aco->min_makespan, output_file);

    delete graph;
    delete aco;
}

void Solve::generationsTest(ifstream &input_file, ofstream &output_file){
    const int npop = 200;
    const int ngen = 200;
    const double Q = 100.0;
    const double alfa = 1.5;
    const double beta = 1.3;
    const double p = 0.5;

    Graph *graph = new Graph();
    graph->readGraph(input_file);
    input_file.close(); 

    solveGenerationsTest(graph, npop, ngen, Q, alfa, beta, p, output_file);

    delete graph;
    output_file.close();
}

void Solve::factorialTest(ifstream &input_file, ofstream &output_file){
    const int npop = 500;
    int ngen = npop;
    array<double, 1> Q = {100};
    array<double, 1> alfa = {1.5};
    array<double, 1> beta = {1.0};
    array<double, 1> p = {0.5};

    Graph *graph = new Graph();
    graph->readGraph(input_file);
    input_file.close(); 

    BuildCSV *build_csv = new BuildCSV();
    build_csv->printFacIdfCSV(output_file);
    delete build_csv;

    for(int iQ = 0; iQ < (int) Q.size(); ++iQ){
        for(int ialfa = 0; ialfa < (int) alfa.size(); ++ialfa){
            for(int ibeta = 0; ibeta < (int) beta.size(); ++ibeta){
                for(int ip = 0; ip < (int) p.size(); ++ip){
                    solveFactorialTest(graph, npop, ngen, Q[iQ], alfa[ialfa], beta[ibeta], p[ip], output_file);
                }
            }
        }
    }

    delete graph;
    output_file.close();
}

void Solve::solveFactorialTest(const Graph *graph, const int npop, const int ngen, const double Q, const double alfa, const double beta, const double p, ofstream &output_file){
    Aco *aco = new Aco(graph, npop, ngen, alfa, beta, Q, p);

    Graph used_costs;
    used_costs.initGraph(graph->getNumRows(), graph->getNumRows(), 0.0);

    vector<int> ants_start(aco->npop);
    aco->buildAntsStart(ants_start);

    for(int generation = 0; generation < aco->ngen; ++generation){
        for(int ant = 0; ant < aco->npop; ++ant){
            auto sequence_info = aco->buildSequence(ants_start[ant]);

            aco->updateUsedCosts(used_costs, sequence_info);

            if(sequence_info.second < aco->min_makespan){
                aco->min_makespan = sequence_info.second;
                aco->best_sequence = sequence_info.first;
            }
        }

        aco->updatePheromones(used_costs);
        used_costs.resetGraph(0.0);
        aco->buildAntsStart(ants_start);
    }

    BuildCSV *build_csv = new BuildCSV();
    build_csv->printFacResultsCSV(aco, output_file);
    
    delete build_csv; 
    delete aco;
}

void Solve::solveGenerationsTest(const Graph *graph, const int npop, const int ngen, const double alfa, const double beta, const double Q, const double p, ofstream &output_file){
    Aco *aco = new Aco(graph, npop, ngen, alfa, beta, Q, p);

    BuildCSV *build_csv = new BuildCSV();
    build_csv->printGenIdfCSV(aco, output_file);
    delete build_csv; 

    Graph used_costs;
    used_costs.initGraph(graph->getNumRows(), graph->getNumRows(), 0.0);

    vector<int> ants_start(aco->npop);
    aco->buildAntsStart(ants_start);

    for(int generation = 0; generation < aco->ngen; ++generation){
        output_file << generation;
        for(int ant = 0; ant < aco->npop; ++ant){
            auto sequence_info = aco->buildSequence(ants_start[ant]);

            aco->updateUsedCosts(used_costs, sequence_info);

            if(sequence_info.second < aco->min_makespan){
                aco->min_makespan = sequence_info.second;
                aco->best_sequence = sequence_info.first;
            }

            output_file << ',' << sequence_info.second;
        }
        output_file << '\n';

        aco->updatePheromones(used_costs);
        used_costs.resetGraph(0.0);
        aco->buildAntsStart(ants_start);
    }

    delete aco;
}