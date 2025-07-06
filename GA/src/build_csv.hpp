#ifndef BUILD_CSV_HPP
#define BUILD_CSV_HPP

#include "ag.hpp"

#include <bits/stdc++.h>

using namespace std;

class BuildCSV{
    public:
        void printGenIdfCSV(Ag *ag, ofstream &gen_file);
        void printFacIdfCSV(ofstream &fac_file);

        void printGenResultsCSV(Ag *ag, ofstream &gen_file, const int gen);
        void printFacResultsCSV(Ag *ag, ofstream &fac_file);
};

#endif
