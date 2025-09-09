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

class Node;

class Node {
private:
    std::string id;
    int num_parents;
    int num_children;
    std::unordered_map<Node*, int> children;

public:
    Node(std::string id, int num_parents = 0, int num_children = 0)
        : id(id), num_parents(num_parents), num_children(num_children) {};

    Node(const Node& other)
        : id(other.id),
          num_parents(other.num_parents),
          num_children(other.num_children),
          children(other.children) {};

    // copy assignment operator
    Node& operator=(const Node& other) {
        if (this != &other) {
            id = other.id;
            num_parents = other.num_parents;
            num_children = other.num_children;
            children = other.children;
        }
        return *this;
    }

    // move operator
    Node(Node&& other) noexcept
        : id(std::move(other.id)),
          num_parents(other.num_parents),
          num_children(other.num_children),
          children(std::move(other.children)) {
        other.num_parents = 0;
        other.num_children = 0;
    }

    // move assignment operator
    Node& operator=(Node&& other) noexcept {
        if (this != &other) {
            id = std::move(other.id);
            num_parents = other.num_parents;
            num_children = other.num_children;
            children = std::move(other.children);
            other.num_parents = 0;
            other.num_children = 0;
        }
        return *this;
    }

    ~Node() = default;

    [[nodiscard]]
    inline std::string get_id() const {
        return id;
    }

    [[nodiscard]]
    inline int get_num_parents() const {
        return num_parents;
    }

    [[nodiscard]]
    inline int get_num_children() const {
        return num_children;
    }

    // returns false if node doesnt exist or edge already exists
    inline bool add_edge(Node* neighbor, int weight);

    inline bool remove_edge(Node* neighbor);

    inline bool change_edge_weight(Node* neighbor, int new_weight);

    [[nodiscard]]
    inline bool contains_edge(Node* neighbor) const {
        return children.find(neighbor) != children.end();
    }
};

class Graph {
private:
    std::unordered_map<std::string, Node*> nodes;

public:
    Graph();
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
    inline int get_num_nodes() const {
        return nodes.size();
    }
};

#endif  // GRAPH_H
