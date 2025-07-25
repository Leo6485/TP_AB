#include <bits/stdc++.h>

#include "instance.hpp"
#include "ag.hpp"

using namespace std;

// Gerador de números aleatórios global
mt19937 globalGenerator(random_device{}());

// Cria uma população inicial com individuos bons utilizando heurística e parte aleatória
void Ag::initPopulation(){
    // 10% dos indivíduos são inicializados de acordo com a heurística, enquanto os demais são inicializados de forma aleatória
    int heuristic_amount = npop * 0.1;

    int n = instance.n;
  
    // Calcula a soma do total dos tempos de cada uma das tarefas
    vector<pair<int, int>> tasks; // pair {soma, idx}
    tasks.reserve(n);
    for(int i = 0; i < n; ++i){
        vector<int> &machine = instance.machines[i];
        int sum = accumulate(machine.begin(), machine.end(), 0);
        tasks.push_back(make_pair(sum, i));
    }

    // Ordena em ordem decrescente as tarefas
    sort(tasks.begin(), tasks.end(), greater<>());
  
    for(int idv = 0; idv < heuristic_amount; ++idv){
        vector<int> sequence;

        int sequence_size = sequence.size();

        for (int i = 0; i < n; ++i){
            int task_idx = tasks[i].second;

            vector<pair<int, vector<int>>> candidates; // {makespan, sequencia}
            candidates.reserve(sequence_size + 1);

            // Testa todas as posições para inserir a tarefa atual
            for (int j = 0; j <= sequence_size; ++j)
            {
                vector<int> new_sequence = sequence;

                // Insere a tarefa na posição j
                new_sequence.insert(new_sequence.begin() + j, task_idx);

                int makespan = evaluateIndividual(new_sequence);

                candidates.push_back(make_pair(makespan, new_sequence));
            }

            // Ordena os candidados em ordem decrescente do seu makespan
            sort(candidates.begin(), candidates.end());

            // Escolhe um dos melhores candidatos
            int nbest = sqrt(sequence_size + 1);
            uniform_int_distribution<> dist(0, nbest - 1);
            int chosen = dist(globalGenerator);

            sequence = candidates[chosen].second;
        }

        // Aplica mutação para diversificar os indivíduos iniciais
        uniform_int_distribution<> gene_distribution(0, ngenes - 1);
        uniform_real_distribution<> mutation_distribution(0.0, 1.0);
        for(int j = 0; j < (int) sequence.size(); ++j){
            float r = mutation_distribution(globalGenerator);

            // Aplica taxa de mutação alta para gerar diversidade
            if(r <= 0.25){
                int gene_idx;
                do{
                    gene_idx = gene_distribution(globalGenerator);
                } while(gene_idx == j);

                // Troca duas tarefas de posição
                swap(sequence[j], sequence[gene_idx]);
            }
        }

        population[idv] = sequence;
    }
     
    // Inicializa o restante da população inicial de forma aleatória
    for(int idv = heuristic_amount + 1; idv < npop; ++idv){
        vector<int>& individual = population[idv];
        iota(individual.begin(), individual.end(), 0);
        shuffle(individual.begin(), individual.end(), globalGenerator);
    }
    
}

// Calcula o makespan de apenas uma sequência
int Ag::evaluateIndividual(const vector<int> &sequence){
    int m = instance.m;
    vector<int> result(m, 0);

    for (auto &item : sequence){
        result[0] = result[0] + instance.machines[item][0];
        for (int i = 1; i < m; i++){
            result[i] = max((result[i - 1]), result[i]) + instance.machines[item][i];
        }
    }

    return result[m - 1];
}

void Ag::evaluatePopulation(){
    for(int idv = 0; idv < npop; ++idv){ 
        fitness[idv] = evaluateIndividual(population[idv]);
    }
}

