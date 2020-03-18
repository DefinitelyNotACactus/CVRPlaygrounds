#include "ILS.hpp"
#include "Structures.hpp"
#include "BuildHeuristics.hpp"
#include "LocalSearch.hpp"
#include "Util.hpp"
#include "Instance.hpp"
#include "Pertubation.hpp"

#include <iostream>

/* Metaheuristica interrota */
void ILS(int max_itr_ils, double alpha, bool show) {
    solution best_solution;
    // Gerar solução inicial
    for(int i = 0; i < s.routes.size(); i++) {
        //buildNearestNeighbor(s.routes[i].order_of_visit, s.routes[i].clients_set); // TODO: Adicionar construção aleatória e gulosa
        buildGreedyRandom(s.routes[i].order_of_visit, s.routes[i].clients_set, alpha);
        s.routes[i].cost = getRouteCost(s.routes[i].order_of_visit);
        s.cost += s.routes[i].cost;
    }
    if (show) {
        std::cout << "Custo incial (Após construção): " << s.cost << std::endl;
    }
    // Busca local
    interRvnd(s.routes);
    for(int i = 0; i < s.routes.size(); i++) {
        rvnd(s.routes[i].order_of_visit, s.routes[i].cost);
    }
    
    pool.add(s.routes); // Adicionar solução inicial ao pool
    if (show) {
        std::cout << "Custo incial (Após rvnd): " << s.cost << std::endl;
    }
    
    best_solution = s;
    solution st = s;
    
    int itr_ils = 0, total_iterations = 0;
    while(itr_ils < max_itr_ils) { // Critério de parada
        // Perturbação
        ejectionChain(st.routes);
        st.getCost(true);
        // Busca local
        interRvnd(st.routes);
        for(int i = 0; i < st.routes.size(); i++) {
            rvnd(st.routes[i].order_of_visit, st.routes[i].cost);
        }
        
        pool.add(st.routes); // Adicionar ao pool
        
        itr_ils++;
        total_iterations++;
        // Criterio de aceitacao
        if(st.getCost(false) < best_solution.getCost(false)) {
            best_solution = st;
            itr_ils = 0;
            if (show) {
                std::cout << "Novo custo: " << best_solution.cost << std::endl;
            }
        }
    }
    
    if (show) {
        std::cout << "Custo final: " << best_solution.cost << std::endl;
        std::cout << "Iterações realizadas: " << total_iterations << std::endl;
    }
    
    s = best_solution;
}
