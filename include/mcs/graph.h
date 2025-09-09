/**
 * @file
 * @author Bryan SebaRaj <bryan.sebaraj@yale.edu>
 * @version 1.0
 * @section DESCRIPTION
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <vector>

#include "node.h"

class Graph {
private:
    std::unordered_map<std::string, Node*> nodes;

public:
    Graph();

    // constructor from vector of nodes
    Graph(const std::vector<Node>& node_list);
    Graph(const Graph& other);  // copy constructor
    // copy assignment operator
    Graph& operator=(const Graph& other);
    // move constructor
    Graph(Graph&& other) noexcept;
    // move assignment operator
    Graph& operator=(Graph&& other) noexcept;

    ~Graph();
    bool is_dag();

    void print_graph() const;

    bool add_node(const std::string& id);

    bool remove_node(const std::string& id);

    bool add_edge(const std::string& from_id, const std::string& to_id, int weight);

    bool remove_edge(const std::string& from_id, const std::string& to_id);

    bool change_edge_weight(const std::string& from_id, const std::string& to_id, int new_weight);

    Node* get_node(const std::string& id) const;

    [[nodiscard]]
    inline int get_num_nodes() const;
    // {
    //     return nodes.size();
    // }
};

#endif  // GRAPH_H
