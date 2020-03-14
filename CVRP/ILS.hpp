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
void interGils_rvnd(solution &s, int itr_max, int ils, double alpha); //TODO

void ILS(route &route, int max_itr_ils);
void interILS(solution &solution, int max_itr_ils);

#endif /* ILS_hpp */
