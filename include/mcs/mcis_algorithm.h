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

enum class AlgorithmType { BRANCH_AND_BOUND, HEURISTIC, DYNAMIC_PROGRAMMING };

class MCISAlgorithm {
private:
    std::vector<MCISFinder*> algorithms;

public:
    MCISAlgorithm();

    ~MCISAlgorithm();

    std::vector<std::vector<Graph&>> run(const Graph& g1, const Graph& g2,
                                         std::vector<AlgorithmType> types);

    std::vector<std::vector<Graph*>> run_all(const Graph& g1, const Graph& g2);
};

#endif  // MCIS_ALGORITHM_H
