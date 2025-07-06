#ifndef IO_HPP
#define IO_HPP

#include <bits/stdc++.h>

using namespace std;

class Io{
    public:
        string getInputFile(const int argc, char **argv);
        string getFacOutputFile() const;
        string getGenOutputFile() const;
        int getSolutionId(const int argc, char **argv);
        int getCrossoverId(const int argc, char **argv);
        int getSelectionId(const int argc, char **argv);
};

#endif
