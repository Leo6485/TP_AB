#ifndef SOLVE_H
#define SOLVE_H

#include <bits/stdc++.h>

#include "instance.hpp"

using namespace std;

class Solve{
    private:
        void solveFactorialTest(Instance &instance, int npop, int ngen, int ngenes,
                                int nelite, double pcrossover, double pwinner,
                                double pmutation, ofstream &output_file,
                                const int crossover_id, const int selection_id);
        void solveGenerationsTest(Instance &instance, int npop, int ngen, int ngenes,
                                  int nelite, double pcrossover, double pwinner,
                                  double pmutation, ofstream &output_file,
                                  const int crossover_id, const int selection_id);
    public:
        void solve(ifstream &input_file_name, const int crossover_id,
                   const int selection_id, ofstream &output_file);
        void factorialTest(ifstream &input_file, ofstream &output_file,
                           const int crossover_id, const int selection_id);
        void generationsTest(ifstream &input_file, ofstream &output_file,
                             const int crossover_id, const int selection_id);
};

#endif
