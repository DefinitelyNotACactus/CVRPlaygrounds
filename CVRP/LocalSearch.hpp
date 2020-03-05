#ifndef LocalSearch_hpp
#define LocalSearch_hpp

#include "Structures.hpp"

// Busca local (intrarota)
void rvnd(std::vector<int> &s, double &cost);
// Busca local (interrota)
void interRvnd(std::vector<route> &rl);

#endif /* LocalSearch_hpp */
