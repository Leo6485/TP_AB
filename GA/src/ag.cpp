#include <bits/stdc++.h>

#include "instance.hpp"
#include "ag.hpp"

using namespace std;

mt19937 globalGenerator(random_device{}());

void Ag::initPopulation(){
    // Inicializa a população inicial aleatória
    for(vector<int> &individual : population){
        iota(individual.begin(), individual.end(), 0);
        shuffle(individual.begin(), individual.end(), globalGenerator);
    }
}

void Ag::evaluatePopulation(){
    for(int idv = 0; idv < npop; ++idv){
        vector<int> result(instance.machines[0].size() + 1, 0);

        for (auto &item : population[idv]) {
            result[0] = result[0] + instance.machines[item][0];
            for(int i = 1; i < (int) instance.machines[0].size(); i++) {
                result[i] = max((result[i-1]), result[i]) + instance.machines[item][i];
            }
        }

        fitness[idv] = result[result.size() - 2];
    }
}

void Ag::twoOpt() {
    uniform_int_distribution<int> dist(0, instance.n - 1);
    std::mt19937 rng(std::random_device{}());

    for (int idv = 0; idv < npop; ++idv) {
        vector<int>& chromo = population[idv];
        // Probabilidade de substituir
        int bestFitness = (rand() % 100 == 0) ? INFINITY : fitness[idv];
        bool improved = false;

        for (int t = 0; t < 100; ++t) {
            int i = dist(rng);
            int j = dist(rng);
            if (i == j) continue;
            if (i > j) std::swap(i, j);

            reverse(chromo.begin() + i, chromo.begin() + j + 1);

            // Calcula makespan
            vector<int> result(instance.machines[0].size() + 1, 0);
            for (auto& item : chromo) {
                result[0] = result[0] + instance.machines[item][0];
                for (int k = 1; k < (int)instance.machines[0].size(); ++k) {
                    result[k] = max(result[k - 1], result[k]) + instance.machines[item][k];
                }
            }
            int newFitness = result[result.size() - 2];

            if (newFitness < bestFitness) {
                bestFitness = newFitness;
                improved = true;
            } else {
                // Reverte a inversão se não melhorou
                reverse(chromo.begin() + i, chromo.begin() + j + 1);
            }
        }

        fitness[idv] = bestFitness;
    }
}

void Ag::threeOpt() {
    std::mt19937 rng(std::random_device{}());
    uniform_int_distribution<int> dist(0, instance.n - 1);

    for (int idv = 0; idv < npop; ++idv) {
        vector<int>& chromo = population[idv];
        int bestFitness = (rand() % 10 == 0) ? INFINITY : fitness[idv];

        for (int t = 0; t < 500; ++t) {
            int i = dist(rng), j = dist(rng), k = dist(rng);
            if (i == j || j == k || i == k) continue;

            // Ordena i < j < k
            if (i > j) std::swap(i, j);
            if (j > k) std::swap(j, k);
            if (i > j) std::swap(i, j);

            // Divide em 4 segmentos: [0,i), [i,j), [j,k), [k,end)
            vector<int> A(chromo.begin(), chromo.begin() + i);
            vector<int> B(chromo.begin() + i, chromo.begin() + j);
            vector<int> C(chromo.begin() + j, chromo.begin() + k);
            vector<int> D(chromo.begin() + k, chromo.end());

            vector<vector<int>> variations = {
                // original: A B C D
                chromo, // original
                // 1: A C B D
                [] (const vector<int>& A, const vector<int>& B, const vector<int>& C, const vector<int>& D) {
                    vector<int> tmp = A;
                    tmp.insert(tmp.end(), C.begin(), C.end());
                    tmp.insert(tmp.end(), B.begin(), B.end());
                    tmp.insert(tmp.end(), D.begin(), D.end());
                    return tmp;
                }(A, B, C, D),
                // 2: A rev(B) rev(C) D
                [] (const vector<int>& A, vector<int> B, vector<int> C, const vector<int>& D) {
                    reverse(B.begin(), B.end());
                    reverse(C.begin(), C.end());
                    vector<int> tmp = A;
                    tmp.insert(tmp.end(), B.begin(), B.end());
                    tmp.insert(tmp.end(), C.begin(), C.end());
                    tmp.insert(tmp.end(), D.begin(), D.end());
                    return tmp;
                }(A, B, C, D),
                // 3: A rev(C) B D
                [] (const vector<int>& A, const vector<int>& B, vector<int> C, const vector<int>& D) {
                    reverse(C.begin(), C.end());
                    vector<int> tmp = A;
                    tmp.insert(tmp.end(), C.begin(), C.end());
                    tmp.insert(tmp.end(), B.begin(), B.end());
                    tmp.insert(tmp.end(), D.begin(), D.end());
                    return tmp;
                }(A, B, C, D)
            };

            for (const auto& candidate : variations) {
                // Calcula makespan
                vector<int> result(instance.machines[0].size() + 1, 0);
                for (auto& item : candidate) {
                    result[0] += instance.machines[item][0];
                    for (int m = 1; m < (int)instance.machines[0].size(); ++m) {
                        result[m] = max(result[m - 1], result[m]) + instance.machines[item][m];
                    }
                }
                int newFitness = result[result.size() - 2];
                if (newFitness < bestFitness) {
                    chromo = candidate;
                    bestFitness = newFitness;
                    break;
                }
            }
        }

        fitness[idv] = bestFitness;
    }
}


