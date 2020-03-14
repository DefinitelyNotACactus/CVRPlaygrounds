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
    //printData();
    solution s = solution();
    route_pool pool = route_pool();
    s.initRoutes();
    interILS(s, 20);
    s.print();
//    for(int i = 0; i < s.routes.size(); i++) {
//        buildNearestNeighbor(s.routes[i].order_of_visit, s.routes[i].clients_set);
//        s.routes[i].cost = getRouteCost(s.routes[i].order_of_visit);
//        pool.add(s.routes[i].order_of_visit, s.routes[i].cost);
//    }
//    s.print();
//    for(int i = 0; i < 20; i++) {
//        ejectionChain(s.routes);
//    }
//    s.print();
//    s.print();
//    cout << "\n" << endl;
//    for(int i = 0; i < 3; i++) {
//        interRvnd(s.routes);
//        interGils_rvnd(routes, 10, 5, 1.0);
//        for(int j = 0; j < s.routes.size(); j++) {
//            pool.add(s.routes[j].order_of_visit, s.routes[j].cost);
//            //fixVector(routes[j].order_of_visit);
//            gils_rvnd(s.routes[j], 3, 5, 2, 0.7);
//            //fixVector(routes[j].order_of_visit);
//            for(int k = 0; k < 5; k++) {
//                rvnd(s.routes[j].order_of_visit, s.routes[j].cost);
//                pool.add(s.routes[j].order_of_visit, s.routes[j].cost);
//                vector<int> st;
//                double sCost;
//                if(s.routes[j].order_of_visit.size() > 4) {
//                    doubleBridge(s.routes[j].order_of_visit, st, s.routes[j].cost, sCost);
//                    s.routes[j].order_of_visit = st;
//                    s.routes[j].cost = getCost(s.routes[j].order_of_visit);
//                }
//            }
//        }
//    }
    //pool.print();
    //interGils_rvnd(s, 5, 3, 1.0);
//    for(int i = 0; i < s.routes.size(); i++) {
//        gils_rvnd(s.routes[i], 3, 10, 5, 0.7);
//    }
//    s.print();
    //cout << "\n" << endl;

    //cout << getCost(routes[0].s) << " " << getCost(routes[1].s) << endl;
    //cout << getDemand(routes[0].s) << " " << getDemand(routes[1].s) << endl;

    //buildPool();
    deallocateInstance();
    
    return 0;
}
