#include "graph.hpp"

#define dbg(x) cout << #x << " == " << x << endl

Graph::Graph() {}
Graph::Graph(const int nrows, const int ncolumns) :
    G(nrows, vector<double> (ncolumns)), n(nrows), m(ncolumns) {}

void Graph::initGraph(const int nrows, const int ncolumns, const double initial_value){
    n = nrows;
    m = ncolumns;

    G.assign(n, vector<double> (m, 0.0));
}

void Graph::resetGraph(const double reset_value){
    for(auto &row : G)
        fill(row.begin(), row.end(), reset_value);
}

void Graph::readGraph(ifstream &input_file){
    input_file >> n >> m;

    for(int i = 0; i < n; ++i){
        vector<double> row;
        for(int j = 0; j < m; ++j){
            double cost; input_file >> cost;
            row.push_back(cost);
        }
        G.push_back(row);
    }
}

double Graph::getEdge(const int src, const int dest) const{
    if(src < 0 or dest < 0 or src > n or dest > m){
        dbg(src), dbg(dest); return 1;
    }
    return G[src][dest];
}

void Graph::setEdge(const int src, const int dest, const double cost){
    G[src][dest] = cost;
}

void Graph::incrementEdge(const int src, const int dest, const double increment){
    G[src][dest] += increment;
}

int Graph::getNumRows() const{
    return n;
}

int Graph::getNumColumns() const{
    return m;
}