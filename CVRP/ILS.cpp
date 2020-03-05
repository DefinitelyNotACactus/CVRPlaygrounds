#include "ILS.hpp"
#include "Structures.hpp"
#include "BuildHeuristics.hpp"
#include "LocalSearch.hpp"
#include "Util.hpp"

#include <cfloat>

using namespace std;
/* Metaheuristica intrarota */
void gils_rvnd(route &r, int initialSubtourSize, int itr_max, int ils, double alpha) {
    vector<int> st, stt;
    route rt = r;
    rt.clients_set.clear();
    double f = DBL_MAX, sCost;
    for(int i = 0; i < itr_max; i++) {
        if(r.clients_set.size() > initialSubtourSize) { // Evitar que a construcao gulosa seja chamada se o subtour for maior que o numero de clientes para a rota
            buildGreedyRandom(r.order_of_visit, r.clients_set, initialSubtourSize, alpha);
        } else {
            buildNearestNeighbor(r.order_of_visit, r.clients_set);
        }
        r.cost = getCost(r.order_of_visit);
        sCost = r.cost;
        st = r.order_of_visit;
        int itr_ils = 0;
        while(itr_ils < ils) {
            rvnd(st, sCost);
            if(sCost < r.cost) {
                r.order_of_visit = st;
                r.cost = sCost;
                itr_ils = 0;
            }
            //doubleBridge(r.order_of_visit, st, r.cost, sCost);
            itr_ils++;
        }
        if(r.cost < f) {
            stt = r.order_of_visit;
            f = r.cost;
        }
    }
    r.cost = f;
    r.order_of_visit = stt;
}

/* Metaheuristica interrota */
void interGils_rvnd(vector<route> &rl, int itr_max, int ils, double alpha) {
    vector<route> r = rl, rt, rtt;
    double f = DBL_MAX, sCost = 0, stCost;
    for(int i = 0; i < itr_max; i++) {
        for(int j = 0; j < r.size(); j++) {
            buildNearestNeighbor(r[j].order_of_visit, r[j].clients_set);
            //gils_rvnd(r[j], 3, itr_max, ils, alpha);
            // Salvar no pool
            r[j].cost = getCost(r[j].order_of_visit);
            sCost += r[j].cost;
        }
        rt = r;
        int itr_ils = 0;
        while(itr_ils < ils) {
            stCost = 0;
            interRvnd(rt);
            for(int j = 0; j < rt.size(); j++) {
                rvnd(rt[j].order_of_visit, rt[j].cost);
                stCost += rt[j].cost;
            }
            if(stCost < sCost) {
                r = rt;
                sCost = stCost;
                itr_ils = 0;
                // adicionar a pool aqui
            }
            // Inserir perturbacao
            itr_ils++;
        }
        if(sCost < f) {
            rtt = r;
            f = sCost;
        }
    }
    rl = rtt;
}

/* Pertubacao intrarota */
void doubleBridge(vector<int> &s, vector<int> &st, double &cost, double &sCost) {
    int interval = s.size() >> 2;
    if(interval) {
        int index_a = 1 + rand() % interval,
        index_b = 1 + index_a + rand() % interval,
        index_c = 1 + index_b + rand() % interval;

        st.clear();
        st.insert(st.end(), s.begin(), s.begin() + index_a);
        st.insert(st.end(), s.begin() + index_c, s.end() - 1);
        st.insert(st.end(), s.begin() + index_b, s.begin() + index_c);
        st.insert(st.end(), s.begin() + index_a, s.begin() + index_b);
        st.insert(st.end(), s.at(0));

        sCost = matrizAdj[s.at(index_a - 1)][s.at(index_c)] + matrizAdj[s.at(s.size() - 2)][s.at(index_b)] + matrizAdj[s.at(index_c - 1)][s.at(index_a)] + matrizAdj[s.at(index_b - 1)][s.at(0)] - matrizAdj[s.at(index_a - 1)][s.at(index_a)] - matrizAdj[s.at(index_b - 1)][s.at(index_b)] - matrizAdj[s.at(index_c - 1)][s.at(index_c)] - matrizAdj[s.at(s.size() - 2)][s.at(0)];
        sCost += cost;
    }
}

/* Pertubacao interrota */
void ejectionChain(vector<route> &rl) {
    for(int i = 0; i < rl.size(); i++) {
        int next = (i + 1) % rl.size(), nextRouteLoad = rl[next].load;
        vector<int> moveable_clients, moveable_index;
        // Obter clientes que podem ser inseridos na rota i + 1
        for(int j = 1; j < rl[i].order_of_visit.size() - 1; j++) {
            if(nextRouteLoad + demands[rl[i].order_of_visit[j]] <= capacity) {
                moveable_clients.push_back(rl[i].order_of_visit[j]);
                moveable_index.push_back(j);
            }
        }
        
        if(moveable_clients.size() > 0) { // Existem clientes que podem ser movidos
            int ejected = rand() % moveable_clients.size(), targetIndex;
            do {
                targetIndex = rand() % rl[next].order_of_visit.size();
            } while(targetIndex != 0); // Evitar que o index seja 0 (i.e. deposito)
            rl[next].order_of_visit.insert(rl[next].order_of_visit.begin() + targetIndex, moveable_clients[ejected]);
            rl[next].cost = getCost(rl[next].order_of_visit);
            rl[next].load += demands[moveable_clients[ejected]];
            rl[next].updateClients();
            
            rl[i].order_of_visit.erase(rl[i].order_of_visit.begin() + moveable_index[ejected]);
            rl[i].cost = getCost(rl[i].order_of_visit);
            rl[i].load -= demands[moveable_clients[ejected]];
            rl[i].updateClients();
        }
    }
}
