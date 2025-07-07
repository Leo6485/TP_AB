#include <bits/stdc++.h>
#include <sys/time.h>

#include "io.hpp"
#include "solve.hpp"

using namespace std;

void printTimeValDelta(struct timeval start, struct timeval end, ofstream &output_file) {
    // Calcula a diferença em segundos
    long int sec_delta = end.tv_sec - start.tv_sec;
    // Calcula a diferença em microsegundos
    long int mil_sec_delta = end.tv_usec - start.tv_usec;

    if (mil_sec_delta < 0) {
        sec_delta -= 1;
        mil_sec_delta += 1000000;
    }

    output_file << "Tempo de Execucao (segundos): " << sec_delta << '.' << mil_sec_delta; 
}

int main(int argc, char** argv){
    struct timeval clock_start_case, clock_end_case;
    gettimeofday(&clock_start_case, NULL);

    // Obtém as informações necessárias do usuário para executar o algoritmo
    Io io;
    int solution = io.getSolutionId(argc, argv);
    int crossover_id = io.getCrossoverId(argc, argv);
    int selection_id = io.getSelectionId(argc, argv);
   
    // Obtém o nome do arquivo de entrada e abre o arquivo de entrada
    ifstream input_file(io.getInputFile(argc, argv));
    if(not input_file.is_open()){
        cerr << "Erro ao abrir o arquivo de entrada.\n";
        return 1;
    }

    // Obtém o nome do arquivo de saída e abre o arquivo de saída
    ofstream output_file(io.getOutputFile(argc, argv));
    if(not output_file.is_open()){
        cerr << "Erro ao abrir o arquivo de saida.\n";
        return 1;
    }

    Solve solve;

    switch(solution){
        case 1: {
            solve.solve(input_file, crossover_id, selection_id, output_file);
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
    printTimeValDelta(clock_start_case, clock_end_case, output_file);

    output_file.close();
    return 0;
}
