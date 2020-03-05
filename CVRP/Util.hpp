#ifndef Util_hpp
#define Util_hpp

// Util
void readInstance(std::string &instance);
void printData();
void printSolution();

void fixVector(std::vector<int> &vec);
void buildPool();
void addToPool(std::vector<int> &route, double cost);
void printPool();

double getCost(std::vector<int> &s);
double getDemand(std::vector<int> &s);

#endif /* Util_hpp */
