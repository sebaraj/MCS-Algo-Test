/**
 * @file
 * @author Bryan SebaRaj <bryan.sebaraj@yale.edu>
 * @version 1.0
 * @section DESCRIPTION
 */

#ifndef MCIS_ALGORITHM_H
#define MCIS_ALGORITHM_H

#include <vector>

#include "graph.h"
#include "src/algorithms/mcis_finder.h"

enum class AlgorithmType { BRON_KERBOSCH_SERIAL, FILLER };

class MCISAlgorithm {
private:
    std::vector<MCISFinder*> algorithms;

public:
    MCISAlgorithm();

    ~MCISAlgorithm();

    std::vector<Graph*> run(const Graph& g1, const Graph& g2, AlgorithmType type);

    std::vector<std::vector<Graph*>> run_many(const Graph& g1, const Graph& g2,
                                              std::vector<AlgorithmType> types);
};

#endif  // MCIS_ALGORITHM_H
