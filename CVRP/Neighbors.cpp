#include "Neighbors.hpp"
#include "Structures.hpp"

#include <cfloat>

using namespace std;

/* Swap 1-1 intrarota */
bool swap(vector<int> &st, double &cost) {
    double delta = DBL_MAX, nDelta = 0;
    int index_a, index_b;
    for(int i = 1; i < st.size() - 1; i++) {
        for(int j = i + 1; j < st.size() - 1; j++) {
            if(i == j - 1) {
                nDelta = matrizAdj[st.at(i - 1)][st.at(j)] + matrizAdj[st.at(j + 1)][st.at(i)] - matrizAdj[st.at(i - 1)][st.at(i)] - matrizAdj[st.at(j + 1)][st.at(j)];
            } else {
                nDelta = matrizAdj[st.at(i)][st.at(j - 1)] + matrizAdj[st.at(i)][st.at(j + 1)] + matrizAdj[st.at(j)][st.at(i - 1)] + matrizAdj[st.at(j)][st.at(i + 1)] - matrizAdj[st.at(j)][st.at(j - 1)] - matrizAdj[st.at(j)][st.at(j + 1)] - matrizAdj[st.at(i)][st.at(i - 1)] - matrizAdj[st.at(i)][st.at(i + 1)];
            }
            if(nDelta < delta) {
                index_a = i;
                index_b = j;
                delta = nDelta;
            }
        }
    }
    
    if(delta < 0) {
        int aux = st.at(index_a);
        
        st.at(index_a) = st.at(index_b);
        st.at(index_b) = aux;
        
        cost += delta;
        return true;
    }
    return false;
}

bool interSwap(vector<route> &rl) {
    double delta = DBL_MAX, aux_da, aux_db, delta_a = 0, delta_b = 0;
    int route_a, route_b, index_a, index_b, aux_dda, aux_ddb, demand_a, demand_b;
    for(int a = 0; a < rl.size() - 1; a++) {
        for(int b = a + 1; b < rl.size(); b++) {
            for(int i = 1; i < rl[a].order_of_visit.size() - 1; i++) {
                for(int j = 1; j < rl[b].order_of_visit.size() - 1; j++) {
                    aux_dda = rl[a].load + demands[rl[b].order_of_visit[j]] - demands[rl[a].order_of_visit[i]];
                    aux_ddb = rl[b].load + demands[rl[a].order_of_visit[i]] - demands[rl[b].order_of_visit[j]];
                    if(aux_dda <= capacity && aux_ddb <= capacity) {
                        aux_da = matrizAdj[rl[b].order_of_visit[j]][rl[a].order_of_visit[i - 1]] + matrizAdj[rl[b].order_of_visit[j]][rl[a].order_of_visit[i + 1]] - matrizAdj[rl[a].order_of_visit[i]][rl[a].order_of_visit[i - 1]] - matrizAdj[rl[a].order_of_visit[i]][rl[a].order_of_visit[i + 1]];
                        aux_db = matrizAdj[rl[a].order_of_visit[i]][rl[b].order_of_visit[j - 1]] + matrizAdj[rl[a].order_of_visit[i]][rl[b].order_of_visit[j + 1]] - matrizAdj[rl[b].order_of_visit[j]][rl[b].order_of_visit[j - 1]] - matrizAdj[rl[b].order_of_visit[j]][rl[b].order_of_visit[j + 1]];
                        if((aux_da + aux_db) < delta) {
                            index_a = i;
                            index_b = j;
                            route_a = a;
                            route_b = b;

                            delta_a = aux_da;
                            delta_b = aux_db;
                            
                            demand_a = aux_dda;
                            demand_b = aux_ddb;

                            delta = (delta_a + delta_b);
                        }
                    }
                }
            }
        }
    }
    
    if(delta < 0) {
        int aux;
        // Trocar clientes
        aux = rl[route_a].order_of_visit.at(index_a);
        rl[route_a].order_of_visit.at(index_a) = rl[route_b].order_of_visit.at(index_b);
        rl[route_b].order_of_visit.at(index_b) = aux;
        // Atualizar custos e demandas
        rl[route_a].cost += delta_a;
        rl[route_a].load = demand_a;
        rl[route_b].cost += delta_b;
        rl[route_b].load = demand_b;
        // Atualizar candidatos
        rl[route_a].updateClients();
        rl[route_b].updateClients();
        return true;
    }

    return false;
}

