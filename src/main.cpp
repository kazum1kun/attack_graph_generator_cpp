#include <iostream>
#include <argparse/argparse.hpp>
#include "Generation.hpp"
#include "GraphToCsv.hpp"
#include "CsvToPdf.hpp"

bool verbose;

int main(int argc, char *argv[]) {
    // Program argument parser
    argparse::ArgumentParser program("AGG", "0.1.0");
    program.add_argument("node")
            .help("number of OR, PF and AND nodes (in this order)")
            .required()
            .nargs(3)
            .scan<'i', int>();
    program.add_argument("edge")
            .help("number of edges")
            .scan<'i', int>()
            .required();
    program.add_argument("-c", "--cycle")
            .help("whether cycles are permitted (does not guarantee them)")
            .flag();
    program.add_argument("-s", "--seed")
            .help("random seed")
            .scan<'i', unsigned long>()
            .default_value(static_cast<unsigned long>(std::time(nullptr)));
    program.add_argument("-o", "--outdir")
            .help("output directory")
            .default_value("./");
    program.add_argument("-g", "--graph")
            .help("generate a visual rendition of the attack graph")
            .flag();
    program.add_argument("--relaxed")
            .help("relax the constraint so that AND node can have multiple outgoing edges")
            .flag();
    program.add_argument("-vs", "--vertsed")
            .help("sed `FILE` to be used to process VERTICES.CSV")
            .default_value("./misc/VERTICES_no_metric.sed");
    program.add_argument("-as", "--arcsed")
            .help("sed `FILE` to be used to process ARCS.CSV")
            .default_value("./misc/ARCS_noLabel.sed");
    program.add_argument("-r", "--randw")
            .help("add random weights to the edges")
            .flag();
    program.add_argument("-v", "--verbose")
            .help("more verbose output")
            .flag();

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    auto nodes = program.get<std::vector<int>>("node");
    auto edge = program.get<int>("edge");
    auto cycle = program.get<bool>("-c");
    auto seed = program.get<unsigned long>("-s");
    auto outDir = program.get<std::string>("-o");
    auto drawGraph = program.get<bool>("-g");
    auto relaxed = program.get<bool>("-r");
    auto vertSed = program.get<std::string>("-vs");
    auto arcSed = program.get<std::string>("-as");
    auto randW = program.get<bool>("-r");
    verbose = program.get<bool>("-v");

    int numOr = nodes[0];
    int numLeaf = nodes[1];
    int numAnd = nodes[2];
//    int minEdge = (numLeaf + 2*numAnd + numOr) / 2;
    int maxEdge;
    if (!relaxed) {
        maxEdge = numLeaf * numAnd + numOr * numAnd;
        if (numOr > numAnd) {
            std::cerr << "Error: number of OR node cannot be greater than number of AND node" << std::endl;
            std::exit(1);
        }
    } else {
        maxEdge = (2*numLeaf + numOr + 4) * numAnd / 2;
    }

    if (edge > maxEdge) {
        std::cerr << "Error: edge out of bound for the current input" << std::endl;
        std::exit(1);
    }

    if (verbose) {
        std::cout << "Starting generation with the following params:\n"
                     "  numOr: " << numOr << "\n"
                     "  numAnd: " << numAnd << "\n"
                     "  numLeaf: " << numLeaf << "\n"
                     "  numEdge: " << edge << "\n"
                     "  seed: " << seed << "\n"
                     "  cycle: " << cycle << "\n"
                     "  drawGraph: " << drawGraph << "\n"
                     "  relaxed: " << relaxed << "\n"
                     "  outDir: " << outDir << "\n"
                     "  vertSed: " << vertSed << "\n"
                     "  acrdSed: " << arcSed << "\n"
                     "  randW: " << randW << "\n"
                     "  verbose: " << verbose << "\n";
    }

    auto graph = generateGraph(numOr, numAnd, numLeaf, edge, cycle, relaxed, seed);

    if (verbose) {
        std::cout << "Graph generated, saving it to files.";
    }
    graphToCsv(graph, outDir, randW);

    if (drawGraph) {
        if (verbose) {
            std::cout << "Converting graph CSV to pdf file.";
        }
        csvToPdf(outDir, arcSed, vertSed);
    }
}
