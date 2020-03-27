#ifndef ILS_hpp
#define ILS_hpp

#include "Structures.hpp"

/* Iterated local search */
void ILS(int max_itr_ils, double alpha, bool show);
/* Iterated local search utilizando várias construções */
void ILSv2(int max_itr_ils, int builds, double alpha, bool show);
#endif /* ILS_hpp */
