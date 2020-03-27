# CVRPlaygrounds
Capacited Vehicle Routing Problem experiments

## Execução
Observação: É necessário possuir a [API do Cplex para Python](#https://www.ibm.com/support/knowledgecenter/SSSA5P_12.7.1/ilog.odms.cplex.help/CPLEX/GettingStarted/topics/set_up/Python_setup.html) e Python 2.7 para resolução do pool de rotas.

```
$ ./<nome executavel> <arquivo entrada> <seed (opcional)
```

## Movimentos de vizinhança utilizados
### Intrarota
- 1-1 Swap
- 2-opt
- Reinsertion

### Interrota
- Swap
- Reinsertion

## Perturbações utilizadas
- Ejection chain
- 1-1 Swap randômico (em caso de impossibilidade de usar o ejection chain em uma das rotas)

## Comparações 
Os valores abaixo são utilizando por base a melhor solução encontrada durante a execução do ILS, a eficácia da pertubação é medida comparando o custo anterior com o custo da nova solução após perturbação e busca local.

| Instância | Solução ótima | Custo (ILS) | Gap (ILS) | Custo (Pool) | Gap (Pool) | Eficácia perturbação (%) |
|-----------|---------------|-------------|-----------|--------------|------------|--------------------------|
| P-n19-k2  | 212           | 212         | 0         | 212          | 0          | 8.49                     |
| P-n20-k2  | 216           | 216         | 0         | 216          | 0          | 15.38                    |
| P-n23-k8  | 529           | 615         | 16.25     | 615          | 16.25      | 2.17                     |
| P-n45-k5  | 510           | 510         | 0         | 510          | 0          | 31.32                    |
| P-n50-k10 | 696           | 700         | 0.57      | 697          | 0.14       | 19.38                    |
| P-n51-k10 | 741           | 772         | 4.18      | 755          | 1.88       | 5.1                      |
| P-n55-k7  | 568           | 576         | 1.41      | 575          | 1.23       | 46.05                    |
