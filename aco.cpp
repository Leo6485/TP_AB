#include "aco.h"
#include <omp.h>

using namespace std;

double p(double a, int b) {
    double result = a;

    if(b > 20) return pow(a, b);
    if (b == 0) return 1.0;

    for (int i = 0; i < (b - 1); i++) {
        result *= a;
    }

    return result;
}

Aco::Aco(int n, int m, int a, int b, int Q_, float evaporacao_) {
    machines.assign(n, vector<int>(m, 0));
    pheromones.assign(n, vector<double>(n, 1e-16));
    probabilities.assign(n, vector<double>(n));
    itens_raw_time.assign(n, vector<int>(n));
    
    num_itens = n;
    num_machines = m;
    
    alfa = a;
    beta = b;
    Q = Q_;
    evaporacao = 1 - evaporacao_;

    att_probabilities();
}

void Aco::calculate_raw_times() {
    for (int i = 0; i < num_itens; i++) {
        vector<int> makespan(num_machines);
        makespan[0] = machines[i][0];
        for (int j = 1; j < num_machines; j++) {
            makespan[j] = makespan[j - 1] + machines[i][j];
        }

        for (int j = 0; j < num_itens; j++) {
            if (j == i) continue;
            vector<int> copy = makespan;
            copy[0] += machines[j][0];
            for (int k = 1; k < num_machines; k++) {
                copy[k] = max(copy[k-1], copy[k]);
                copy[k] += machines[j][k];
            }
            itens_raw_time[i][j] = copy[num_machines - 1] - makespan[num_machines - 1];
        }
    }
}

void Aco::add_item_machine_time(int item, int machine, int w) {
    machines[item][machine] = w;
}

int Aco::get_next_item(
    int v,
    vector<int>& makespan,
    vector<bool>& visitados,
    int items_left
) {
    vector<double> weights(items_left + 1);
    vector<int> nodes(items_left);
    vector<vector<int>> prob_makespans(items_left, vector<int> (makespan));
    weights[0] = 0;

    double total = 0;
    int i = 0;
    for (int node = 0; node < num_itens; node++) {
        if (!visitados[node]) {
            prob_makespans[i][0] = prob_makespans[i][0] + machines[node][0];
            for(int j = 1; j < num_machines; j++) {
                prob_makespans[i][j] = max(prob_makespans[i][j-1], prob_makespans[i][j]);
                prob_makespans[i][j] += machines[node][j];
            }
            double delta = prob_makespans[i][num_machines - 1] - makespan[num_machines - 1];
            total += probabilities[v][node] * pow(1 / delta, beta);
            weights[i+1] = total;
            nodes[i] = node;
            i++;
        }
    }
    
    double pos = ((double)rand() / RAND_MAX) * weights[items_left];

    for (i = 1; i < items_left; i++) {
        if (weights[i] >= pos) break;
    }
    int i_1 = i-1;
    makespan = prob_makespans[i_1];
    visitados[nodes[i_1]] = true;
    return nodes[i_1];
    
}

void Aco::evaporate() {
    #pragma omp parallel for
    for (int i = 0; i < num_itens; i++) {
        for (int j = 0; j < num_itens; j++) {
            pheromones[i][j] *= evaporacao;
            if (pheromones[i][j] == 0)
                pheromones[i][j] = 1e-16;
        }   
    }
}

void Aco::path_pheromones(vector<int>& path, int result) {
    double value = (double)Q / result;
    for (int i = 1; i < num_itens; i++) {
        int from = path[i-1];
        int to = path[i];
        pheromones[from][to] += value;
    }
}

void Aco::paths_pheromones(vector<vector<int>>& path, vector<int> result) {
    int num_ants = path.size();
    for (int ant = 0; ant < num_ants; ant++) {
        path_pheromones(path[ant], result[ant]);
    }
}

void Aco::att_probabilities() {
    #pragma omp parallel for
    for (int i = 0; i < num_itens; i++) {
        for (int j = 0; j < num_itens; j++) {
            probabilities[i][j] = pow(pheromones[i][j], alfa);
        }   
    }
}

void Aco::run(int run_id, int generations, int num_ants) {

    int global_best = numeric_limits<int>::max();
    vector<int> best_path;
    
    for (int h = 0; h < generations; h++) {
        vector<vector <int>> ant_paths(num_ants);
        vector<int> ant_paths_results(num_ants);
        //#pragma omp parallel for
        for (int i = 0; i < num_ants; i++) {
            vector<int> path(num_itens);
            vector<int> makespan(num_machines);
            vector<bool> visitados(num_itens, false);
            
            path[0] = rand() % num_itens; // Primeiro vértice
            visitados[path[0]] = true;
            makespan[0] = machines[path[0]][0];
            for(int i = 1; i < num_machines; i++) {
                makespan[i] = makespan[i-1] + machines[path[0]][i];
            }

            // Caminho da formiga
            for (int j = 1; j < num_itens; j++) {
                path[j] = get_next_item(path[j-1], makespan, visitados, num_itens - j);
            }

            int result = makespan[num_machines - 1];
            ant_paths[i] = path;
            ant_paths_results[i] = result;

            //#pragma omp critical
            {
                if (result < global_best) {    
                    global_best = result;
                    best_path = path;
                    cout << global_best << " " << h << "\n";
                }
            }
        }   
        // Update data
        evaporate();
        paths_pheromones(ant_paths, ant_paths_results);
        path_pheromones(best_path, global_best * 8);
        att_probabilities();
    }
    cout << global_best << "\n";
    for (int i = 0; i < num_itens; i++) {
        cout << best_path[i] << " ";
    }
    cout << "\n";
}

Aco::~Aco() {}
