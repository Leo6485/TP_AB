#include <bits/stdc++.h>

#include "io.hpp"

using namespace std;

string Io::getInputFile(const int argc, char** argv){
    string input_file_name = "../inputs/in.txt";
    string flag = "-i";
    for(int i = 1; i < argc; i+= 2){
        string argument = argv[i];
        if(argument == flag){
            input_file_name = argv[i+1];
            input_file_name = "../inputs/" + input_file_name;
            break;
        }
    }
    return input_file_name;
}

string Io::getFacOutputFile() const {
    return "../data/results.csv";
}

string Io::getGenOutputFile() const {
    return "../data/generations.csv";
}

int Io::getSolutionId(const int argc, char** argv){
    for(int i = 1; i < argc; i+= 2){
        if(not strcmp(argv[i], "-s")){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}

int Io::getCrossoverId(const int argc, char **argv){
    for(int i = 1; i < argc; i += 2){
        if(not strcmp(argv[i], "-c")){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}

int Io::getSelectionId(const int argc, char **argv){
    for(int i = 1; i < argc; i += 2){
        if(not strcmp(argv[i], "-p")){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}
