#ifndef INSTANCE_H
#define INSTANCE_H

#include<fstream>

#include <bits/stdc++.h>

using namespace std;

class Instance{
    public:
        int n; // Número de itens
        int m; // Número de máquinas

        vector<vector<int>> machines;

        void readInstance(ifstream &input_file);

        Instance();
};

#endif
