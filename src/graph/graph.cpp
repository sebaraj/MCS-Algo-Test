#include <mcs/graph.h>

#include <cstddef>
#include <iostream>

Graph::Graph() = default;

Graph::Graph(const std::vector<Node>& node_list) {
    for (const auto& node : node_list) {
        add_node(node.get_id());
    }
}

Graph::Graph(const Graph& other) {
    for (const auto& pair : other.nodes) {
        nodes[pair.first] = new Node(*pair.second);
    }
}

Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        for (auto& pair : nodes) {
            delete pair.second;
        }
        nodes.clear();
        for (const auto& pair : other.nodes) {
            nodes[pair.first] = new Node(*pair.second);
        }
    }
    return *this;
}

Graph::Graph(Graph&& other) noexcept : nodes(std::move(other.nodes)) { other.nodes.clear(); }

Graph& Graph::operator=(Graph&& other) noexcept {
    if (this != &other) {
        for (auto& pair : nodes) {
            delete pair.second;
        }
        nodes = std::move(other.nodes);
        other.nodes.clear();
    }
    return *this;
}

Graph::~Graph() {
    for (auto& pair : nodes) {
        delete pair.second;
    }
}

bool Graph::is_dag() {
    std::unordered_map<std::string, int> in_degree;
    for (const auto& pair : nodes) {
        in_degree[pair.first] = pair.second->get_num_parents();
    }

    std::vector<std::string> zero_in_degree;
    for (const auto& pair : in_degree) {
        if (pair.second == 0) {
            zero_in_degree.push_back(pair.first);
        }
    }

    int visited_count = 0;
    while (!zero_in_degree.empty()) {
        std::string node_id = zero_in_degree.back();
        zero_in_degree.pop_back();
        visited_count++;

        Node* node = nodes[node_id];
        for (const auto& child_pair : node->get_children()) {
            Node* child = child_pair.first;
            in_degree[child->get_id()]--;
            if (in_degree[child->get_id()] == 0) {
                zero_in_degree.push_back(child->get_id());
            }
        }
    }

    return static_cast<size_t>(visited_count) == nodes.size();
}

void Graph::print_graph() const {
    for (const auto& pair : nodes) {
        const Node* node = pair.second;
        std::cout << node->get_id() << ": [";
        bool first = true;
        for (const auto& child_pair : node->get_children()) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << child_pair.first->get_id() << "(" << child_pair.second << ")";
            first = false;
        }
        std::cout << "]\n";
    }
}

bool Graph::add_node(const std::string& id) {
    if (nodes.find(id) != nodes.end()) {
        return false;
    }
    nodes[id] = new Node(id);
    return true;
}

bool Graph::remove_node(const std::string& id) {
    auto it = nodes.find(id);
    if (it == nodes.end()) {
        return false;
    }

    Node* node_to_remove = it->second;

    for (auto& pair : nodes) {
        Node* node = pair.second;
        const auto& children = node->get_children();
        if (children.find(node_to_remove) != children.end()) {
            node->remove_edge(node_to_remove);
            node->decrement_children();
            node_to_remove->decrement_parents();
        }
    }

    delete node_to_remove;
    nodes.erase(it);
    return true;
}

bool Graph::add_edge(const std::string& from_id, const std::string& to_id, int weight) {
    auto from_it = nodes.find(from_id);
    auto to_it = nodes.find(to_id);
    if (from_it == nodes.end() || to_it == nodes.end()) {
        return false;
    }
    return from_it->second->add_edge(to_it->second, weight);
}

bool Graph::remove_edge(const std::string& from_id, const std::string& to_id) {
    auto from_it = nodes.find(from_id);
    auto to_it = nodes.find(to_id);
    if (from_it == nodes.end() || to_it == nodes.end()) {
        return false;
    }
    return from_it->second->remove_edge(to_it->second);
}

bool Graph::change_edge_weight(const std::string& from_id, const std::string& to_id,
                               int new_weight) {
    auto from_it = nodes.find(from_id);
    auto to_it = nodes.find(to_id);
    if (from_it == nodes.end() || to_it == nodes.end()) {
        return false;
    }
    return from_it->second->change_edge_weight(to_it->second, new_weight);
}

Node* Graph::get_node(const std::string& id) const {
    auto it = nodes.find(id);
    if (it != nodes.end()) {
        return it->second;
    }
    return nullptr;
}

inline int Graph::get_num_nodes() const { return nodes.size(); }

const std::unordered_map<std::string, Node*>& Graph::get_nodes() const { return nodes; }

bool operator==(const Graph& lhs, const Graph& rhs) {
    if (lhs.get_num_nodes() != rhs.get_num_nodes()) {
        return false;
    }

    for (const auto& pair : lhs.get_nodes()) {
        const std::string& node_id = pair.first;
        Node* lhs_node = pair.second;
        Node* rhs_node = rhs.get_node(node_id);
        if (!rhs_node || *lhs_node != *rhs_node) {
            return false;
        }
    }

    return true;
}
