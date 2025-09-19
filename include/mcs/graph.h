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

/**
 * @class Graph
 * @brief Represents a directed graph using an adjacency list.
 * This class provides methods to manage nodes and edges, and to check if the graph is a DAG.
 */
class Graph {
private:
    /**
     * @brief Map of node IDs to Node pointers representing the graph's nodes.
     */
    std::unordered_map<std::string, Node*> nodes;

public:
    /**
     * @brief Default constructor that initializes an empty graph.
     */
    Graph();

    /**
     * @brief Constructs a graph from a list of nodes.
     * @param node_list Vector of Node objects to initialize the graph with.
     */
    Graph(const std::vector<Node>& node_list);

    /**
     * @brief Copy constructor.
     * @param other Graph to copy from.
     */
    Graph(const Graph& other);

    /**
     * @brief Copy assignment operator.
     * @param other Graph to copy from.
     * @return Reference to the assigned Graph.
     */
    Graph& operator=(const Graph& other);

    /**
     * @brief Move constructor.
     * @param other Graph to move from.
     */
    Graph(Graph&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other Graph to move from.
     * @return Reference to the assigned Graph.
     */
    Graph& operator=(Graph&& other) noexcept;

    /**
     * @brief Destructor that cleans up dynamically allocated nodes.
     */
    ~Graph();

    /**
     * @brief Checks if the graph is a Directed Acyclic Graph (DAG).
     * @return True if the graph is a DAG, false otherwise.
     */
    bool is_dag();

    /**
     * @brief Prints the graph as node:[adjacency list].
     */
    void print_graph() const;

    /**
     * @brief Adds a node with the given ID to the graph.
     * @param id Unique identifier for the new node.
     * @return True if the node was added successfully, false if a node with the same ID already
     * exists.
     */
    bool add_node(const std::string& id);

    /**
     * @brief Adds multiple nodes with the given IDs to the graph.
     * @param ids Vector of unique identifiers for the new nodes.
     * @return True if all nodes were added successfully, false if any node with the same ID already
     * exists.
     */
    bool add_node_set(const std::vector<std::string>& ids);

    /**
     * @brief Removes the node with the given ID from the graph.
     * @param id Unique identifier of the node to remove.
     * @return True if the node was removed successfully, false if the node does not exist.
     */
    bool remove_node(const std::string& id);

    /**
     * @brief Adds a directed edge from one node to another with a specified weight.
     * @param from_id ID of the source node.
     * @param to_id ID of the destination node.
     * @param weight Weight of the edge.
     * @return True if the edge was added successfully, false if either node does not exist or the
     * edge already exists.
     */
    bool add_edge(const std::string& from_id, const std::string& to_id, int weight);

    /**
     * @brief Adds directed edges from one node to multiple other nodes with a specified weight.
     * @param from_id ID of the source node.
     * @param to_ids Vector of IDs of the children nodes.
     * @param weights Vector of weights of the edges, defaults to 0 if vector is empty.
     * @return True if all edges were added successfully, false if the source node does not exist or
     * any edge already exists.
     */
    bool add_edge_set(const std::string& from_id, const std::vector<std::string>& to_ids,
                      const std::vector<int>& weights = {});

    /**
     * @brief Removes the directed edge from one node to another.
     * @param from_id ID of the source node.
     * @param to_id ID of the destination node.
     * @return True if the edge was removed successfully, false if either node does not exist or the
     * edge does not exist.
     */
    bool remove_edge(const std::string& from_id, const std::string& to_id);

    /**
     * @brief Changes the weight of the edge from one node to another.
     * @param from_id ID of the source node.
     * @param to_id ID of the destination node.
     * @param new_weight New weight for the edge.
     * @return True if the weight was changed successfully, false if either node does not exist or
     * the edge does not exist.
     */
    bool change_edge_weight(const std::string& from_id, const std::string& to_id, int new_weight);

    /**
     * @brief Retrieves the node identified by the given ID
     * @param id Unique identifier of the node to retrieve.
     * @return Pointer to the Node if found, nullptr otherwise.
     */
    Node* get_node(const std::string& id) const;

    /**
     * @brief Retrieves the number of nodes in the graph.
     * @return The number of nodes.
     */
    [[nodiscard]]
    int get_num_nodes() const;

    /**
     * @brief Retrieves the map of nodes in the graph.
     * @return Constant reference to the unordered map of node IDs to Node pointers.
     */
    [[nodiscard]]
    const std::unordered_map<std::string, Node*>& get_nodes() const;

    /**
     * @brief Equality operator to compare two graphs.
     * @return True if the graphs are equal, false otherwise.
     */
    bool operator==(const Graph& other) const;

    /**
     * @brief Stream insertion operator to print the graph.
     * @param os Output stream.
     * @param graph Graph to print.
     * @return Reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

    /**
     * @brief Generates a DOT file representing the graph for visualization.
     * @param filename Name of the output graph name
     */
    void generate_diagram_file(const std::string& graph_name) const;
};

#endif  // GRAPH_H
