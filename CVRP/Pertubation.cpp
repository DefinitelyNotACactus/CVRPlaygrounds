#include "Pertubation.hpp"
#include "Instance.hpp"
#include "Util.hpp"

/* Perturbação intrarrota */
void randomSwap(std::vector<int> &s, double &cost) {
    if(s.size() < 4) { // Rota serve menos de 2 clientes, encerre
        return;
    }
    std::vector<int> st(s.begin() + 1, s.end() - 1); // copiar s sem incluir o depósito
    int index_a = rand() % st.size(), index_b; // Escolher o primeiro indice aleatoriamente
    do {
        index_b = rand() % st.size();
    } while(index_b != index_a); // Segundo indice deve ser diferente do primeiro
    // Fazer o movimento
    int aux = st[index_a];
    st[index_a] = st[index_b];
    st[index_b] = aux;
    // Atualizar rota original
    s.clear();
    s.push_back(1);
    s.insert(s.end(), st.begin(), st.end());
    s.push_back(1);
    cost = getRouteCost(s);
}

/* Perturbação interrota */
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
        } else { // Caso não existam, faça um swap 1-1 intrarota aleatório
            randomSwap(rl[i].order_of_visit, rl[i].cost);
        }
    }
}
