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
    vector<vector<double>> data;
    vector<vector<double>> acumulado;
    vector<vector<int>> dists; // Grafo
    vector<vector<int>> machines; // Máquinas


    float evaporacao;
    int size;

    int alfa;
    int beta;
    double Q;
    double e;

    ostringstream csv;

    Aco(int n, int m, int a, int b, int Q_, float e_);
    void add_conn(int v, int u, int w);
    void add_machine(int item, int machine, int w);
    int get_next(int v, vector<bool>& visitados);
    void update_acumulado(vector<int>& path, int result, int e);
    void update();
    void show();
    void save(string filename);
    void run(int run_id, int generations, int num_ants);
    void set(int a, int b, int Q_, float e);
    ~Aco();
};

int calc(const vector<int>& path, const vector<vector<int>>& machines);
int calcula_ociosidade(const vector<int>& path, const vector<vector<int>>& machines);

#endif