vector<int> Ag::rouletteSelection(){
    vector<int> parents (npop);

    // Calcula as probabilidades acumuladas
    vector<double> accumulated_fitness (npop + 1, 0.0);
    for(int i = 1; i < npop + 1; ++i){
        accumulated_fitness[i] = accumulated_fitness[i-1] + 1.0 / fitness[i];
    }

    uniform_real_distribution<> parent_distribution(accumulated_fitness[1],
                                                    accumulated_fitness[npop]);

    // Seleciona os pais de maneira aleatória
    for(auto &parent : parents){
        double r = parent_distribution(globalGenerator);

        // Busca binária pelo primeiro valor menor ou igual ao valor aleatório
        // gerado
        auto it_selected = lower_bound(accumulated_fitness.begin(),
                                       accumulated_fitness.end(), r);
        
        // O pai recebe o índice da posição encontrada pela busca binária
        parent = distance(accumulated_fitness.begin(), it_selected) - 1;
    }
    return parents;
}

vector<int> Ag::tournamentSelection(){
    vector<int> parents (npop);

    uniform_int_distribution<> parent_distribution(0, npop-1);
    uniform_real_distribution<> winner_distribution(0.0, 1.0);

    for(int i = 0; i < npop; ++i){
        int parent_1 = parent_distribution(globalGenerator);
        int parent_2;

        do {
            parent_2 = parent_distribution(globalGenerator);
        } while (parent_1 == parent_2);

        // Obtém a probabilidade do mais apto ser o vencedor
        float r = winner_distribution(globalGenerator);

        if(fitness[parent_1] > fitness[parent_2]){
            parents[i] = parent_2;
            if(r > pwinner){
                parents[i] = parent_1;
            }
        } else {
            parents[i] = parent_1;
            if(r > pwinner){
                parents[i] = parent_2;
            }
        }
    }
    return parents;
}

vector<int> Ag::parentsSelection(const int selection_id){
    return selection_id == 1 ? tournamentSelection() : rouletteSelection();
}

void Ag::crossoverACO(vector<int> &parents) {
    uniform_real_distribution<> pcrossover_distribution(0.0, 1.0);
    uniform_int_distribution<> start_dist(0, instance.n - 1);

    vector<int> candidates(instance.n);
    vector<int> weights(instance.n);

    for (int i = 0; i < npop - 1; i += 2) {
        float r = pcrossover_distribution(globalGenerator);
        int parent_1_idx = parents[i];
        int parent_2_idx = parents[i + 1];

        if (r > pcrossover) {
            copy_n(population[parent_1_idx].begin(), instance.n, intermediate_population[i].begin());
            copy_n(population[parent_2_idx].begin(), instance.n, intermediate_population[i + 1].begin());
            continue;
        }

        const vector<int> &p1 = population[parent_1_idx];
        const vector<int> &p2 = population[parent_2_idx];

        // Reutilize a matriz de feromônio local
        vector<vector<int>> pheromone(instance.n, vector<int>(instance.n, 10));
        for (int j = 0; j < instance.n - 1; ++j) {
            pheromone[p1[j]][p1[j + 1]] += 5;
            pheromone[p2[j]][p2[j + 1]] += 5;
        }

        for (int ant = 0; ant < 2; ++ant) {
            vector<int> &child = intermediate_population[i + ant];
            vector<bool> visited(instance.n, false);
            int current = start_dist(globalGenerator);

            child[0] = current;
            visited[current] = true;

            for (int step = 1; step < instance.n; ++step) {
                candidates.clear();
                weights.clear();

                for (int j = 0; j < instance.n; ++j) {
                    if (!visited[j]) {
                        candidates.push_back(j);
                        weights.push_back(pheromone[current][j]);
                    }
                }

                int next;
                if (!candidates.empty() && accumulate(weights.begin(), weights.end(), 0) > 0) {
                    discrete_distribution<int> dist(weights.begin(), weights.end());
                    next = candidates[dist(globalGenerator)];
                } else {
                    // Fallback direto ao primeiro não visitado
                    next = distance(visited.begin(), find(visited.begin(), visited.end(), false));
                }

                child[step] = next;
                visited[next] = true;
                current = next;
            }
        }
    }
}


