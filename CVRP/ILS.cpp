#include "ILS.hpp"
#include "Structures.hpp"
#include "BuildHeuristics.hpp"
#include "LocalSearch.hpp"
#include "Util.hpp"
#include "Instance.hpp"
#include "Pertubation.hpp"

/* Metaheuristica interrota */
void interILS(solution &s, int max_itr_ils) {
    solution best_solution;
    // Gerar solução inicial
    for(int i = 0; i < s.routes.size(); i++) {
        buildNearestNeighbor(s.routes[i].order_of_visit, s.routes[i].clients_set); // TODO: Adicionar construção aleatória e gulosa
        s.routes[i].cost = getRouteCost(s.routes[i].order_of_visit);
        s.cost += s.routes[i].cost;
    }
    // Busca local
    interRvnd(s.routes);
    for(int i = 0; i < s.routes.size(); i++) {
        rvnd(s.routes[i].order_of_visit, s.routes[i].cost);
    }
    
    best_solution = s;
    solution st = s;
    
    int itr_ils = 0;
    while(itr_ils < max_itr_ils) { // Critério de parada
        // Perturbação
        ejectionChain(st.routes);
        st.getCost(true);
        // Busca local
        interRvnd(st.routes);
        for(int i = 0; i < st.routes.size(); i++) {
            rvnd(st.routes[i].order_of_visit, st.routes[i].cost);
        }
        itr_ils++;
        // Criterio de aceitacao
        if(st.getCost(false) < best_solution.getCost(false)) {
            best_solution = st;
            itr_ils = 0;
        }
    }
    
    s = best_solution;
}