/* 2-Opt intrarota */
bool twoOpt(vector<int> &st, double &cost) {
    double delta = DBL_MAX, nDelta = 0;
    int index_a, index_b;
    for(int i = 1; i < st.size() - 1; i++) {
        for(int j = i + 1; j < st.size() - 1; j++) {
            nDelta = matrizAdj[st.at(i - 1)][st.at(j)] + matrizAdj[st.at(j + 1)][st.at(i)] - matrizAdj[st.at(j + 1)][st.at(j)] - matrizAdj[st.at(i - 1)][st.at(i)];
            if(nDelta < delta) {
                index_a = i;
                index_b = j;
                delta = nDelta;
            }
        }
    }
    
    if(delta < 0) {
        vector<int> aux; // assumindo que index_a > index_b
        for(int i = index_b; i >= index_a; i--) {
            aux.push_back(st.at(i));
        }
        st.erase(st.begin() + index_a, st.begin() + index_b + 1);
        st.insert(st.begin() + index_a, aux.begin(), aux.end());
        
        cost += delta;
        return true;
    }
    return false;
}

/* Reinsercao intrarota */
bool reinsertion(vector<int> &st, int subtourSize, double &cost) {
    double delta = DBL_MAX, nDelta = 0;
    int index_a, index_b;
    for(int i = 1; i < st.size() - subtourSize; i++) {
        for(int j = 1; j < st.size() - 1; j++) {
            if(j >= i && j <= i + subtourSize) {
                continue;
            }
            nDelta = matrizAdj[st.at(j - 1)][st.at(i)] + matrizAdj[st.at(i + subtourSize - 1)][st.at(j)] + matrizAdj[st.at(i - 1)][st.at(i + subtourSize)] - matrizAdj[st.at(i + subtourSize - 1)][st.at(i + subtourSize)] - matrizAdj[st.at(j - 1)][st.at(j)] - matrizAdj[st.at(i - 1)][st.at(i)];
            if(nDelta < delta) {
                index_a = i;
                index_b = j;
                delta = nDelta;
            }
        }
    }
    
    if(delta < 0) {
        vector<int> aux;
        aux.insert(aux.end(), st.begin() + index_a, st.begin() + index_a + subtourSize);
        st.erase(st.begin() + index_a, st.begin() + index_a + subtourSize);
        if(index_a >= index_b) {
            st.insert(st.begin() + index_b, aux.begin(), aux.end());
        } else {
            st.insert(st.begin() + index_b - subtourSize, aux.begin(), aux.end());
        }
        
        cost += delta;
        return true;
    }
    return false;
}

/* Reinsercao interrota 1*/
bool interReinsertion(vector<route> &rl) {
    double delta = DBL_MAX, aux_da, aux_db, delta_a = 0, delta_b = 0;
    int route_a, route_b, index_a, index_b, aux_d, demand_delta;
    for(int a = 0; a < rl.size(); a++) {
        for(int b = 0; b < rl.size(); b++) {
            if(a == b) {
                continue;
            }
            for(int i = 1; i < rl[a].order_of_visit.size() - 1; i++) {
                aux_d = demands[rl[a].order_of_visit[i]];
                aux_da = matrizAdj[rl[a].order_of_visit[i - 1]][rl[a].order_of_visit[i + 1]] - matrizAdj[rl[a].order_of_visit[i - 1]][rl[a].order_of_visit[i]] - matrizAdj[rl[a].order_of_visit[i]][rl[a].order_of_visit[i + 1]];
                for(int j = 1; j < rl[b].order_of_visit.size(); j++) {
                    if(rl[b].load + aux_d <= capacity) {
                        aux_db = matrizAdj[rl[b].order_of_visit[j - 1]][rl[a].order_of_visit[i]] + matrizAdj[rl[a].order_of_visit[i]][rl[b].order_of_visit[j]] - matrizAdj[rl[b].order_of_visit[j - 1]][rl[b].order_of_visit[j]];
                        if((aux_da + aux_db) < delta) {
                            index_a = i;
                            index_b = j;
                            route_a = a;
                            route_b = b;

                            delta_a = aux_da;
                            delta_b = aux_db;
                            
                            demand_delta = aux_d;

                            delta = (delta_a + delta_b);
                        }
                    }
                }
            }
        }
    }
    
    if(delta < 0) {
        //Atualizar rotas
        rl[route_b].order_of_visit.insert(rl[route_b].order_of_visit.begin() + index_b, rl[route_a].order_of_visit.at(index_a));
        rl[route_a].order_of_visit.erase(rl[route_a].order_of_visit.begin() + index_a);
        //Atualizer custos e demandas
        rl[route_a].cost += delta_a;
        rl[route_a].load -= demand_delta;
        rl[route_b].cost += delta_b;
        rl[route_b].load += demand_delta;
        //Atualizar candidatos das rotas
        rl[route_a].updateClients();
        rl[route_b].updateClients();
        return true;
    }

    return false;
}
