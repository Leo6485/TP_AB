#ifndef SOLVE_HPP
#define SOLVE_HPP

#include "graph.hpp"
#include "aco.hpp"

#include <bits/stdc++.h>

using namespace std;

class Solve{
    private:
        void solveFactorialTest(const Graph *graph, const int npop, const int ngen, const double Q, const double alfa, const double beta, const double p, ofstream &output_file);
        void solveGenerationsTest(const Graph *graph, const int npop, const int ngen, const double Q, const double alfa, const double beta, const double p, ofstream &output_file);
    public:
        void solve(ifstream &input_file, ofstream &output_file);
        void factorialTest(ifstream &input_file, ofstream &output_file);
        void generationsTest(ifstream &input_file, ofstream &output_file);
};

#endif