/**
 * @file
 * @author Bryan SebaRaj <bryan.sebaraj@yale.edu>
 * @version 1.0
 * @section DESCRIPTION
 */

#ifndef BRON_KERBOSCH_SERIAL_H
#define BRON_KERBOSCH_SERIAL_H

#include <vector>

#include "mcis_finder.h"
#include "mcs/graph.h"

class BronKerboschSerial : public MCISFinder {
public:
    std::vector<Graph&> find(const Graph& g1, const Graph& g2) override;
};

#endif  // BRON_KERBOSCH_SERIAL_H
