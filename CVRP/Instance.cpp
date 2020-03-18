#include <iostream>
#include <fstream>

#include "Instance.hpp"

// Variáveis de instância
std::string instance;
double ** matrizAdj; // matriz de adjacencia
int *demands; // vetor de demandas
int dimension, vehicles, capacity; // quantidade total de vertices, veiculos e capacidade dos veiculos
client *clients; // Conjunto de clientes da instancia

// Variáveis da solução
solution s;
route_pool pool;

void readInstance(std::string &instance) {
    std::ifstream file(instance);
    std::string aux;
    if(!file.is_open()) {
        std::cout << "Error while opening file: " << instance << std::endl;
        exit(-1);
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

void printInstance() {
    std::cout << "Instance: " << instance << std::endl;
    std::cout << "Dimension: " << dimension << std::endl;
    std::cout << "Vehicles: " << vehicles << std::endl;
    std::cout << "Capacity: " << capacity << std::endl;
    std::cout << "Clients: " << clients << std::endl;
    for(int i = 0; i < dimension; i++) {
        std::cout << "Index: " << clients[i].index << "Demand: " << clients[i].demand << std::endl;
    }
    std::cout << "Matrix:" << std::endl;
    for(int i = 1; i <= dimension; i++) {
        for(int j = 1; j <= dimension; j++) {
            std::cout << matrizAdj[i][j] << " ";
        }
    }
    std::cout << std::endl;
}

void deallocateInstance() {
    for(int i = 0; i <= dimension; i++) {
        delete [] matrizAdj[i];
    }
    delete [] matrizAdj;
    delete [] demands;
    delete [] clients;
}
