#ifndef ATTACKGRAPHGENERATOR_GENERATION_HPP
#define ATTACKGRAPHGENERATOR_GENERATION_HPP

#include "AttackGraph.hpp"
#include <string>
#include <queue>
#include <forward_list>
#include <ranges>
#include "effolkronium/random.hpp"

extern int verbosity;
extern bool manualStepping;

inline bool addEdge(GraphNode *src, GraphNode *dst, const bool cycleOk) {
    if (verbosity > 1) {
        std::cout << "Attempting to add edge from " << src->getId() << " to " << dst->getId();
    }
    // Check for cycles (a cycle is found when the dst predecessors are a subset of src predecessor)
    if (!cycleOk && src->predContains(dst->getPred())) {
        if (verbosity > 1) {
            std::cout << "...failed: will form a cycle" << std::endl;
        }
        return false;
    }
    if (src->adjContains(dst)) {
        if (verbosity > 1) {
            std::cout << "...failed: already exists" << std::endl;
        }
        return false;
    }
    if (src == dst) {
        if (verbosity > 1) {
            std::cout << "...failed: adding a self loop" << std::endl;
        }
        return false;
    }
    src->addAdj(dst);
    if (verbosity > 1) {
        std::cout << "...ok" << std::endl;
    }

    // Update pred info for non-cycle graphs
    if (!cycleOk) {
        std::queue<GraphNode *> q;

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

inline AttackGraph *generateGraph(const int numOr, const int numAnd, const int numLeaf,
                                  int numEdge, const bool cycleOk, const bool relaxed, const unsigned long seed) {
    const int total = numOr + numAnd + numLeaf;
    using Random = effolkronium::random_static;

    Random::seed(seed);
    // Initialize a graph
    auto *graph = new AttackGraph(1, total, numEdge);
    // Initialize nodes
    for (int i = 1; i <= total; i++) {
        NodeType type;
        int iCap, oCap;
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

        auto typeStr = (type == LEAF) ? "LEAF" : (type == OR) ? "OR" : "AND";
        if (verbosity > 1) {
            std::cout << "Generated node: {id = " << i << ", type = " << typeStr << "}" << std::endl;
        }

        const auto node = new GraphNode(i, type, desc, iCap, oCap);
        node->addPred(node);
        graph->addNode(node);
    }

    if (verbosity > 0) {
        std::cout << "Node generation complete." << std::endl;
    }

    if (manualStepping) {
        std:: cout << "\nr>";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Create the permutation of all possible edges
    std::list<Edge> allEdges;
    for (int src = 2; src <= total; src++) {
        if (graph->getNode(src)->getType() == AND) {
            for (int dst = 1; dst <= numOr; dst++) {
                allEdges.push_front(Edge(graph->getNode(src), graph->getNode(dst)));
            }
        } else {
            for (int dst = numOr + numLeaf + 1; dst <= total; dst++) {
                allEdges.push_front(Edge(graph->getNode(src), graph->getNode(dst)));
            }
        }
    }

    if (verbosity > 2) {
        std::cout << "All edges in the linked list: \n";
        int index = 0;
        for (auto &edge: allEdges) {
            std::cout << "index: " << index << "(" << edge.src << ", " << edge.dst << ") " << std::endl;
            index += 1;
        }
    }

    if (manualStepping) {
        std:: cout << "\nr>";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (numEdge > 0) {
        // Randomly choose one from the permutations
        int chosen = Random::get(0, int(allEdges.size() - 1));
        auto it = allEdges.begin();
        std::advance(it, chosen);

        if (verbosity > 1) {
            std::cout << "Chosen index id: " << chosen
                      << ", corresponding edge: (" << it->src << ", " << it->dst << ")";
        }

        if (manualStepping) {
            std:: cout << "\nr>";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (addEdge(it->src, it->dst, cycleOk)) {
            numEdge -= 1;

            // Delete the added edge from the list of all edges
            if (!relaxed && it->src->getType() == AND) {
                // Delete all edges that starts from this node, as only one can exist
                if (verbosity > 1) {
                    auto removed = allEdges | std::views::filter([&it](Edge e) {
                        return e.src == it->src; });
                    for (auto &edge: removed) {
                        std::cout << "removing edge: (" << edge.src << ", " << edge.dst << ")" << std::endl;
                    }
                }
                allEdges.remove_if([&it](Edge e) { return e.src == it->src; });

            } else {
                if (verbosity > 1) {
                    auto removed = allEdges | std::views::filter([&it](Edge e) {
                        return e.src == it->src && e.dst == it->dst; });
                    for (auto &edge: removed) {
                        std::cout << "removing edge: (" << edge.src << ", " << edge.dst << ")" << std::endl;
                    }
                }
                allEdges.remove_if([&it](Edge e) { return e.src == it->src && e.dst == it->dst; });
            }
        }

        if (manualStepping) {
            std:: cout << "\nr>";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (verbosity > 2) {
            std::cout << "Edges in the linked list after the removal: \n";
            int index = 0;
            for (auto &edge: allEdges) {
                std::cout << "index: " << index << "(" << edge.src << ", " << edge.dst << ") " << std::endl;
                index += 1;
            }
        }

        if (manualStepping) {
            std:: cout << "\nr>";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return graph;
}

#endif //ATTACKGRAPHGENERATOR_GENERATION_HPP
