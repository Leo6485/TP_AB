#include "aco.h"
#include <omp.h>

using namespace std;

Aco::Aco(int n, int m, int a, int b, int Q_, float e_) {
    dists.assign(n,  vector<int>(n, 0)); // Distâncias
    machines.assign(n, vector<int>(m, 0));
    size = n;
    evaporacao = 0.5;

    csv << "Exe, Geracao, Alfa, Beta, E, Melhor Custo, Melhor custo da geracao, Media, Pior\n";
    this->set(a, b, Q_, e_);
}

// Reinicia todos os valores, exceto o grafo
void Aco::set(int a, int b, int Q_, float e_) {
    data.assign(size, vector<double>(size, 1e-6));
    acumulado.assign(size, vector<double>(size, 0));

    alfa = a;
    beta = b;
    Q = Q_;
    e = e_;

    for (int i = 0; i < size; i++) data[i][i] = 0;
}

void Aco::add_conn(int v, int u, int w) {
    dists[v][u] = w;
    // dists[u][v] = w;
}

void Aco::add_machine(int item, int machine, int w) {
    machines[item][machine] = w;
}

double p(double a, int b) {
    double result = a;

    if(b > 20) return pow(a, b);
    if (b == 0) return 1.0;

    for (int i = 0; i < (b - 1); i++) {
        result *= a;
    }

    return result;
}

int Aco::get_next(int v, vector<bool>& visitados) {
    vector<double> weights;
    double total = 0;

    for (int i = 0; i < size; i++) {
        if (v != i && !visitados[i]) {
            total += p(data[v][i], alfa) / p((double)dists[v][i], beta);
        }

        weights.push_back(total);
    }

    double pos = ((double)rand() / RAND_MAX) * weights.back();

    for (int i = 0; i < size; i++) {
        if (weights[i] >= pos) return i;
    }

    return 0;
}

void Aco::update_acumulado(vector<int>& path, int result, int e) {
    double value = e * ((double)Q / result);
    for (int i = 1; i < size; i++) {
        acumulado[path[i - 1]][path[i]] += value;
        // acumulado[path[i]][path[i - 1]] += value;
    }

    // acumulado[path[0]][path.back()] += value;
    // acumulado[path.back()][path[0]] += value;
}

void Aco::update() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;

            data[i][j] = (1 - evaporacao) * data[i][j] + acumulado[i][j];
            acumulado[i][j] = 0;

            if (data[i][j] < 1e-6) data[i][j] = 1e-6;
        }
    }
}

void Aco::show() {
    for (auto &linha : data) {
        for (auto &item : linha) {   
            cout << item << " ";
        }
        cout << "\n";
    }
}

void Aco::save(string filename) {
    ofstream file(filename);
    file << csv.str();
    file.close();
}

void Aco::run(int run_id, int generations, int num_ants) {

    int global_best = numeric_limits<int>::max();
    vector<int> best_path;
    
    for (int h = 0; h < generations; h++) {
        int pior = 0;
        int best = numeric_limits<int>::max();
        int mean = 0;

        #pragma omp parallel for
        for (int i = 0; i < num_ants; i++) {
            vector<int> path;
            vector<bool> visitados(size, false);
            
            int x = rand() % size; // Primeiro vértice

            path.push_back(x);
            visitados[x] = true;

            // Caminho da formiga
            for (int j = 0; j < size - 1; j++) {
                int next = get_next(path.back(), visitados);
                visitados[next] = true;
                path.push_back(next);
            }

            int result = calc(path, machines);

            #pragma omp critical
            {
                mean += (double)result / num_ants;
                
                update_acumulado(path, result, 1);
                
                if (result < best) best = result;
                
                if (result < global_best) {
                    global_best = result;
                    best_path = path;
                }

                if (result > pior) {
                    pior = result;
                }
            }

        }

        // Reforço para o melhor caminho
        update_acumulado(best_path, global_best, e);

        // Update data
        update();

        csv << run_id << ", " << h << ", " << alfa << ", " << beta << ", " << e << ", " << global_best << ", " << best << ", " << mean << ", " << pior << "\n";
        cout << global_best << "\n";
    }
}
Aco::~Aco() {}

int calc(const vector<int>& path, const vector<vector<int>>& machines) {
    vector<int> result(machines[0].size() + 1, 0);
    for (auto &item : path) {
        // cout << item << " ";
        result[0] = result[0] + machines[item][0];
        for(int i = 1; i < machines[0].size(); i++) {
            result[i] = max((result[i-1]), result[i]) + machines[item][i];
        }
    }
    // cout << "\n";
    return result[result.size() - 2];
}

int calcula_ociosidade(const vector<int>& path, const vector<vector<int>>& machines) {
    vector<int> result(machines[0].size() + 1, 0);
    int ocioso = 0;
    for (auto &item : path) {
        // cout << item << " ";
        result[0] = result[0] + machines[item][0];
        for(int i = 1; i < machines[0].size(); i++) {
            int tempo_ideal = result[i-1]; // Tempo ideal para começar o serviço
            int maquina_livre = result[i]; // Tempo quando a próxima máquina poderá ser utilizada
            ocioso += abs(maquina_livre - tempo_ideal);
            result[i] = max((result[i-1]), result[i]) + machines[item][i];
        }
    }
    // cout << "\n";
    return ocioso;
}