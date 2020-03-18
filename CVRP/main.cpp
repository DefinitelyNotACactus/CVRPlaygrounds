#ifndef __APPLE__
#include <bits/stdc++.h>
#endif

#include "Structures.hpp"
#include "Neighbors.hpp"
#include "BuildHeuristics.hpp"
#include "LocalSearch.hpp"
#include "Util.hpp"
#include "ILS.hpp"
#include "Instance.hpp"
#include "Pertubation.hpp"

#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    instance = argv[1];
    srand(time(nullptr));
    readInstance(instance);
    s = solution();
    pool = route_pool();
    s.initRoutes();
    ILS(30, 0.5, false);
    pool.print();
    
    deallocateInstance();
    return 0;
}