void Ag::crossoverPMX(vector<int> &parents){
    uniform_real_distribution<> pcrossover_distribution(0.0, 1.0);
    uniform_int_distribution<> point_distribution(0, ngenes - 1);

    for(int i = 0; i < npop - 1; i += 2){
        float r = pcrossover_distribution(globalGenerator);

        int parent_1_idx = parents[i];
        int parent_2_idx = parents[i+1];

        if(r > pcrossover){
            // Copia os genes dos pais diretamente
            copy(population[parent_1_idx].begin(), population[parent_1_idx].end(),
                 intermediate_population[i].begin());
            copy(population[parent_2_idx].begin(), population[parent_2_idx].end(),
                 intermediate_population[i+1].begin());
            continue;
        }

        // "Renomeia" os indivíduos
        const vector<int> &parent_1 = population[parent_1_idx];
        const vector<int> &parent_2 = population[parent_2_idx];
        vector<int> &child_1 = intermediate_population[i];
        vector<int> &child_2 = intermediate_population[i+1];

        // Obtém os dois pontos de corte
        int point_1 = point_distribution(globalGenerator);
        int point_2 = point_distribution(globalGenerator);
        if(point_1 > point_2) swap(point_1, point_2);

        // Copia a seção entre os pontos de corte
        for(int j = point_1; j <= point_2; ++j){
            child_1[j] = parent_2[j];
            child_2[j] = parent_1[j];
        }

        // Cria os mapeamentos
        map<int, int> map_1, map_2;
        for(int j = point_1; j <= point_2; ++j){
            map_1[parent_2[j]] = parent_1[j];
            map_2[parent_1[j]] = parent_2[j];
        }

        // Preenche os genes antes do primeiro ponto de corte
        for(int j = 0; j < point_1; ++j){
            // Filho 1
            int gene = parent_1[j];
            while(map_1.find(gene) != map_1.end())
                gene = map_1[gene];
            child_1[j] = gene;

            // Filho 2
            gene = parent_2[j];
            while(map_2.find(gene) != map_2.end())
                gene = map_2[gene];
            child_2[j] = gene;
        }

        // Preenche os genes depois do segundo ponto de corte
        for(int j = point_2 + 1; j < ngenes; ++j){
            // Filho 1
            int gene = parent_1[j];
            while(map_1.find(gene) != map_1.end())
                gene = map_1[gene];
            child_1[j] = gene;

            // Filho 2
            gene = parent_2[j];
            while(map_2.find(gene) != map_2.end())
                gene = map_2[gene];
            child_2[j] = gene;
        }
    }
}

void Ag::crossoverOX(vector<int> &parents){
    uniform_real_distribution<> pcrossover_distribution(0.0, 1.0);
    uniform_int_distribution<> point_distribution(0, ngenes - 1);

    for(int i = 0; i < npop - 1; i+=2){
        float r = pcrossover_distribution(globalGenerator);

        int parent_1 = parents[i];
        int parent_2 = parents[i+1];

        if(r > pcrossover){
            // Copia os genes dos pais diretamente
            copy(population[parent_1].begin(), population[parent_1].end(),
                 intermediate_population[i].begin());
            copy(population[parent_2].begin(), population[parent_2].end(),
                 intermediate_population[i+1].begin());
            continue;
        }

        // Obtém os dois pontos de corte
        int point_1 = point_distribution(globalGenerator);
        int point_2 = point_distribution(globalGenerator);
        if(point_1 > point_2) swap(point_1, point_2);

        set<int> son_1, son_2;
        for(int j = point_1; j <= point_2; ++j){
            intermediate_population[i][j] = population[parent_2][j];
            son_1.insert(population[parent_2][j]);

            intermediate_population[i+1][j] = population[parent_1][j];
            son_2.insert(population[parent_1][j]);
        }

        int j = (point_2 + 1) % ngenes;
        int j1 = j, j2 = j;

        while(j != point_1){
            while(son_1.find(population[parent_1][j1]) != son_1.end()){
                son_1.erase(son_1.find(population[parent_1][j1]));
                j1 = (j1+1) % ngenes;
            }

            while(son_2.find(population[parent_2][j2]) != son_2.end()){
                son_2.erase(son_2.find(population[parent_2][j2]));
                j2 = (j2+1) % ngenes;
            }

            intermediate_population[i][j] = population[parent_1][j1];
            intermediate_population[i+1][j] = population[parent_2][j2];

            j  = (j+1)  % ngenes;
            j1 = (j1+1) % ngenes;
            j2 = (j2+1) % ngenes;
        }
    }
}

