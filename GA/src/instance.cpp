#include "instance.hpp"
#include <bits/stdc++.h>

Instance::Instance(){}

void Instance::readInstance(ifstream &input_file){
    // Lê e armazena o conteúdo do arquivo de entrada
    input_file >> n >> m;

    machines.assign(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int w; input_file >> w;
            machines[i][j] = w;
        }
    }
}