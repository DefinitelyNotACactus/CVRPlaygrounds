#ifndef Pertubation_hpp
#define Pertubation_hpp

#include "Structures.hpp"

#include <vector>

/* Pertubacao intrarota */
void doubleBridge(std::vector<int> &s, std::vector<int> &st, double &cost, double &sCost);
/* Pertubacao interrota */
void ejectionChain(std::vector<route> &rl);

#endif /* Pertubation_hpp */
