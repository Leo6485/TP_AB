#include "aco.hpp"
#include "io.hpp"
#include "solve.hpp"

#include <bits/stdc++.h>

#define _ ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

using namespace std;

int main(int argc, char **argv) { 
    Io io;
    
    string input_file_name = io.getInputFileName(argc, argv);
    cout << input_file_name << endl;

    ifstream input_file(input_file_name);
    int solution_id = io.getSolutionId(argc, argv);

    if(not input_file.is_open()){
        cerr << "Erro ao abrir o arquivo de entrada.\n";
        return 1;
    }

    Solve solve;

    switch(solution_id){
        case 1: {
            ofstream output_file(io.getOutputFileName(argc, argv));

            if(not output_file.is_open()){
                cerr << "Erro ao abrir o arquivo de saida.\n";
                return 1;
            }

            solve.solve(input_file, output_file);
            break;
        }   
        case 2: {
            ofstream gen_output_file(io.getGenOutputFileName(argc, argv));

            if(not gen_output_file.is_open()){
                cerr << "Erro ao abrir o arquivo de saida do teste de geracoes." << '\n';
                return 1;
            }

            solve.generationsTest(input_file, gen_output_file);
            break;
        }
        case 3: {
            ofstream fac_output_file(io.getFacOutputFileName(argc, argv));

            if(not fac_output_file.is_open()){
                cerr << "Erro ao abrir o arquivo de saida do teste fatorial.\n";
                return 1;
            }

            solve.factorialTest(input_file, fac_output_file);
            break;
        }
    };
    return 0;
}
