/**
 * @file
 * @author Bryan SebaRaj <bryan.sebaraj@yale.edu>
 * @version 1.0
 * @section DESCRIPTION
 */

#ifndef NODE_H
#define NODE_H

#include <string>
#include <unordered_map>

class Node {
private:
    std::string id;
    int num_parents;
    int num_children;
    std::unordered_map<Node*, int> children;

public:
    Node(std::string id, int num_parents = 0, int num_children = 0);

    Node(const Node& other);

    Node& operator=(const Node& other);

    Node(Node&& other) noexcept;

    Node& operator=(Node&& other) noexcept;

    ~Node() = default;

    [[nodiscard]]
    inline std::string get_id() const;

    [[nodiscard]]
    inline int get_num_parents() const;

    [[nodiscard]]
    inline int get_num_children() const;

    // returns false if node doesnt exist or edge already exists
    inline bool add_edge(Node* neighbor, int weight);

    inline bool remove_edge(Node* neighbor);

    inline bool change_edge_weight(Node* neighbor, int new_weight);

    [[nodiscard]]
    inline bool contains_edge(Node* neighbor) const;
};

#endif  // NODE_H
