#include "Pertubation.hpp"
#include "Instance.hpp"
#include "Util.hpp"

/* Pertubacao intrarota */
void doubleBridge(std::vector<int> &s, std::vector<int> &st, double &cost, double &sCost) {
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

        //sCost = matrizAdj[s.at(index_a - 1)][s.at(index_c)] + matrizAdj[s.at(s.size() - 2)][s.at(index_b)] + matrizAdj[s.at(index_c - 1)][s.at(index_a)] + matrizAdj[s.at(index_b - 1)][s.at(0)] - matrizAdj[s.at(index_a - 1)][s.at(index_a)] - matrizAdj[s.at(index_b - 1)][s.at(index_b)] - matrizAdj[s.at(index_c - 1)][s.at(index_c)] - matrizAdj[s.at(s.size() - 2)][s.at(0)];
        //sCost += cost;
        sCost = getRouteCost(st);
    }
}

/* Pertubacao interrota, caso não seja possível aplicar em uma rota utiliza-se doubleBridge */
void ejectionChain(std::vector<route> &rl) {
    for(int i = 0; i < rl.size(); i++) {
        int next = (i + 1) % rl.size(), nextRouteLoad = rl[next].load;
        std::vector<int> moveable_clients, moveable_index;
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
            } while(targetIndex == 0); // Evitar que o index seja 0 (i.e. deposito)
            
            rl[next].order_of_visit.insert(rl[next].order_of_visit.begin() + targetIndex, moveable_clients[ejected]);
            rl[next].cost = getRouteCost(rl[next].order_of_visit);
            rl[next].load += demands[moveable_clients[ejected]];
            rl[next].updateClients();
            
            rl[i].order_of_visit.erase(rl[i].order_of_visit.begin() + moveable_index[ejected]);
            rl[i].cost = getRouteCost(rl[i].order_of_visit);
            rl[i].load -= demands[moveable_clients[ejected]];
            rl[i].updateClients();
        } else { // Usa double bridge (porém tamanho da rota tem de ser >= 4)
            std::vector<int> st = rl[i].order_of_visit;
            double sCost = rl[i].cost;
            doubleBridge(rl[i].order_of_visit, st, rl[i].cost, sCost);
        }
    }
}
