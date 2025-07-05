#include "io.hpp"

string Io::getFileName(const int &argc, char **argv, const string &flag){
    string file_name;
    for(int i = 1; i < argc; i += 2){
        string argument = argv[i];
        if(argument == flag){
            file_name = argv[i+1];
            return file_name;
        }
    }

    return "";
}

string Io::getFacOutputFileName(const int &argc, char **argv){
    string flag = "-o";
    string fac_output_file_name = getFileName(argc, argv, flag);
    if(fac_output_file_name == "")
        fac_output_file_name = "results.csv";
    fac_output_file_name = "tables/" + fac_output_file_name;
    return fac_output_file_name;
}

string Io::getGenOutputFileName(const int &argc, char **argv){
    string flag = "-o";
    string gen_output_file_name = getFileName(argc, argv, flag);
    if(gen_output_file_name == "")
        gen_output_file_name = "generations.csv";
    gen_output_file_name = "tables/" + gen_output_file_name;
    return gen_output_file_name;
}

string Io::getOutputFileName(const int &argc, char **argv){
    string flag = "-i";
    string output_file_name = "resultado_instancia_";
    int input_id = getId(argc, argv, flag);
    output_file_name += input_id;
    output_file_name += ".txt";
    return output_file_name;
}

string Io::getInputFileName(const int &argc, char **argv){
    string flag = "-i";
    int input_id = getId(argc, argv, flag);
    string input_file_name = "fssp_instance_0";
    input_file_name += to_string(input_id);
    input_file_name += ".txt";
    return input_file_name;
}

int Io::getId(const int &argc, char **argv, const string &flag){
    for(int i = 1; i < argc; i+=2){
        if(not strcmp(argv[i], flag.c_str())){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}

int Io::getSolutionId(const int &argc, char **argv){
    return getId(argc, argv, "-s");
}

void Io::printResults(const vector<int> &sequence, double min_makespan, ofstream &output_file){
    output_file << "Melhor Makespan: " << min_makespan << endl;
    
    output_file << "Melhor Sequencia:";
    for(auto& task : sequence) output_file << " " << task;
    output_file << endl;

    output_file << "Tempo de Execucao (segundos): " << endl;
}