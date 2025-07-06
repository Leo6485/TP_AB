#include <bits/stdc++.h>

#include "io.hpp"

using namespace std;

// Lê o índice do arquivo de entrada
string Io::getInputFile(const int argc, char** argv){
    string input_file_name = "../inputs/fssp_instance_0";
    int input_file_id = 1;
    string flag = "-i";
    for(int i = 1; i < argc; i+= 2){
        string argument = argv[i];
        if(argument == flag){
            input_file_id = atoi(argv[i+1]);
            break;
        }
    }
    return input_file_name + to_string(input_file_id) + ".txt";
}

string Io::getOutputFile(const int argc, char **argv){
    string output_file_name = "resultado_instancia_0";
    int output_file_id = 1;
    string flag = "-i";
    for(int i = 1; i < argc; i += 2){
        string argument = argv[i];
        if(argument == flag){
            output_file_id = atoi(argv[i+1]);
            break;
        }
    }
    return output_file_name + to_string(output_file_id) + ".txt";
}

string Io::getFacOutputFile() const {
    return "../data/tables/results.csv";
}

string Io::getGenOutputFile() const {
    return "../data/tables/generations.csv";
}

// Obtém o id da solução a ser executada
int Io::getSolutionId(const int argc, char** argv){
    for(int i = 1; i < argc; i+= 2){
        if(not strcmp(argv[i], "-s")){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}

// Obtém o id do crossover a ser utilizado
int Io::getCrossoverId(const int argc, char **argv){
    for(int i = 1; i < argc; i += 2){
        if(not strcmp(argv[i], "-c")){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}

// Obtém o id da forma de seleção de pais a ser utilizada
int Io::getSelectionId(const int argc, char **argv){
    for(int i = 1; i < argc; i += 2){
        if(not strcmp(argv[i], "-p")){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}
