#ifndef ACO_HPP
#define ACO_HPP

#include <bits/stdc++.h>

#include "graph.hpp"
#include "utils.hpp"

using namespace std;

class Aco {
public:
    const Graph *graph;
    Graph pheromones;
    
    vector<vector<int>> itens_raw_time;

    int npop;
    int ngen;
    double alfa;
    double beta;
    double Q;
    double evaporation;
    const double STARTING_PHEROMONES = 1e-16;

    int num_items;
    int num_machines;

    double min_makespan;
    double sum_costs;
    vector<int> best_sequence;

    void updatePheromones(const Graph &used_costs);
    void updateUsedCosts(Graph &used_costs, const pair<vector<int>, double> &sequence_info);
    pair<vector<int>, double> buildSequence(const int first_task);
    void buildAntsStart(vector<int> &ants_start);

    void calculate_raw_times();
    int getNextItem(int v, vector<int> &makespan, vector<bool>& visited, int items_left);
    
    Aco(const Graph *_graph, const int _npop, const int _ngen, const double _alfa, const double _beta, const double _Q, const double _p);
    ~Aco();
};

#endif