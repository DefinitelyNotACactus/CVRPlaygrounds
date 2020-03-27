#!/usr/bin/python
import sys
import cplex
import math
from cplex.exceptions import CplexError

# leitor do arquivo do pool
def readPool(filePath):
    f = file(filePath, "r")
    text = str(f.read()).split()
    p = dimension = vehicles = 0
    routes = []
    costs = []
    true_routes = []
    while True:
        if(text[p] == "END"):
            break
        elif(text[p] == "Vehicles:"):
            p = p + 1
            k = int(text[p])
            p = p + 1
        elif(text[p] == "Clients:"):
            p = p + 1
            dimension = int(text[p])
            p = p + 1
        elif(text[p] == "Pool_size:"):
            p = p + 2
            # sem utilidade tam pool
        elif(text[p] == "Route:"):
            route = [0] * (dimension + 1)
            true_route = []
            while True:
                p = p + 1
                if(text[p] != "Cost:"):
                    route[int(text[p])] = route[int(text[p])] + 1
                    true_route.append(int(text[p]))
                else:
                    p = p + 1
                    costs.append(int(text[p]))
                    p = p + 1
                    break
            routes.append(route)
            true_routes.append(true_route)
        else:
            break

    f.close()
    
    return costs, true_routes, routes, k, dimension

def createProblem(costs, routes, k, v):
    
    prob = cplex.Cplex()
    prob.objective.set_sense(prob.objective.sense.minimize)
    
    for i in range(len(costs)): # variaveis xr
        prob.variables.add(obj = [costs[i]], lb = [0], ub = [1], types = "I", names = ["x_" + str(i + 1)])
        
    for i in range(2, v + 1): # restricao dos clientes
        var_list = []
        coeff_list = []
        for j in range(len(costs)):
            var_list.append("x_" + str(j + 1))
            coeff_list.append(routes[j][i])

        prob.linear_constraints.add(lin_expr = [[var_list, coeff_list]], senses = "E", rhs = [1], names = ["COV1_" + str(i)])

    var_list = []
    coeff_list = []
    for i in range(len(costs)): # Numero de rotas = K (Numero de veiculos)
        var_list.append("x_" + str(i + 1))
        coeff_list.append(1)
        
    prob.linear_constraints.add(lin_expr = [[var_list, coeff_list]], senses = "L", rhs = [k], names = ["COV2"])
    
    return prob

def main():
    try:
        costs, true_routes, routes, k, dimension = readPool(sys.argv[1])
        prob = createProblem(costs, routes, k, dimension)
        prob.write("modelo.lp")
        prob.solve()
    except CplexError as exc:
        print(exc)
        return

    # solution.get_status() returns an integer code
    print "Solution status = ", prob.solution.get_status(), ":",
    # the following line prints the corresponding string
    print prob.solution.status[prob.solution.get_status()]
    print "Solution value  = ", prob.solution.get_objective_value()
    print "Solution (Instance:", str(sys.argv[1]) + ")"
    
    for i in range(len(routes)):
        if(prob.solution.get_values("x_" + str(i+1)) == 1):
            print true_routes[i]

if __name__ == "__main__":
   main()

