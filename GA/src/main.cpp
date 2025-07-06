#include <bits/stdc++.h>
#include <sys/time.h>

#include "io.hpp"
#include "solve.hpp"

using namespace std;

#define FACTORIAL_TEST_RESULTS_FILE "../data/results.csv"
#define GENERATIONS_RESULTS_FILE "../data/generations.csv"

void printTimeValDelta(
    struct timeval start, struct timeval end) {
    // Calcula a diferença em segundos
    long int sec_delta = end.tv_sec - start.tv_sec;
    // Calcula a diferença em microsegundos
    long int mil_sec_delta = end.tv_usec - start.tv_usec;
    if (mil_sec_delta < 0) {
        sec_delta -= 1;
        mil_sec_delta += 1000000;
    }
    cout << sec_delta << "." << mil_sec_delta;
}

int main(int argc, char** argv){
    struct timeval clock_start_case, clock_end_case;
    gettimeofday(&clock_start_case, NULL);
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
            break;
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
    gettimeofday(&clock_end_case, NULL);
    cout << "Tempo de Execucao (segundos): ";
    printTimeValDelta(clock_start_case, clock_end_case);
    return 0;
}
