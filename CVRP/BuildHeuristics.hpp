#ifndef BuildHeuristics_hpp
#define BuildHeuristics_hpp

#include <vector>

// Heuristicas de construcao (intrarota)
void buildNearestNeighbor(std::vector<int> &s, std::vector<int> &cl);
void buildCheapestInsertion(std::vector<int> &s, std::vector<int> &cl);
void buildGreedyRandom(std::vector<int> &s, std::vector<int> &cl, double alpha);
// Heuristica de construcao (global)
void buildRoutes();

#endif /* BuildHeuristics_hpp */