void Ag::threeOpt() { 
    uniform_int_distribution<int> dist(0, instance.n - 1);

    for (int idv = 0; idv < npop; ++idv) {
        vector<int>& chromo = population[idv];

        // Ocasionalmente força a busca mesmo que ela não provoque melhora
        int best_fitness = (uniform_int_distribution<int>(0, 4)
                           (globalGenerator) == 0) ? INFINITY : fitness[idv];
        vector<int> candidate = chromo;

        for (int t = 0; t < 1000; ++t) {
            // Sorteia 3 posições distintas
            int i = dist(globalGenerator), j = dist(globalGenerator);
            int k = dist(globalGenerator);
            if (i == j || j == k || i == k) continue;

            // Ordena i < j < k
            if (i > j) swap(i, j);
            if (j > k) swap(j, k);
            if (i > j) swap(i, j);

            bool improved = false;

            // Testa até 3 variações de 3-opt entre [i,j,k]
            for (int variant = 0; variant < 3; ++variant) {
                candidate = chromo;

                switch (variant) {
                    // Reversão nos dois blocos e depois no geral
                    case 0: 
                        reverse(candidate.begin() + i, candidate.begin() + j);
                        reverse(candidate.begin() + j, candidate.begin() + k);
                        reverse(candidate.begin() + i, candidate.begin() + k);
                        break;
                    // Variante 1: inverte os dois blocos separadamente
                    case 1: 
                        reverse(candidate.begin() + i, candidate.begin() + j);
                        reverse(candidate.begin() + j, candidate.begin() + k);
                        break;
                    // Variante 2: inverte só o segundo bloco
                    case 2: 
                        reverse(candidate.begin() + j, candidate.begin() + k);
                        break;
                }

                // Recalcula o makespan
                int new_fitness = evaluateIndividual(candidate);

                // Se for melhor, atualiza o cromossomo e o fitness
                if (new_fitness < best_fitness) {
                    chromo = candidate;
                    best_fitness = new_fitness;
                    improved = true;
                    break; 
                }
            }

            if (!improved) continue;
        }

        // Atualiza o fitness final do indivíduo
        fitness[idv] = best_fitness;
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

vector<int> Ag::tournamentSelection() {
    int n_cidades = 5;
    int idv_per_city = npop / n_cidades; // Calcula o número de indivíduos por cidade

    vector<int> parents(npop);
    uniform_int_distribution<> parent_city_distribution(0, idv_per_city - 1);
    uniform_real_distribution<> winner_distribution(0.0, 1.0);

    for (int i = 0; i < npop; ++i) {
        // Calcula a cidade que o indivíduo pertence
        int cidade = (i * n_cidades) / npop;

        // Seleciona indivíduos diferentes da mesma cidade
        int parent_1 = parent_city_distribution(globalGenerator) + cidade * idv_per_city;
        int parent_2;
        do {
            parent_2 = parent_city_distribution(globalGenerator) + cidade * idv_per_city;
        } while (parent_1 == parent_2);

        float r = winner_distribution(globalGenerator);

        // Seleciona o melhor, se r <= pwinner, e o pior, caso contrário
        if (fitness[parent_1] > fitness[parent_2]) {
            parents[i] = (r > pwinner) ? parent_1 : parent_2;
        } else {
            parents[i] = (r > pwinner) ? parent_2 : parent_1;
        }
    }

    return parents;
}

// Seleciona qual das estratégias de seleção de pais será utilizada
vector<int> Ag::parentsSelection(const int selection_id){
    return selection_id == 1 ? tournamentSelection() : rouletteSelection();
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

        // Se não ocorrer crossover, copia os genes dos pais diretamente
        if(r > pcrossover){
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

        // Preenchimento circular dos demais genes (fora do segmento)
        int j = (point_2 + 1) % ngenes;
        int j1 = j, j2 = j;

        while(j != point_1){
            // Avança j1 até encontrar um gene do pai 1 que ainda não foi usado no filho 1
            while(son_1.find(population[parent_1][j1]) != son_1.end()){
                son_1.erase(son_1.find(population[parent_1][j1]));
                j1 = (j1+1) % ngenes;
            }

            // Avança j2 até encontrar um gene do pai 2 que ainda não foi usado no filho 2
            while(son_2.find(population[parent_2][j2]) != son_2.end()){
                son_2.erase(son_2.find(population[parent_2][j2]));
                j2 = (j2+1) % ngenes;
            }

            // Preenche a posição j com os genes não repetidos
            intermediate_population[i][j] = population[parent_1][j1];
            intermediate_population[i+1][j] = population[parent_2][j2];

            // Avança os índices de forma circular
            j  = (j+1)  % ngenes;
            j1 = (j1+1) % ngenes;
            j2 = (j2+1) % ngenes;
        }
    }
}

void Ag::crossover(vector<int> &parents, int crossover_id){
    // Seleciona qual maneira de crossover será utilizada pelo algoritmo  
    if(crossover_id == 1){
        crossoverPMX(parents); 
    } else if (crossover_id == 2){ 
        crossoverOX(parents);
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

// Imprime os resultados do algoritmo de acordo com as especificações
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

// Função para depuração dos indivíduos
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
