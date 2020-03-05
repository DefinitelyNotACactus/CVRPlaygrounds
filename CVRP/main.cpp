#include <fstream>
#include <iostream>
#include <vector>
#include <cfloat>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>

#ifndef __APPLE__
#include <bits/stdc++.h>
#endif

#include "Structures.hpp"
#include "Neighbors.hpp"
#include "BuildHeuristics.hpp"
#include "LocalSearch.hpp"
#include "Util.hpp"
#include "ILS.hpp"

using namespace std;

//Variaveis
string instance;
double ** matrizAdj; // matriz de adjacencia
int *demands; // vetor de demandas
int dimension, vehicles, actual_vehicles, capacity; // quantidade total de vertices, veiculos e capacidade dos veiculos
double currentCost; // custo da solucao atual
vector<route> routes;
unordered_map<string, int> pool;
client *clients;

void deallocate() {
    for(int i = 0; i <= dimension; i++) {
        delete [] matrizAdj[i];
    }
    delete [] matrizAdj;
    delete [] demands;
    routes.clear();
}

//#ifndef DEBUG_VALUES
//#define DEBUG_VALUES
//#endif

int main(int argc, char** argv) {
    instance = argv[1];
    srand(time(nullptr));
    readInstance(instance);
    //printData();
    buildRoutes();
    for(int i = 0; i < actual_vehicles; i++) {
        buildNearestNeighbor(routes[i].order_of_visit, routes[i].clients_set);
        routes[i].cost = getCost(routes[i].order_of_visit);
        addToPool(routes[i].order_of_visit, routes[i].cost);
    }
    //printSolution();
    //cout << "\n" << endl;
    for(int i = 0; i < 3; i++) {
        interRvnd(routes);
        //interGils_rvnd(routes, 10, 5, 1.0);
        for(int j = 0; j < routes.size(); j++) {
            addToPool(routes[j].order_of_visit, routes[j].cost);
            //fixVector(routes[j].order_of_visit);
            //gils_rvnd(routes[j], 3, 5, 2, 0.7);
            //fixVector(routes[j].order_of_visit);
            for(int k = 0; k < 5; k++) {
                rvnd(routes[j].order_of_visit, routes[j].cost);
                addToPool(routes[j].order_of_visit, routes[j].cost);
                vector<int> st;
                double sCost;
                if(routes[j].order_of_visit.size() > 4) {
                    doubleBridge(routes[j].order_of_visit, st, routes[j].cost, sCost);
                    routes[j].order_of_visit = st;
                    routes[j].cost = getCost(routes[j].order_of_visit);
                }
            }
        }
    }
    printPool();
    //interGils_rvnd(routes, 10, 5, 1.0);
    //printSolution();
    //cout << "\n" << endl;

    //cout << getCost(routes[0].s) << " " << getCost(routes[1].s) << endl;
    //cout << getDemand(routes[0].s) << " " << getDemand(routes[1].s) << endl;

    //buildPool();
    deallocate();
    
    return 0;
}

/* Construcao inicial das rotas */
void buildRoutes() {
    int* visited = new int[dimension];
    for(int i = 0; i < dimension; i++) {
        visited[i] = 0;
    }
    sort(clients + 1, clients + dimension); // Faz o sort dos clientes em ordem decrescente de demanda (overloading do < em clients para fazer >)

    #ifdef DEBUG_VALUES
    for(int i = 0; i < dimension; i++) {
        cout << clients[i].demand << " ";
    }
    cout << endl;
    for(int i = 0; i < dimension; i++) {
        cout << clients[i].index << " ";
    }
    cout << endl;
    #endif

    actual_vehicles = 0;
    for(int v = 0; v < vehicles; v++) {
        routes[v].load = 0;
        for(int i = 1; i < dimension; i++) {
            if(clients[i].demand + routes[v].load <= capacity
               && !visited[clients[i].index]) {
                routes[v].clients_set.push_back((clients[i].index + 1));
                routes[v].load += clients[i].demand;
                visited[clients[i].index] = 1;
            }
        }
        if(routes[v].clients_set.size() > 0) {
            actual_vehicles++;
        }
    }

    #ifdef DEBUG_VALUES
    for(int i = 1; i < dimension; i++) {
        if(!visited[i]) {
            cout << i + 1 << " nao inserido" << endl;
        }
    }
    #endif

    delete [] clients;
}


double getDemand(vector<int> &s) {
    double d = 0;
    for(int i = 0; i < s.size() - 1; i++) {
        d += demands[s[i]];
    }
    return d;
}

double getCost(vector<int> &s) {
   double c = 0;
    for(int i = 0; i < s.size() - 1; i++) {
        c += matrizAdj[s.at(i)][s.at(i+1)];
    }
    return c;
}

