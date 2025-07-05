#include "utils.hpp"

double utils::pow(double base, int exponent){
    if(utils::isInteger(base)){
        double result = 1.0;
        while(exponent > 0){
            if(exponent & 1){
                result *= base;
            }
            base *= base;
            exponent >>= 1;
        }
        return result;
    } 
    return std::pow(base, exponent);
}

bool utils::isInteger(double value){
    double integer_part;
    double fractional_part = modf(value, &integer_part);
    return fractional_part == 0.0;
}