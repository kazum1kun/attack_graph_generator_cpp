#ifndef ATTACKGRAPHGENERATOR_GENERATION_H
#define ATTACKGRAPHGENERATOR_GENERATION_H

#include "AttackGraph.h"
#include <string>

void generateGraph(int numOr, int numAnd, int numLeaf, int numEdge, bool cycleOk, bool relaxed) {
    int total = numOr + numAnd + numLeaf;
    int leafPadding = 1 + numOr;
    int andPadding = 1 + numOr + numLeaf;

    // Initialize a graph
    auto *graph = new AttackGraph(1, total);
    // Initialize nodes
    for (int i = 1; i <= total; i++) {
        NodeType type = AND;
        int iCap = 0;
        int oCap = 0;
        std::string desc;

        // Goal node
        if (i == 1) {
            type = OR;
            iCap = numAnd;
            oCap = 0;
            desc = "goal";
        } else if (i <= numOr) {
            type = OR;
            iCap = numAnd;
            oCap = numOr;
            desc = "d" + std::to_string(i);
        } else if (i <= numOr + numLeaf) {
            type = LEAF;
            iCap = 0;
            oCap = numAnd;
            desc = "p" + std::to_string(i - numOr);
        } else {
            type = AND;
            iCap = numOr + numLeaf;
            if (relaxed) {
                oCap = numOr;
            } else {
                oCap = 1;
            }
            desc = "r" + std::to_string(i - numOr - numLeaf);
        }

        auto node = new Node(i, type, desc, iCap, oCap);
        node->addPred(node);
        graph->addNode(node);
    }

    // Calculate the portion of each
    int andTotalOCap = 0;
    int orTotalOCap = (numOr - 1) * numAnd;
    if (relaxed) {
        andTotalOCap = numAnd * numOr;
    } else {
        andTotalOCap = numAnd;
    }
    int leafTotalOCap = numLeaf * numAnd;

    double orPct = (double) orTotalOCap / total;
    double andPct = (double) andTotalOCap / total + orPct;

    while (numEdge > 0) {
        double randNum = (double) rand() / (double) RAND_MAX;
        if (randNum < orPct) {
            
        } else if (randNum >= orPct && randNum < andPct) {

        } else {

        }
    }
};

#endif //ATTACKGRAPHGENERATOR_GENERATION_H
