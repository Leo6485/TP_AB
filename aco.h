#ifndef ACO_HPP
#define ACO_HPP

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <sstream>

using namespace std;

class Aco {
public:
    vector<vector<double>> pheromones;
    vector<vector<double>> probabilities;
    vector<vector<int>> machines; // Máquinas
    vector<vector<int>> itens_raw_time;

    int num_itens;
    int num_machines;

    double alfa;
    double beta;
    double Q;
    double evaporacao;

    Aco(int n, int m, int a, int b, int Q_, float evaporacao_);
    void calculate_raw_times();
    void add_item_machine_time(int item, int machine, int w);
    int get_next_item(int v, vector<int>& makespan, vector<bool>& visitados, int items_left);
    void evaporate();
    void path_pheromones(vector<int>& path, int result);
    void paths_pheromones(vector<vector<int>>& path, vector<int> result);
    void att_probabilities();
    void run(int run_id, int generations, int num_ants);
    ~Aco();
};

#endif