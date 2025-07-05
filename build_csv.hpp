#ifndef BUILD_CSV_HPP
#define BUILD_CSV_HPP

#include "aco.hpp"

#include <bits/stdc++.h>

using namespace std;

class BuildCSV{
    public:
        void printGenIdfCSV(const Aco *aco, ofstream &gen_file);
        void printFacIdfCSV(ofstream &fac_file);

        void printFacResultsCSV(Aco *aco, ofstream &fac_file);
};

#endif