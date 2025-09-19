/**
 * @file
 * @author Bryan SebaRaj <bryan.sebaraj@yale.edu>
 * @version 1.0
 * @section DESCRIPTION
 */

#include "mcis/mcis_algorithm.h"

#include <iostream>

#include "bron_kerbosch_serial.h"

MCISAlgorithm::MCISAlgorithm() { algorithms.push_back(new BronKerboschSerial()); }

MCISAlgorithm::~MCISAlgorithm() {
    for (auto algorithm : algorithms) {
        delete algorithm;
    }
}

std::vector<Graph*> MCISAlgorithm::run(const Graph& g1, const Graph& g2, AlgorithmType type) {
    switch (type) {
        case AlgorithmType::BRON_KERBOSCH_SERIAL:
            return algorithms[static_cast<int>(type)]->find(g1, g2);
            break;
        default:
            std::cerr << "Algorithm type not implemented.\n";
            return {};
    }

    return {};
}

std::vector<std::vector<Graph*>> MCISAlgorithm::run_many(const Graph& g1, const Graph& g2,
                                                         std::vector<AlgorithmType> types) {
    std::vector<std::vector<Graph*>> results;
    for (const auto& type : types) {
        switch (type) {
            case AlgorithmType::BRON_KERBOSCH_SERIAL:
                results.push_back(algorithms[static_cast<int>(type)]->find(g1, g2));
                break;
            default:
                std::cerr << "Algorithm type not implemented.\n";
                results.push_back({});
                break;
        }
    }
    return results;
}
