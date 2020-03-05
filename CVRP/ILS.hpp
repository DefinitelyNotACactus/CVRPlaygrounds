#ifndef ILS_hpp
#define ILS_hpp

#include "Structures.hpp"

using namespace std;
// Perturbacao (intrarota)
void doubleBridge(vector<int> &s, vector<int> &st, double &cost, double &sCost);
// Pertubacao (interrota)
void ejectionChain(vector<route> &rl); //TODO
// Metaheuristica (intrarota)
void gils_rvnd(route &r, int initialSubtour, int itr_max, int ils, double alpha);
// Metaheuristica (interrota)
void interGils_rvnd(vector<route> &rl, int itr_max, int ils, double alpha); //TODO

#endif /* ILS_hpp */
