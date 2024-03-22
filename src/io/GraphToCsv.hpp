#ifndef ATTACKGRAPHGENERATOR_GRAPHTOCSV_HPP
#define ATTACKGRAPHGENERATOR_GRAPHTOCSV_HPP

#include "AttackGraph.hpp"
#include "effolkronium/random.hpp"
#include "FsUtils.hpp"
#include <iostream>
#include <fstream>
#include <format>
#include <random>

double generateEdgeWeight(int edgeNum) {
    using Random = effolkronium::random_static;
    auto result = Random::get<std::normal_distribution<>>(1 / std::log2(edgeNum), 0.2);
    return result < 0 ? 0.01 : result > 1 ? 1.0 : result;

}

void graphToCsv(AttackGraph *graph, std::string &outDir, bool rndEdgeWeight) {
    // First create the folder if not exist
    std::error_code err;
    if (!CreateDirectoryRecursive(outDir, err))
        std::cerr << "Error creating folder " << outDir << ". " << err.message() << std::endl;

    // Then open/create the files for writing
    std::fstream vertFile(outDir + "/VERTICES.CSV", std::ios::out | std::ios::trunc);
    std::fstream arcFile(outDir + "/ARCS.CSV", std::ios::out | std::ios::trunc);

    for (int i = 1; i <= graph->getSize(); ++i) {
        GraphNode *node = graph->getNode(i);
        std::string type;
        if (node->getType() == LEAF) type = "LEAF";
        else if (node->getType() == AND) type = "AND";
        else type = "OR";

        vertFile << std::format("{},\"{}\",\"{}\",{}\n", node->getId(), node->getDesc(), type,
                                node->getType() == LEAF ? 1 : 0);

        for (const auto &adj: node->getAdj()) {
            double edgeProb = 1.0;
            if (rndEdgeWeight) edgeProb = generateEdgeWeight(graph->getNumEdges());

            arcFile << std::format("{},{},{}\n", adj->getId(), node->getId(), edgeProb);
        }
    }
    vertFile.close();
    arcFile.close();
}

#endif //ATTACKGRAPHGENERATOR_GRAPHTOCSV_HPP