void printData() {
  cout << "dimension: " << dimension << endl;
  for (size_t i = 1; i <= dimension; i++) {
    for (size_t j = 1; j <= dimension; j++) {
      cout << matrizAdj[i][j] << " ";
    }
    cout << endl;
  }
}

void printSolution() {
    cout << "Instancia: " << instance << endl;

    #ifdef DEBUG_VALUES
    int *points = new int[dimension + 1];
    double actual_total = 0, aux;
    for(int i = 0; i < dimension + 1; i++) {
        points[i] = 0;
    }
    #endif

    double total = 0;
    for(int i = 0; i < vehicles; i++) {
        cout << "Rota[" << i << "] = {";
        for(int j = 0; j < routes[i].order_of_visit.size() - 1; j++) {
            cout << routes[i].order_of_visit[j] << ", ";

            #ifdef DEBUG_VALUES
            points[routes[i].order_of_visit[j]]++;
            #endif
        }
        cout << routes[i].order_of_visit[(routes[i].order_of_visit.size() - 1)] << "}" << endl;
        cout << "Custo: " << routes[i].cost << endl;
        cout << "Demanda: " << routes[i].load << endl;

        #ifdef DEBUG_VALUES
        aux = getCost(routes[i].order_of_visit);
        actual_total += aux;
        cout << "Custo (real): " << aux << endl;
        cout << "Demanda (real): " << getDemand(routes[i].order_of_visit) << endl;
        cout << "Clientes[" << i << "] = {";
        for(int j = 0; j < routes[i].clients_set.size() - 1; j++) {
            cout << routes[i].clients_set[j] << ", ";
        }
        cout << routes[i].clients_set[(routes[i].clients_set.size() - 1)] << "}\n" << endl;
        #endif

        total += routes[i].cost;
    }
    cout << "Custo total: " << total << endl;

    #ifdef DEBUG_VALUES
    cout << "Custo total (real): " << actual_total << endl;
    for(int i = 1; i < dimension + 1; i++) {
        if(!points[i]) {
            cout << "Faltando: " << i << endl;
        } else if(i > 1 && points[i] > 1) {
            cout << i << " apareceu " << points[i] << " vezes!!" << endl;
        }
    }

    delete [] points;
    #endif
}

void readInstance(string &instance) {
    std::ifstream file(instance);
    std::string aux;
    if(!file.is_open()) {
        std::cout << "Error while opening file: " << instance << std::endl;
        exit(1);
    }
    file >> aux; // NAME:
    file >> instance;
    file >> aux; // DIMENSION:
    file >> dimension;

    // Aloca os vetores de demanda e adjacencia
    matrizAdj = new double*[dimension + 1];
    for(int i = 0; i <= dimension; i++) {
        matrizAdj[i] = new double[dimension + 1];
    }
    demands = new int[dimension + 1];
    clients = new client[dimension];

    file >> aux; // VEHICLES:
    file >> vehicles;

    for(int i = 0; i < vehicles; i++) {
        routes.push_back(route());
    }
    
    file >> aux; // CAPACITY:
    file >> capacity;
    file >> aux; // DEMAND_SECTION:
    for(int i = 0; i < dimension; i++) {
        file >> clients[i].index; // Index
        file >> clients[i].demand;
        demands[i + 1] = clients[i].demand;
    }
    
    file >> aux; // NODE_COORD_SECTION / EDGE_WEIGHT_SECTION
    for(int i = 1; i <= dimension; i++) {
        for(int j = 1; j <= dimension; j++) {
            file >> matrizAdj[i][j];
        }
    }
    file.close();
}

void buildPool() {
    for(int i = 0; i  < vehicles; i++) {
        string r = "";
        for(int j = 0; j < routes[i].order_of_visit.size() - 2; j++) {
            r = r + to_string(routes[i].order_of_visit[j]);
            r = r + " ";
        }
        r = r + to_string(routes[i].order_of_visit[routes[i].order_of_visit.size() - 1]);
        pool[r] = routes[i].cost;
    }
}

void addToPool(vector<int> &route, double cost) {
    string r = "";
    for(int j = 0; j < route.size() - 1; j++) {
        r = r + to_string(route[j]);
        r = r + " ";
    }
    r = r + to_string(route[route.size() - 1]);
    pool[r] = cost;
}

void printPool() {
    cout << "Vehicles: " << vehicles << endl;
    cout << "Clients: " << dimension << endl;
    cout << "Pool_size: " << pool.size() << endl;
    for(auto r : pool) {
        cout << "Route: " << r.first << endl;
        cout << "Cost: " << r.second << endl;
    }
    cout << "END" << endl;
}

void fixVector(vector<int> &vec) {
    auto last = unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
}
