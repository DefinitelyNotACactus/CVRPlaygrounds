#ifndef Structures_hpp
#define Structures_hpp

#include <vector>

//Variaveis
extern std::string instance;
extern double ** matrizAdj; // matriz de adjacencia
extern int *demands; // vetor de demandas
extern int dimension, vehicles, actual_vehicles, capacity; // quantidade total de vertices, veiculos e capacidade dos veiculos

// Estruturas
struct insertionInfo {
    int insertedNode;
    int removedEdge;
    double cost;

    bool operator <(const insertionInfo &b) const {
        return cost < b.cost;
    }
};

struct route {
    std::vector<int> order_of_visit, clients_set; // vetor da ordem de visita e conjunto de clientes da rota
    int load;
    double cost;
    
    void updateClients() {
        clients_set.clear();
        for(int i = 1; i < order_of_visit.size() - 1; i++) {
            clients_set.push_back(order_of_visit[i]);
        }
    }
};

struct client {
    int index;
    int demand;
    
    bool operator <(const client &c) const { // Comparar clientes pela demanda
        return demand > c.demand;
    }
};

#endif /* Structures_hpp */
