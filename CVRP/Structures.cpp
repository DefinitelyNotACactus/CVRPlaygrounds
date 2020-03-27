#include "Structures.hpp"
#include "Instance.hpp"
#include "Util.hpp"

#include <iostream>

double solution::getCost(bool update) {
    if(update) {
        cost = 0;
        for(int i = 0; i < routes.size(); i++) {
            cost += routes[i].cost;
        }
    }
    
    return cost;
}

void solution::print() {
    std::cout << "Instancia: " << instance << std::endl;
    std::cout << "Seed: " << seed << std::endl;
    
    #ifdef DEBUG_VALUES
    int *points = new int[dimension + 1];
    double actual_total = 0, aux;
    for(int i = 0; i < dimension + 1; i++) {
        points[i] = 0;
    }
    #endif

    double total = 0;
    for(int i = 0; i < vehicles; i++) {
        std::cout << "Rota[" << i << "] = {";
        for(int j = 0; j < routes[i].order_of_visit.size() - 1; j++) {
            std::cout << routes[i].order_of_visit[j] << ", ";

            #ifdef DEBUG_VALUES
            points[routes[i].order_of_visit[j]]++;
            #endif
        }
        std::cout << routes[i].order_of_visit[(routes[i].order_of_visit.size() - 1)] << "}" << std::endl;
        std::cout << "Custo: " << routes[i].cost << std::endl;
        std::cout << "Demanda: " << routes[i].load << std::endl;

        #ifdef DEBUG_VALUES
        aux = getRouteCost(routes[i].order_of_visit);
        actual_total += aux;
        std::cout << "Custo (real): " << aux << std::endl;
        std::cout << "Demanda (real): " << getRouteLoad(routes[i].order_of_visit) << std::endl;
        std::cout << "Clientes[" << i << "] = {";
        for(int j = 0; j < routes[i].clients_set.size() - 1; j++) {
            std::cout << routes[i].clients_set[j] << ", ";
        }
        std::cout << routes[i].clients_set[(routes[i].clients_set.size() - 1)] << "}\n" << std::endl;
        #endif

        total += routes[i].cost;
    }
    cost = total;
    std::cout << "Custo total: " << cost << std::endl;

    #ifdef DEBUG_VALUES
    cost = actual_total;
    std::cout << "Custo total (real): " << cost << std::endl;
    for(int i = 1; i < dimension + 1; i++) {
        if(!points[i]) {
            std::cout << "Faltando: " << i << std::endl;
        } else if(i > 1 && points[i] > 1) {
            std::cout << i << " apareceu " << points[i] << " vezes!!" << std::endl;
        }
    }

    delete [] points;
    #endif
}

/* Distribuicao inicial dos clientes nas rotas */
void solution::initRoutes() {
    int* visited = new int[dimension];
    for(int i = 0; i < dimension; i++) {
        visited[i] = 0;
    }
    std::sort(clients + 1, clients + dimension); // Faz o sort dos clientes em ordem decrescente de demanda

    #ifdef DEBUG_VALUES
    for(int i = 0; i < dimension; i++) {
        std::cout << clients[i].demand << " ";
    }
    std::cout << std::endl;
    for(int i = 0; i < dimension; i++) {
        std::cout << clients[i].index << " ";
    }
    std::cout << std::endl;
    #endif

    for(int v = 0; v < vehicles; v++) {
        route r = route();
        r.load = 0;
        for(int i = 1; i < dimension; i++) {
            if(clients[i].demand + r.load <= capacity
               && !visited[clients[i].index]) {
                r.clients_set.push_back((clients[i].index + 1));
                r.load += clients[i].demand;
                visited[clients[i].index] = 1;
            }
        }
        if(r.clients_set.size() > 0) {
            routes.push_back(r);
        }
    }

    #ifdef DEBUG_VALUES
    for(int i = 1; i < dimension; i++) {
        if(!visited[i]) {
            std::cout << i + 1 << " nao inserido" << std::endl;
        }
    }
    #endif
}

void route_pool::add(std::vector<int> &route, double cost) {
    std::string r = "";
    for(int j = 0; j < route.size() - 1; j++) {
       r = r + std::to_string(route[j]);
       r = r + " ";
    }
    r = r + std::to_string(route[route.size() - 1]);
    pool[r] = getRouteCost(route);
}

void route_pool::add(std::vector<route> &routes) {
    for(int i = 0; i  < routes.size(); i++) {
        std::string r = "";
        add(routes[i].order_of_visit, routes[i].cost);
    }
}

void route_pool::print(std::ostream &out) {
    out << "Vehicles: " << vehicles << std::endl;
    out << "Clients: " << dimension << std::endl;
    out << "Pool_size: " << pool.size() << std::endl;
    for(auto r : pool) {
        out << "Route: " << r.first << std::endl;
        out << "Cost: " << r.second << std::endl;
    }
    out << "END" << std::endl;
}
