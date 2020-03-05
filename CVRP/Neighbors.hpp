#ifndef Neighbors_hpp
#define Neighbors_hpp

#include "Structures.hpp"

// Movimentos de vizinhanca (intrarota)
bool swap(std::vector<int> &st, double &cost);
bool twoOpt(std::vector<int> &st, double &cost);
bool reinsertion(std::vector<int> &st, int subtourSize, double &cost);
// Movimentos de vizinhanca (interrota)
bool interSwap(std::vector<route> &rl);
bool interReinsertion(std::vector<route> &rl);

#endif /* Neighbors_hpp */
