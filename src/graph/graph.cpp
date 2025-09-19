#include <mcs/graph.h>

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "time.h"

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

bool Graph::add_node_set(const std::vector<std::string>& ids) {
    bool all_added = true;
    for (const std::string& id : ids) {
        if (!add_node(id)) {
            all_added = false;
        }
    }
    return all_added;
}

bool Graph::remove_node(const std::string& id) {
    auto it = nodes.find(id);
    if (it == nodes.end()) {
        return false;
    }

    Node* node_to_remove = it->second;

    // Remove edges FROM the node being removed to other nodes
    // We need to create a copy of children to avoid modifying while iterating
    auto children_copy = node_to_remove->get_children();
    for (const auto& [child, weight] : children_copy) {
        node_to_remove->remove_edge(child);  // This properly updates parent counts
    }

    // Remove edges TO the node being removed from other nodes
    for (auto& pair : nodes) {
        Node* node = pair.second;
        if (node != node_to_remove) {  // Skip the node being removed
            const auto& children = node->get_children();
            if (children.find(node_to_remove) != children.end()) {
                node->remove_edge(node_to_remove);
            }
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

bool Graph::add_edge_set(const std::string& from_id, const std::vector<std::string>& to_ids,
                         const std::vector<int>& weights) {
    bool use_zero_weights = weights.empty() || weights.size() != to_ids.size();
    auto from_it = nodes.find(from_id);
    if (from_it == nodes.end()) {
        return false;
    }
    bool all_added = true;
    for (size_t i = 0; i < to_ids.size(); ++i) {
        int weight = use_zero_weights ? 0 : weights[i];
        auto to_it = nodes.find(to_ids[i]);
        if (to_it == nodes.end() || !from_it->second->add_edge(to_it->second, weight)) {
            all_added = false;
        }
    }
    return all_added;
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

int Graph::get_num_nodes() const { return static_cast<int>(nodes.size()); }

const std::unordered_map<std::string, Node*>& Graph::get_nodes() const { return nodes; }

bool Graph::operator==(const Graph& other) const {
    if (get_num_nodes() != other.get_num_nodes()) {
        return false;
    }

    for (const auto& pair : nodes) {
        const std::string& node_id = pair.first;
        Node* this_node = pair.second;
        Node* other_node = other.get_node(node_id);
        if (!other_node || *this_node != *other_node) {
            return false;
        }
    }

    return true;
}

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
    const auto& nodes_map = graph.get_nodes();
    std::vector<Node*> node_list(nodes_map.size());
    int idx = 0;
    for (const auto& [_, node] : nodes_map) {
        node_list[idx++] = node;
    }
    std::sort(node_list.begin(), node_list.end(),
              [](const Node* a, const Node* b) { return a->get_id() < b->get_id(); });
    for (const auto node : node_list) {
        os << *node << "\n";
    }
    return os;
}

void Graph::generate_diagram_file(const std::string& graph_name) const {
    // https://www.graphviz.org/pdf/dotguide.pdf
    std::string filename = currentDateTime() + "_" + graph_name + ".gv";
    std::string path = "diagrams/" + filename;
    std::ofstream outputFile(path);

    outputFile << "digraph G {\n";
    for (const auto& [_, node] : nodes) {
        for (const auto& [child, weight] : node->get_children()) {
            outputFile << "    " << std::quoted(node->get_id()) << " -> "
                       << std::quoted(child->get_id()) << " [label=\"" << weight << "\"];\n";
        }
    }
    outputFile << "}\n";
    outputFile.close();

    system(("dot -Tpng " + filename + " -o diagrams/" + filename + ".png").c_str());
}
