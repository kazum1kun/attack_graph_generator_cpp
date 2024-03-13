#ifndef ATTACKGRAPHGENERATOR_GENERATION_HPP
#define ATTACKGRAPHGENERATOR_GENERATION_HPP

#include "AttackGraph.hpp"
#include <string>
#include <queue>
#include "effolkronium/random.hpp"

inline bool addEdge(Node *src, Node *dst, const bool cycleOk) {
    // Check for cycles (a cycle is found when the dst predecessors are a subset of src predecessor)
    if (!cycleOk && src->predContains(dst->getPred())) {
        return false;
    }
    if (src->adjContains(dst)) {
        return false;
    }

    src->addAdj(dst);

    // Update pred info for non-cycle graphs
    if (!cycleOk) {
        std::queue<Node*> q;

        q.push(dst);
        while (!q.empty()) {
            auto node = q.front();
            node->addPred(src->getPred());
            for (auto adj: *node->getAdj()) {
                q.push(adj);
            }
            q.pop();
        }
    }

    src->decOCap();
    dst->decICap();

    return true;
}

inline AttackGraph* generateGraph(const int numOr, const int numAnd, const int numLeaf,
    int numEdge, const bool cycleOk, const bool relaxed, const unsigned long seed) {
    const int total = numOr + numAnd + numLeaf;
    const int leafPadding = 1 + numOr;
    const int andPadding = 1 + numOr + numLeaf;
    using Random = effolkronium::random_static;

    Random::seed(seed);
    // Initialize a graph
    auto *graph = new AttackGraph(1, total, numEdge);
    // Initialize nodes
    for (int i = 1; i <= total; i++) {
        NodeType type;
        int iCap , oCap;
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

        const auto node = new Node(i, type, &desc, iCap, oCap);
        node->addPred(node);
        graph->addNode(node);
    }

    // Calculate the portion of each
    int andTotalOCap;
    const int orTotalOCap = (numOr - 1) * numAnd;
    if (relaxed) {
        andTotalOCap = numAnd * numOr;
    } else {
        andTotalOCap = numAnd;
    }
    int leafTotalOCap = numLeaf * numAnd;
    int totalOCap = orTotalOCap + andTotalOCap + leafTotalOCap;

    while (numEdge > 0) {
        auto randNum = Random::get(1, totalOCap);
        int srcNodeNum;
        if (randNum <= orTotalOCap)
            // Skip index 0 and the goal node
            srcNodeNum = Random::get(2, numOr);
        else if (randNum > orTotalOCap && randNum <= orTotalOCap + leafTotalOCap)
            srcNodeNum = Random::get(leafPadding, numOr + numAnd);
        else
            srcNodeNum = Random::get(andPadding, total);
        Node *src = graph->getNode(srcNodeNum);
        if (src->getOCap() == 0)
            continue;

        Node* dst = graph->getNode(src->getType() == AND ?
                Random::get(1, numOr) : Random::get(andPadding, total));

        if (dst->getICap() == 0) {
            continue;
        }

        if (addEdge(src, dst, cycleOk)) {
            numEdge -= 1;
        }
    }
    return graph;
}

#endif //ATTACKGRAPHGENERATOR_GENERATION_HPP
