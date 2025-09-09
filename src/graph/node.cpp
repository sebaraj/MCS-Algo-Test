#include <mcs/node.h>

Node::Node(std::string id, int num_parents, int num_children)
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

Node::~Node() { children.clear(); }

std::string Node::get_id() const { return id; }

int Node::get_num_parents() const { return num_parents; }

int Node::get_num_children() const { return num_children; }

bool Node::add_edge(Node* neighbor, int weight) {
    if (children.find(neighbor) != children.end()) {
        return false;
    }
    children[neighbor] = weight;
    num_children++;
    neighbor->num_parents++;
    return true;
}

bool Node::remove_edge(Node* neighbor) {
    if (children.find(neighbor) == children.end()) {
        return false;
    }
    children.erase(neighbor);
    num_children--;
    neighbor->num_parents--;
    return true;
}

bool Node::change_edge_weight(Node* neighbor, int new_weight) {
    if (children.find(neighbor) == children.end()) {
        return false;
    }
    children[neighbor] = new_weight;
    return true;
}

bool Node::contains_edge(Node* neighbor) const { return children.find(neighbor) != children.end(); }

bool Node::is_source() const { return num_parents == 0; }

bool Node::is_sink() const { return num_children == 0; }

bool operator==(const Node& lhs, const Node& rhs) {
    bool same = lhs.get_num_parents() == rhs.get_num_parents() && lhs.get_id() == rhs.get_id()
                && lhs.get_num_children() == rhs.get_num_children();

    for (const auto& [child, weight] : lhs.get_children()) {
        auto rhs_children = rhs.get_children();
        auto it = rhs_children.find(child);
        if (it == rhs_children.end() || it->second != weight) {
            return false;
        }
    }
    return same;
}

bool Node::same_id(const Node& other) const { return id == other.id; }

const std::unordered_map<Node*, int>& Node::get_children() const { return children; }

void Node::decrement_parents() { num_parents--; }

void Node::decrement_children() { num_children--; }
