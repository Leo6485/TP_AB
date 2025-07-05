#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <bits/stdc++.h>

using namespace std;

class Graph{
    private:
        vector<vector<double>> G;
        int n, m;
    public:
        void initGraph(const int nrows, const int ncolumns, const double initial_value);
        void resetGraph(const double reset_value);
        void readGraph(ifstream &input_file);
        double getEdge(const int src, const int dest) const;
        void setEdge(const int src, const int dest, const double cost);
        void incrementEdge(const int src, const int dest, const double increment);
        int getNumRows() const;
        int getNumColumns() const;

        Graph();
        Graph(const int nrows, const int ncolumns);
};

#endif