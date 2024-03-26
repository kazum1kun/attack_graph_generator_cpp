#ifndef ATTACKGRAPHGENERATOR_CSVTOGRAPH_HPP
#define ATTACKGRAPHGENERATOR_CSVTOGRAPH_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "AttackGraph.hpp"
#include <lazycsv.hpp>

namespace fs = std::filesystem;

AttackGraph csvToGraph(const fs::path& arcFilePath, const fs::path& vertFilePath) {

    // If we couldn't open the input file stream for reading
    if (!fs::exists(arcFilePath) || !fs::exists(vertFilePath)) {
        // Print an error and exit
        std::cerr << "Unable to open csv file for reading\n";
        std::exit(1);
    }
    auto arcFileStream = std::ifstream{arcFilePath};
    auto numEdges = std::count_if(std::istreambuf_iterator<char>
            {arcFileStream}, {}, [](char c) { return c == '\n'; });
    arcFileStream.close();
    auto vertFileStream = std::ifstream{vertFilePath};
    auto numVertices = std::count_if(std::istreambuf_iterator<char>
            {vertFileStream}, {}, [](char c) { return c == '\n'; });
    vertFileStream.close();

    auto graph = AttackGraph(1, static_cast<int>(numVertices), static_cast<int>(numEdges));

    // Add vertices
    lazycsv::parser<lazycsv::mmap_source, lazycsv::has_header<false>> vertParser{vertFilePath};
    for (const auto row : vertParser) {
        const auto [nodeId, name, typeCell, prob] = row.cells(0, 1, 2, 3);
        NodeType type;
        auto typeStr = typeCell.trimed();
        if (typeStr == "\"OR\"" || typeStr == "OR") {
            type = OR;
        } else if (typeStr == "\"AND\"" || typeStr == "AND") {
            type = AND;
        } else if (typeStr == "\"LEAF\"" || typeStr == "LEAF") {
            type = LEAF;
        } else {
            std::cerr << "Unknown node type \"" << typeStr << "\"\n";
            continue;
        }
        auto id = std::stoi(static_cast<const std::string>(nodeId.trimed()));
        auto nameStr = static_cast<const std::string>(name.trimed());

        const auto node = new GraphNode(id, type, nameStr);
        graph.addNode(*node);
    }

    // Add edges
    lazycsv::parser<lazycsv::mmap_source, lazycsv::has_header<false>> arcParser{ arcFilePath };
    for (const auto row : arcParser) {
        const auto [dstCell, srcCell] = row.cells(0, 1);
        int dst = std::stoi(static_cast<const std::string>(dstCell.trimed()));
        int src = std::stoi(static_cast<const std::string>(srcCell.trimed()));

        graph.addEdgeById(src, dst);
    }
//
//    // Construct vertices
//    while (std::getline(vertFile, strInput)) {
//        auto tokens = split(strInput, ",");
//        int nodeId = std::stoi(tokens[0]);
//        std::string name = tokens[1];
//        NodeType type;
//        if (tokens[2] == "\"OR\"" || tokens[2] == "OR") {
//            type = OR;
//            break;
//        } else if (tokens[2] == "\"AND\"" || tokens[2] == "AND") {
//            type = AND;
//            break;
//        } else if (tokens[2] == "\"LEAF\"" || tokens[2] == "LEAF") {
//            type = LEAF;
//            break;
//        } else {
//            std::cerr << "Unknown node type \"" << tokens[2] << "\"\n";
//            continue;
//        }
//        double prob = std::stod(tokens[3]);
//
//        const auto node = new GraphNode(nodeId, type, name);
//        graph.addNode(*node);
//    }

//    // Construct edges
//    while (std::getline(arcFile, strInput)) {
//        auto tokens = split(strInput, ",");
//        int dst = std::stoi(tokens[0]);
//        int src = std::stoi(tokens[1]);
//        double prob = std::stod(tokens[2]);
//
//        graph.addEdgeById(src, dst);
//
//    }

    return graph;
}

#endif //ATTACKGRAPHGENERATOR_CSVTOGRAPH_HPP
