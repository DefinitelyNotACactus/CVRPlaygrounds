#ifndef Structures_hpp
#define Structures_hpp

#include <vector>
#include <unordered_map>
#include <string>

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

struct solution {
    double cost;
    std::vector<route> routes;
    
    double getCost(bool update);
    
    void print();
    void initRoutes();
};

struct route_pool {
    std::unordered_map<std::string, int> pool;
    
    void add(std::vector<int> &route, double cost);
    void add(std::vector<route> &routes);
    void print(std::ostream &out);
};
#endif /* Structures_hpp */
