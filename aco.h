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
    vector<vector<double>> dists; // Grafo
    vector<vector<int>> machines; // Máquinas

    int num_itens;
    int num_machines;

    double alfa;
    double beta;
    double Q;
    double evaporacao;

    Aco(int n, int m, int a, int b, int Q_, float evaporacao_);
    void add_item_distance(int v, int u, int w);
    void add_item_machine_time(int item, int machine, int w);
    int get_next_item(int v, vector<bool>& visitados, int items_left);
    void evaporate();
    void paths_pheromones(vector<vector<int>>& path, vector<int> result);
    void att_probabilities();
    void run(int run_id, int generations, int num_ants);
    ~Aco();
};

int calc(const vector<int>& path, const vector<vector<int>>& machines);
int calcula_ociosidade(const vector<int>& path, const vector<vector<int>>& machines);

#endif