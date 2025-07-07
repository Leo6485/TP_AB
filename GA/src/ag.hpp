#ifndef AG_HPP
#define AG_HPP

#include "instance.hpp"

class Ag{
    public:
        Instance instance;

        int npop;
        int ngen;
        int ngenes;
        int nelite;
        double pcrossover;
        double pmutation;

        double local_min;
        double extra_pmutation;
        int count_gen_local_min;

        vector<vector<int>> population;
        vector<vector<int>> intermediate_population;
        vector<int> fitness;

        double pwinner;

        void initPopulation();
        void evaluatePopulation();
        int evaluateIndividual(const vector<int> &sequence);
        void twoOpt();
        void threeOpt();
        vector<int> parentsSelection(const int selection_id);
        vector<int> tournamentSelection();
        vector<int> rouletteSelection();

        void crossoverACO(vector<int> &parents);
        void crossoverPMX(vector<int> &parents);
        void crossoverOX(vector<int> &parents);
        void crossover(vector<int> &parents, int crossover_id);
        void mutation();
        void copyPopulation();
        void elitism();
        void printResults(ofstream &output_file);
        void printPopulation() const;

        Ag(const Instance &_instance, int _npop, int _ngen, int _ngenes, int _nelite,
            double _pcrossover, double _pmutation, double _pwinner);
        ~Ag();
};

#endif