void Ag::crossover(vector<int> &parents, int crossover_id){
    // Seleciona qual maneira de crossover será utilizada pelo algoritmo  
    if(crossover_id == 1){
        crossoverOX(parents);
    } else if (crossover_id == 2){
        crossoverPMX(parents);
    } else if (crossover_id == 3){
        crossoverACO(parents);
    } else {
        cerr << "Crossover ID invalido\n";
        exit(1);
    }
}

void Ag::mutation(){
    uniform_real_distribution<> mutation_distribution(0.0, 1.0);
    uniform_int_distribution<> gene_distribution(0, ngenes - 1);

    // Lógica de mutação incremental quando o algoritmo fica preso em um
    // mínimo local
    auto it_local_min = min_element(fitness.begin(), fitness.end());
    if(*it_local_min < local_min){
        local_min = *it_local_min;
        extra_pmutation = 0.0;
        count_gen_local_min = 0;
    } else {
        count_gen_local_min += 1;
        if(count_gen_local_min >= 10){
            extra_pmutation = 0.01;
        }
    }

    for(int i = 0; i < npop; ++i){
        for(int j = 0; j < ngenes; ++j){
            float r = mutation_distribution(globalGenerator);

            if(r <= (pmutation + extra_pmutation)){
                int gene_idx;
                do {
                    gene_idx = gene_distribution(globalGenerator);
                } while (gene_idx == j);

                // Troca a posição de dois elementos do indivíduo
                // selecionado da população intermediária
                swap(intermediate_population[i][j],
                     intermediate_population[i][gene_idx]);
            }
        }
    }
}

void Ag::copyPopulation(){
    // Copia a população intermediária para a população original
    for(int i = 0; i < npop; ++i){
        copy(intermediate_population[i].begin(), intermediate_population[i].end(),
             population[i].begin());
    }
}

void Ag::elitism(){
    // Min heap
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;
    for(int i = 0; i < npop; ++i){
        min_heap.push(make_pair(fitness[i], i));
    }

    // Seleciona os nelite mais aptos para fazerem parte da nova população 
    for(int i = 0; i < nelite; ++i){
        pair<int, int> elite_idv = min_heap.top();
        min_heap.pop();

        copy(population[elite_idv.second].begin(),
             population[elite_idv.second].end(),
             intermediate_population[i].begin());
    }
}

// Imprime os resultados
void Ag::printResults(ofstream &output_file) {
    auto it_min_makespan = min_element(fitness.begin(), fitness.end());
    int min_makespan_id = distance(fitness.begin(), it_min_makespan);

    output_file << "Melhor Makespan: " << *it_min_makespan << endl;
    output_file << "Melhor Sequencia: " << population[min_makespan_id][0];
    for(int i = 1; i < ngenes; ++i){
        output_file << ' ' << population[min_makespan_id][i];
    }
    output_file << endl;
}

void Ag::printPopulation() const {
    for(int i = 0; i < npop; ++i){
        cout << "Percurso " << i+1 << ":" << endl;
        cout << "\tDistancia: " << fitness[i] << endl;
        cout << "\tCaminho: ";
        for(int j = 0; j < ngenes; ++j)
            cout << population[i][j] << " -> ";
        cout << population[i][0] << endl << endl;
    }
}

Ag::Ag(const Instance &_instance, int _npop, int _ngen, int _ngenes, int _nelite,
         double _pcrossover, double _pmutation, double _pwinner) :
    instance(_instance), npop(_npop), ngen(_ngen), ngenes(_ngenes), nelite(_nelite),
    pcrossover(_pcrossover), pmutation(_pmutation), local_min(INT_MAX),
    extra_pmutation(0.0), count_gen_local_min(0),
    population(_npop, vector<int> (_ngenes)),
    intermediate_population(_npop, vector<int> (_ngenes)), fitness(_npop),
    pwinner(_pwinner) {}

Ag::~Ag() {}
