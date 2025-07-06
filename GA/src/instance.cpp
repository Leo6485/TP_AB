#include "instance.hpp"
#include <bits/stdc++.h>

Instance::Instance(){}

void Instance::readInstance(ifstream &input_file){
    input_file >> n >> m;

    machines.assign(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int w; input_file >> w;
            machines[i][j] = w;
        }
    }

    // G.assign(n, vector<int>(n, 0));
}

// int Instance::getEdge(int src, int dst) const {
    // return G[src][dst];
// }