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
    dists.assign(n,  vector<double>(n, 0)); // Distâncias
    machines.assign(n, vector<int>(m, 0));
    pheromones.assign(n, vector<double>(n, 1e-6));
    probabilities.assign(n, vector<double>(n));
    
    num_itens = n;
    num_machines = m;
    
    alfa = a;
    beta = b;
    Q = Q_;
    evaporacao = 1 - evaporacao_;

    att_probabilities();
}

void Aco::add_item_distance(int v, int u, int w) {
    dists[v][u] = pow(1 / (double)w, beta);
}

void Aco::add_item_machine_time(int item, int machine, int w) {
    machines[item][machine] = w;
}

int Aco::get_next_item(int v, vector<bool>& visitados, int items_left) {
    vector<double> weights(items_left + 1);
    vector<int> nodes(items_left);
    weights[0] = 0;

    double total = 0;
    int i = 1;
    for (int node = 0; node < num_itens; node++) {
        if (!visitados[node]) {
            total += probabilities[v][node];
            weights[i] = total;
            nodes[i-1] = node;
            i++;
        }
    }

    double pos = ((double)rand() / RAND_MAX) * weights[items_left];

    for (i = 1; i < items_left; i++) {
        if (weights[i] >= pos) break;
    }
    
    return nodes[i - 1];
}

void Aco::evaporate() {
    #pragma omp parallel for
    for (int i = 0; i < num_itens; i++) {
        for (int j = 0; j < num_itens; j++) {
            pheromones[i][j] *= evaporacao;
            if (pheromones[i][j] == 0.0)
                pheromones[i][j] = 1e-6;
        }   
    }
}

void Aco::paths_pheromones(vector<vector<int>>& path, vector<int> result) {
    int num_ants = path.size();
    for (int ant = 0; ant < num_ants; ant++) {
        double value = (double)Q / result[ant];
        for (int i = 1; i < num_itens; i++) {
            int from = path[ant][i-1];
            int to = path[ant][i];
            pheromones[from][to] += value;
        }
    }
}

void Aco::att_probabilities() {
    #pragma omp parallel for
    for (int i = 0; i < num_itens; i++) {
        for (int j = 0; j < num_itens; j++) {
            probabilities[i][j] = pow(pheromones[i][j], alfa) * dists[i][j];
        }   
    }
}

void Aco::run(int run_id, int generations, int num_ants) {

    int global_best = numeric_limits<int>::max();
    vector<int> best_path;
    
    for (int h = 0; h < generations; h++) {
        vector<vector <int>> ant_paths(num_ants);
        vector<int> ant_paths_results(num_ants);
        #pragma omp parallel for
        for (int i = 0; i < num_ants; i++) {
            vector<int> path(num_itens);
            vector<bool> visitados(num_itens, false);
            
            path[0] = rand() % num_itens; // Primeiro vértice
            visitados[path[0]] = true;

            // Caminho da formiga
            for (int j = 1; j < num_itens; j++) {
                int next = get_next_item(path[j-1], visitados, num_itens - j);
                visitados[next] = true;
                path[j] = next;
            }

            int result = calc(path, machines);
            ant_paths[i] = path;
            ant_paths_results[i] = result;

            #pragma omp critical
            {
                if (result < global_best) {    
                    global_best = result;
                    best_path = path;
                }
            }
        }   
        // Update data
        paths_pheromones(ant_paths, ant_paths_results);
        evaporate();
        att_probabilities();
    }
    cout << global_best << "\n";
}

Aco::~Aco() {}

int calc(const vector<int>& path, const vector<vector<int>>& machines) {
    int num_machines = machines[0].size();
    vector<int> result(num_machines + 1, 0);
    for (auto &item : path) {
        // cout << item << " ";
        result[0] = result[0] + machines[item][0];
        for(int i = 1; i < num_machines; i++) {
            result[i] = max((result[i-1]), result[i]) + machines[item][i];
        }
    }
    // cout << "\n";
    return result[result.size() - 2];
}

int calcula_ociosidade(const vector<int>& path, const vector<vector<int>>& machines) {
    int num_machines = machines[0].size();
    vector<int> result(num_machines + 1, 0);
    int ocioso = 0;
    for (auto &item : path) {
        // cout << item << " ";
        result[0] = result[0] + machines[item][0];
        for(int i = 1; i < num_machines; i++) {
            int tempo_ideal = result[i-1]; // Tempo ideal para começar o serviço
            int maquina_livre = result[i]; // Tempo quando a próxima máquina poderá ser utilizada
            ocioso += abs(maquina_livre - tempo_ideal);
            result[i] = max((result[i-1]), result[i]) + machines[item][i];
        }
    }
    // cout << "\n";
    return ocioso;
}