#include "BuildHeuristics.hpp"
#include "Structures.hpp"

#include <cfloat>
#include <cmath>
#include <iostream>
using namespace std;

void buildNearestNeighbor(vector<int> &s, vector<int> &cl) {
    vector<int> candidates = cl;
    s.push_back(1);
    int currentPoint = 1;
    while(candidates.size() > 1) {
        double nearestDist = matrizAdj[currentPoint][candidates.at(0)];
        int nearestIndex = 0;
        for(int i = 1; i < candidates.size(); i++) {
            if(matrizAdj[currentPoint][candidates.at(i)] < nearestDist) {
                nearestDist = matrizAdj[currentPoint][candidates.at(i)];
                nearestIndex = i;
            }
        }
        currentPoint = candidates.at(nearestIndex);
        s.push_back(currentPoint);
        candidates.erase(candidates.begin() + nearestIndex);
    }
    s.push_back(candidates.at(0));
    s.push_back(1);
}

void buildCheapestInsertion(vector<int> &s, vector<int> &cl) {
    // Subrota inicial usando o primeiro do vetor de candidatos
    vector<int> candidates = cl;
    s.push_back(1);
    s.push_back(candidates.at(0));
    candidates.erase(candidates.begin());
    s.push_back(1);
    // Insercao na subrota
    int pi, pj, pk;
    double skij;
    while(candidates.size() > 0) {
        pi = 0, pj = 1, pk = 0;
        for(int i = 0; i < s.size() - 1; i++) {
            for(int j = 1; j < s.size(); j++) {
                skij = DBL_MAX;
                for(int k = 0; k < candidates.size(); k++) {
                    if(matrizAdj[s.at(i)][candidates.at(k)] + matrizAdj[candidates.at(0)][s.at(j)] - matrizAdj[s.at(i)][s.at(j)] < skij) {
                        skij = matrizAdj[s.at(i)][candidates.at(k)] + matrizAdj[candidates.at(0)][s.at(j)] - matrizAdj[s.at(i)][s.at(j)];
                        pi = i;
                        pj = j;
                        pk = k;
                    }
                }
            }
        }
        s.insert(s.begin() + pi, candidates.at(pk));
        candidates.erase(candidates.begin() + pk);
    }
}

void buildGreedyRandom(vector<int> &s, vector<int> &cl, int initialSubtour, double alpha) {
    s = {1, 1};
    vector<int> candidates = cl;
    int targetIndex;
    for(int i = 0; i < initialSubtour; i++) {
        targetIndex = rand() % candidates.size();
        s.insert(s.begin() + 1, candidates.at(targetIndex));
        candidates.erase(candidates.begin() + targetIndex);
    }

    while (candidates.size() > 0) {
        vector<insertionInfo> insertionCost((s.size() - 1) * candidates.size());
        for(int i = 1, l = 0; i < s.size(); i++) {
            for (int j = 0; j < candidates.size(); j++) {
                insertionCost[l].cost = matrizAdj[s[i - 1]][candidates[j]] + matrizAdj[s[i]][candidates[j]] - matrizAdj[s[i - 1]][s[i]];
                insertionCost[l].insertedNode = j;
                insertionCost[l].removedEdge = i;
                l++;
            }
        }
        sort(insertionCost.begin(), insertionCost.end());
        targetIndex = rand() % (int) floor(alpha * insertionCost.size());
        s.insert(s.begin() + insertionCost[targetIndex].removedEdge, candidates[insertionCost[targetIndex].insertedNode]);
        candidates.erase(candidates.begin() + insertionCost[targetIndex].insertedNode);
    }
}
