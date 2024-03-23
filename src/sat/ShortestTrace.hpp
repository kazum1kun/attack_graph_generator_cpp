#ifndef ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP
#define ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP

#include "AttackGraph.hpp"
#include "PriorityQueue.hpp"
#include <optional>
#include <vector>
#include <queue>
#include <limits>
#include <forward_list>


std::forward_list<Edge> constructTrace(AttackGraph& graph) {
    std::queue<GraphNode *> q;
    std::forward_list<Edge> results;

    q.push(graph.getNode(graph.getGoalId()));
    while (!q.empty()) {
        auto v = q.front();
        q.pop();

        if (v->getType() == OR) {
            auto u = v->getParent();
            q.push(u);
            results.emplace_front(u, v);
        }
        if (v->getType() == AND) {
            for (auto u : v->getRevAdj()) {
                q.push(u);
                results.emplace_front(u, v);
            }
        }
    }
    return results;
}

// Implements Alg. 1 of the IoTDI paper SAT(G)
std::optional<std::forward_list<Edge>> Sat(AttackGraph &graph, const std::optional<std::vector<double>>& weight) {
    auto pq = PriorityQueue(graph.getSize());
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
        auto u = pq.pop();
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
                    pq.decreaseKey(v->getPosInHeap(), temp);
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
    return std::nullopt;
    trace_found:
        // Extract the trace
    return constructTrace(graph);
}

#endif //ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP
