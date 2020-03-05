#include "LocalSearch.hpp"
#include "Neighbors.hpp"

using namespace std;
/* RVND intrarota */
void rvnd(vector<int> &s, double &cost) {
    vector<int> nl = {0, 1, 2, 3, 4};
    double tempCost = cost;
    
    int n = 0;
    bool improve = false;
    while(!nl.empty()) {
        n = rand() % nl.size();
        switch(nl.at(n)) {
            case 0: // swap
                improve = swap(s, tempCost);
                break;
            case 1: // 2-opt
                improve = twoOpt(s, tempCost);
                break;
            case 2: // reinsercao 1
                improve = reinsertion(s, 1, tempCost);
                break;
            case 3: // reinsercao 2
                improve = reinsertion(s, 2, tempCost);
                break;
            case 4: // reinsercao 3
                improve = reinsertion(s, 3, tempCost);
                break;
        }
        if(improve) {
            nl = {0, 1, 2, 3, 4};
        } else {
            nl.erase(nl.begin() + n);
        }
    }

    cost = tempCost;
}

/* RVND interrota */
void interRvnd(vector<route> &rl) {
    vector<int> nl = {0, 1};
    int n = 0;
    bool improve = false;
    while(!nl.empty()) {
        n = rand() % nl.size();
        switch(nl.at(n)) {
            case 0: // swap
                improve = interSwap(rl);
                break;
            case 1: // reinsertion
                improve = interReinsertion(rl);
                break;
        }
        if(improve) {
            nl = {0, 1};
        } else {
            nl.erase(nl.begin() + n);
        }
    }
}
