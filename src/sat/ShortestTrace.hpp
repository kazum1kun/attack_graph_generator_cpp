#ifndef ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP
#define ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP

#include "AttackGraph.hpp"
#include <optional>
#include <vector>
#include <queue>
#include <limits>


// Implements Alg. 1 of the IoTDI paper SAT(G)
void Sat(AttackGraph graph, const std::optional<std::vector<double>>& weight) {
    std::priority_queue<GraphNode*, std::vector<GraphNode *>, std::greater<>> pq;
    const double INFINITY = std::numeric_limits<double>::max();

    // Initialize the data structure
    // Note: type, done, inDegree, and color is already correctly set up before the graph is passed in here
    for (auto &node: *graph.getNodes()) {
        if (node->getType() == OR) {
            node->setWeight(INFINITY);
        } else if (node->getType() == AND) {
            node->setWeight(-INFINITY);
        } else {
            pq.push(node);
            node->setColor(GRAY);
        }
    }

    while (!pq.empty()) {
        auto u = pq.top();
        pq.pop();
        if (u->getId() == graph.getGoalId()) {
            goto trace_found;
        }
        for (auto& v : u->getAdj()) {
            if (u->getColor() == BLACK) break;
            // For now just ignore the weights (default to IoTA 2022 settings: edges = 1, nodes = 0)
            double temp = u->getWeight() + 1 + 0;
            if (v->getType() == OR) {
                if (v->getDone() == 0) {
                    v->setWeight(temp);
                    v->setParent(u);
                    pq.push(v);
                    v->setColor(GRAY);
                }
                else if (temp < v->getWeight()) {
                    v->setWeight(temp);
                    v->setParent(u);
                }
                v->incDone();
            }
            else if (v->getType() == AND) {
                v->setWeight(std::max(v->getWeight(), temp));
                v->incDone();
                if (v->getDone() == v->getInDegree()) {
                    pq.push(v);
                    v->setColor(GRAY);
                }
            }
        }
    }
    // No attack trace is found
    return;
    trace_found:
        // Extract the trace
    return;
}

#endif //ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP
