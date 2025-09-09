#include <mcs/node.h>

Node::Node(std::string id, int num_parents = 0, int num_children = 0)
    : id(std::move(id)), num_parents(num_parents), num_children(num_children) {};

Node::Node(const Node& other)
    : id(other.id),
      num_parents(other.num_parents),
      num_children(other.num_children),
      children(other.children) {};

Node& Node::operator=(const Node& other) {
    if (this != &other) {
        id = other.id;
        num_parents = other.num_parents;
        num_children = other.num_children;
        children = other.children;
    }
    return *this;
}

Node::Node(Node&& other) noexcept
    : id(std::move(other.id)),
      num_parents(other.num_parents),
      num_children(other.num_children),
      children(std::move(other.children)) {
    other.num_parents = 0;
    other.num_children = 0;
}

Node& Node::operator=(Node&& other) noexcept {
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

Node::~Node() = default;

inline std::string Node::get_id() const { return id; }

inline int Node::get_num_parents() const { return num_parents; }

inline int Node::get_num_children() const { return num_children; }

inline bool Node::add_edge(Node* neighbor, int weight) {
    if (children.find(neighbor) != children.end()) {
        return false;
    }
    children[neighbor] = weight;
    num_children++;
    neighbor->num_parents++;
    return true;
}

inline bool Node::remove_edge(Node* neighbor) {
    if (children.find(neighbor) == children.end()) {
        return false;
    }
    children.erase(neighbor);
    num_children--;
    neighbor->num_parents--;
    return true;
}

inline bool Node::change_edge_weight(Node* neighbor, int new_weight) {
    if (children.find(neighbor) == children.end()) {
        return false;
    }
    children[neighbor] = new_weight;
    return true;
}

inline bool Node::contains_edge(Node* neighbor) const {
    return children.find(neighbor) != children.end();
}

inline bool Node::is_source() const { return num_parents == 0; }

inline bool Node::is_sink() const { return num_children == 0; }
