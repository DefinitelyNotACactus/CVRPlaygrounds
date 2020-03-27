#ifndef Pertubation_hpp
#define Pertubation_hpp

#include "Structures.hpp"

#include <vector>

/* Perturbação intrarrota */
void randomSwap(std::vector<int> &s, double &cost);
/* Perturbação interrota */
void ejectionChain(std::vector<route> &rl);

#endif /* Pertubation_hpp */
