#include <vector>

#include "Instance.hpp"

double getRouteLoad(std::vector<int> &s) {
    double d = 0;
    for(int i = 0; i < s.size() - 1; i++) {
        d += demands[s[i]];
    }
    return d;
}

double getRouteCost(std::vector<int> &s) {
   double c = 0;
    for(int i = 0; i < s.size() - 1; i++) {
        c += matrizAdj[s.at(i)][s.at(i+1)];
    }
    return c;
}
