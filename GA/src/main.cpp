#include <bits/stdc++.h>

#include "io.hpp"
#include "solve.hpp"

using namespace std;

#define FACTORIAL_TEST_RESULTS_FILE "../data/results.csv"
#define GENERATIONS_RESULTS_FILE "../data/generations.csv"

int main(int argc, char** argv){
    Io io;
    int solution = io.getSolutionId(argc, argv);
    int crossover_id = io.getCrossoverId(argc, argv);
    int selection_id = io.getSelectionId(argc, argv);
    ifstream input_file(io.getInputFile(argc, argv));


    if(not input_file.is_open()){
        cerr << "Erro ao abrir o arquivo de entrada.\n";
        return 1;
    }

    Solve solve;

    switch(solution){
        case 1: {
            solve.solve(input_file, crossover_id, selection_id);
            break;
        }
        case 2: {
            ofstream fac_output_file(io.getFacOutputFile().c_str());
            if(not fac_output_file.is_open()){
                cerr << "Erro ao abrir o arquivo de saida do teste fatorial.\n";
                return 1;
            }

            solve.factorialTest(input_file, fac_output_file, crossover_id,
                                selection_id);
            break;
        }
        case 3: {
            ofstream gen_output_file(io.getGenOutputFile().c_str());
            if(not gen_output_file.is_open()){
                cerr << "Erro ao abrir o arquivo de saida do teste de geracoes.\n";
                return 1;
            }

            solve.generationsTest(input_file, gen_output_file, crossover_id,
                                  selection_id);
            break;
        }
    };
    return 0;
}
