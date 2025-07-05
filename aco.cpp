#include "aco.hpp"
#include <omp.h>

#define dbg(x) cout << #x << " == " << x << endl

mt19937 globalGenerator(random_device{}());

Aco::Aco(const Graph *_graph, const int _npop, const int _ngen, const double _alfa, const double _beta, const double _Q, const double _p) :
    graph(_graph), npop(_npop), ngen(_ngen), alfa(_alfa), beta(_beta), Q(_Q), evaporation(1.0 - _p)
{
    num_items = graph->getNumRows();
    num_machines = graph->getNumColumns(); 

    itens_raw_time.assign(num_items, vector<int> (num_items));

    pheromones.initGraph(num_items, num_items, STARTING_PHEROMONES);

    min_makespan = numeric_limits<double>::max();
    sum_costs = 0.0;
}

void Aco::calculate_raw_times() {
    for (int i = 0; i < num_items; i++) {
        vector<int> makespan(num_machines);
        makespan[0] = graph->getEdge(i, 0);
        for (int j = 1; j < num_machines; j++) {
            makespan[j] = makespan[j - 1] + graph->getEdge(i, j);
        }

        for (int j = 0; j < num_items; j++) {
            if (j == i) continue;
            vector<int> copy = makespan;
            copy[0] += graph->getEdge(j, 0);
            for (int k = 1; k < num_machines; k++) {
                copy[k] = max(copy[k-1], copy[k]);
                copy[k] += graph->getEdge(j, k);
            }
            itens_raw_time[i][j] = copy[num_machines - 1] - makespan[num_machines - 1];
        }
    }
}

int Aco::getNextItem(int v, vector<int> &makespan, vector<bool>& visited, int items_left){
    double current_final = makespan[num_machines-1];
    vector<double> weights(1, 0.0);
    vector<int> nodes;
    nodes.reserve(items_left);
    double total = 0.0;

    for (int node = 0; node < num_items; node++) {
        if (visited[node]) continue;
             
        int new_finish = makespan[0] + graph->getEdge(node, 0);
        for (int j = 1; j < num_machines; j++) {
            new_finish = max(new_finish, makespan[j]);
            new_finish += graph->getEdge(node, j);
        }
        double delta = new_finish - current_final;
        delta = max(delta, 1e-16); // Evitar divisão por 0

        double heuristic = 1.0 / delta;
        double pheromone = pheromones.getEdge(v, node);

        double weight = utils::pow(pheromone, alfa) * utils::pow(heuristic, beta);
        total += weight;

        weights.push_back(total);
        nodes.push_back(node);
    }

    int idx = 0;
    if (items_left > 1) {
        uniform_real_distribution<double> dist(0.0, total);
        double r = dist(globalGenerator);

        auto it = lower_bound(weights.begin() + 1, weights.end(), r);
        idx = distance(weights.begin() + 1, it);
    }

    int chosen_node = nodes[idx];

    makespan[0] += graph->getEdge(chosen_node, 0);
    for (int j = 1; j < num_machines; j++) {
        makespan[j] = max(makespan[j-1], makespan[j]);
        makespan[j] += graph->getEdge(chosen_node, j);
    }

    visited[chosen_node] = true;
    return chosen_node;
}

pair<vector<int>, double> Aco::buildSequence(const int first_task){
    vector<int> sequence (num_items);
    vector<bool> visited (num_items, false);
    vector<int> makespan (num_machines);

    visited[first_task] = true;

    sequence[0] = first_task;
    makespan[0] = graph->getEdge(first_task, 0);

    for(int i = 1; i < num_machines; ++i){
        makespan[i] = makespan[i-1] + graph->getEdge(first_task, i);
    }

    for(int i = 1; i < num_items; ++i){ 
        sequence[i] = getNextItem(sequence[i-1], makespan, visited, num_items - i);
    }

    double result = makespan[num_machines - 1]; 
    sum_costs += result;    
    return make_pair(sequence, result);
}

void Aco::updatePheromones(const Graph &used_costs){
    for(int i = 0; i < num_items; ++i){
        for(int j = 0; j < num_items; ++j){
            double new_pheromone = evaporation * pheromones.getEdge(i, j) + used_costs.getEdge(i, j);

            new_pheromone = max(new_pheromone, STARTING_PHEROMONES);
            
            pheromones.setEdge(i, j, new_pheromone); 
        }
    }
}

void Aco::updateUsedCosts(Graph &used_costs,
                     const pair<vector<int>, double> &sequence_info){ 
    double gbest_bonus = Q / min_makespan;
          
    bool is_gbest = sequence_info.first == best_sequence;

    for(int i = 0; i < num_items - 1; ++i){
        int src = sequence_info.first[i];
        int dest = sequence_info.first[i+1];

        double increment = Q / sequence_info.second;
        used_costs.incrementEdge(src, dest, increment);

        // Incremento adicional nos feromônios se a sequência for a Gbest
        if(is_gbest)
            used_costs.incrementEdge(src, dest, gbest_bonus);
    }
}

void Aco::buildAntsStart(vector<int> &ants_start){
    uniform_int_distribution<> dis(0, graph->getNumRows() - 1);
    for(int &ant : ants_start){ 
        ant = dis(globalGenerator);
    }
}

Aco::~Aco() {}
