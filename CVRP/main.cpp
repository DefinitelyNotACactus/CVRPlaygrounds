#ifndef __APPLE__
#include <bits/stdc++.h>
#endif

#include "Structures.hpp"
#include "Neighbors.hpp"
#include "BuildHeuristics.hpp"
#include "LocalSearch.hpp"
#include "Util.hpp"
#include "ILS.hpp"
#include "Instance.hpp"
#include "Pertubation.hpp"

#include <iostream>
#include <fstream>

#include <unistd.h>

void solvePool(bool keep);

int main(int argc, char** argv) {
    if(argc < 2 || argc > 3) {
        std::cout << "Use ./<executable name> <instance> <seed (optional)>" << std::endl;
        exit(-1);
    }
    instance = argv[1];
    
    if(argc == 3) {
        seed = atoi(argv[2]);
    } else {
        seed = time(nullptr);
    }
    srand(seed);
    
    readInstance(instance);
    
    s = solution();
    pool = route_pool();
    
    s.initRoutes();
    
    ILS(50, 0.5, true);
        
    solvePool(false);
    
    deallocateInstance();
    
    return 0;
}

// Chamada o script para resolver o problema do pool
void solvePool(bool keep) {
    std::string poolFilename(instance + ".pool");
    std::ofstream poolFile(poolFilename);
    
    if(poolFile.is_open()) {
        pool.print(poolFile);
        poolFile.close();
    } else {
        std::cout << "Erro ao manipular o arquivo do pool!" << std::endl;
        exit(-1);
    }
    
    
    int status;
    
    std::vector<char*> argv;
    argv.push_back(const_cast<char*>("python"));
    argv.push_back(const_cast<char*>("solvePool.py"));
    argv.push_back(const_cast<char*>(poolFilename.c_str()));
    argv.push_back(nullptr);
    
    char **commands = argv.data();
    
    pid_t processId = fork();
    if(processId == 0) { // Processo filho
        execvp(commands[0], &commands[0]); // Chamar o script de python pra resolver o pool
    } else { // Processo pai
        waitpid(-1, &status, 0); // Aguardar o pool ser resolvido
    }
    
    if(!keep) {
        std::remove(poolFilename.c_str()); // Apagar o arquivo do pool
    }
}
