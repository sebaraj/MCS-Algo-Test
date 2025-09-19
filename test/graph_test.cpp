#include "mcs/graph.h"
#include "mcs/node.h"

#include <algorithm>
#include <memory>
#include <sstream>
#include <unordered_set>

#include "gtest/gtest.h"

class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        graph = std::make_unique<Graph>();
    }

    void TearDown() override {
        graph.reset();
    }

    std::unique_ptr<Graph> graph;
};

// Test 1: Default constructor and initial state
TEST_F(GraphTest, DefaultConstructorAndInitialState) {
    EXPECT_EQ(graph->get_num_nodes(), 0);
    EXPECT_TRUE(graph->get_nodes().empty());
    EXPECT_TRUE(graph->is_dag()); // Empty graph is a DAG
}

// Test 2: Vector constructor
TEST_F(GraphTest, VectorConstructor) {
    std::vector<Node> nodes;
    nodes.emplace_back("A");
    nodes.emplace_back("B");
    nodes.emplace_back("C");

    Graph graph_from_vector(nodes);

    EXPECT_EQ(graph_from_vector.get_num_nodes(), 3);
    EXPECT_NE(graph_from_vector.get_node("A"), nullptr);
    EXPECT_NE(graph_from_vector.get_node("B"), nullptr);
    EXPECT_NE(graph_from_vector.get_node("C"), nullptr);
    EXPECT_EQ(graph_from_vector.get_node("D"), nullptr);
}

// Test 3: Copy constructor with empty graph
TEST_F(GraphTest, CopyConstructorEmpty) {
    Graph copied_graph(*graph);

    EXPECT_EQ(copied_graph.get_num_nodes(), 0);
    EXPECT_TRUE(copied_graph.get_nodes().empty());
    EXPECT_TRUE(*graph == copied_graph);
}

// Test 4: Copy constructor with populated graph
TEST_F(GraphTest, CopyConstructorPopulated) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_node("C");
    graph->add_edge("A", "B", 10);
    graph->add_edge("B", "C", 20);

    Graph copied_graph(*graph);

    EXPECT_EQ(copied_graph.get_num_nodes(), 3);
    EXPECT_TRUE(*graph == copied_graph);

    // Verify independence - changes to original don't affect copy
    graph->add_node("D");
    EXPECT_NE(copied_graph.get_num_nodes(), graph->get_num_nodes());
    EXPECT_FALSE(*graph == copied_graph);
}

// Test 5: Copy assignment operator
TEST_F(GraphTest, CopyAssignmentOperator) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_edge("A", "B", 15);

    Graph assigned_graph;
    assigned_graph.add_node("X"); // Pre-populate to test cleanup

    assigned_graph = *graph;

    EXPECT_EQ(assigned_graph.get_num_nodes(), 2);
    EXPECT_TRUE(*graph == assigned_graph);
    EXPECT_EQ(assigned_graph.get_node("X"), nullptr); // Old node should be gone

    // Test self-assignment
    assigned_graph = assigned_graph;
    EXPECT_EQ(assigned_graph.get_num_nodes(), 2);
}

// Test 6: Move constructor
TEST_F(GraphTest, MoveConstructor) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_edge("A", "B", 25);

    int original_node_count = graph->get_num_nodes();

    Graph moved_graph(std::move(*graph));

    EXPECT_EQ(moved_graph.get_num_nodes(), original_node_count);
    EXPECT_EQ(graph->get_num_nodes(), 0); // Original should be empty
    EXPECT_NE(moved_graph.get_node("A"), nullptr);
    EXPECT_NE(moved_graph.get_node("B"), nullptr);
    EXPECT_EQ(graph->get_node("A"), nullptr);
}

// Test 7: Move assignment operator
TEST_F(GraphTest, MoveAssignmentOperator) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_edge("A", "B", 30);

    Graph assigned_graph;
    assigned_graph.add_node("Y"); // Pre-populate

    assigned_graph = std::move(*graph);

    EXPECT_EQ(assigned_graph.get_num_nodes(), 2);
    EXPECT_EQ(graph->get_num_nodes(), 0);
    EXPECT_NE(assigned_graph.get_node("A"), nullptr);
    EXPECT_EQ(assigned_graph.get_node("Y"), nullptr); // Old node cleaned up
}

// Test 8: Adding single nodes - success cases
TEST_F(GraphTest, AddSingleNodeSuccess) {
    EXPECT_TRUE(graph->add_node("Node1"));
    EXPECT_EQ(graph->get_num_nodes(), 1);
    EXPECT_NE(graph->get_node("Node1"), nullptr);

    EXPECT_TRUE(graph->add_node("Node2"));
    EXPECT_EQ(graph->get_num_nodes(), 2);

    // Test with empty string (should work)
    EXPECT_TRUE(graph->add_node(""));
    EXPECT_NE(graph->get_node(""), nullptr);

    // Test with special characters
    EXPECT_TRUE(graph->add_node("Node@#$%"));
    EXPECT_NE(graph->get_node("Node@#$%"), nullptr);
}

// Test 9: Adding single nodes - failure cases
TEST_F(GraphTest, AddSingleNodeFailure) {
    EXPECT_TRUE(graph->add_node("Duplicate"));
    EXPECT_FALSE(graph->add_node("Duplicate")); // Should fail
    EXPECT_EQ(graph->get_num_nodes(), 1);
}

// Test 10: Adding multiple nodes
TEST_F(GraphTest, AddMultipleNodes) {
    std::vector<std::string> node_ids = {"A", "B", "C", "D", "E"};

    EXPECT_TRUE(graph->add_node_set(node_ids));
    EXPECT_EQ(graph->get_num_nodes(), 5);

    for (const auto& id : node_ids) {
        EXPECT_NE(graph->get_node(id), nullptr);
    }

    // Test partial failure - some duplicates
    std::vector<std::string> mixed_ids = {"A", "F", "B", "G"}; // A and B already exist
    EXPECT_FALSE(graph->add_node_set(mixed_ids)); // Should return false due to duplicates
    EXPECT_EQ(graph->get_num_nodes(), 7); // F and G should still be added
    EXPECT_NE(graph->get_node("F"), nullptr);
    EXPECT_NE(graph->get_node("G"), nullptr);
}

// Test 11: Removing nodes - success cases
TEST_F(GraphTest, RemoveNodeSuccess) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_node("C");
    graph->add_edge("A", "B", 10);
    graph->add_edge("B", "C", 20);

    // Remove middle node - should clean up all edges
    EXPECT_TRUE(graph->remove_node("B"));
    EXPECT_EQ(graph->get_num_nodes(), 2);
    EXPECT_EQ(graph->get_node("B"), nullptr);

    // Verify edges are cleaned up
    Node* nodeA = graph->get_node("A");
    Node* nodeC = graph->get_node("C");
    EXPECT_EQ(nodeA->get_num_children(), 0);
    EXPECT_EQ(nodeC->get_num_parents(), 0);
}

// Test 12: Removing nodes - failure cases
TEST_F(GraphTest, RemoveNodeFailure) {
    EXPECT_FALSE(graph->remove_node("NonExistent"));

    graph->add_node("A");
    EXPECT_TRUE(graph->remove_node("A"));
    EXPECT_FALSE(graph->remove_node("A")); // Already removed
}

// Test 13: Adding edges - success cases
TEST_F(GraphTest, AddEdgeSuccess) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_node("C");

    EXPECT_TRUE(graph->add_edge("A", "B", 5));
    EXPECT_TRUE(graph->add_edge("B", "C", 10));
    EXPECT_TRUE(graph->add_edge("A", "C", 15));

    // Verify edge properties
    Node* nodeA = graph->get_node("A");
    Node* nodeB = graph->get_node("B");
    Node* nodeC = graph->get_node("C");

    EXPECT_TRUE(nodeA->contains_edge(nodeB));
    EXPECT_TRUE(nodeA->contains_edge(nodeC));
    EXPECT_TRUE(nodeB->contains_edge(nodeC));
    EXPECT_EQ(nodeB->get_num_parents(), 1);
    EXPECT_EQ(nodeC->get_num_parents(), 2);
}

// Test 14: Adding edges - failure cases
TEST_F(GraphTest, AddEdgeFailure) {
    graph->add_node("A");
    graph->add_node("B");

    // Non-existent source node
    EXPECT_FALSE(graph->add_edge("X", "A", 5));

    // Non-existent destination node
    EXPECT_FALSE(graph->add_edge("A", "Y", 5));

    // Both nodes non-existent
    EXPECT_FALSE(graph->add_edge("X", "Y", 5));

    // Add valid edge first
    EXPECT_TRUE(graph->add_edge("A", "B", 10));

    // Try to add same edge with different weight (should fail based on Node implementation)
    EXPECT_FALSE(graph->add_edge("A", "B", 20));
}

// Test 15: Adding multiple edges
TEST_F(GraphTest, AddMultipleEdges) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_node("C");
    graph->add_node("D");

    std::vector<std::string> targets = {"B", "C", "D"};
    std::vector<int> weights = {10, 20, 30};

    EXPECT_TRUE(graph->add_edge_set("A", targets, weights));

    Node* nodeA = graph->get_node("A");
    EXPECT_EQ(nodeA->get_num_children(), 3);

    // Test with default weights (zeros)
    graph->add_node("E");
    graph->add_node("F");
    std::vector<std::string> targets2 = {"E", "F"};

    EXPECT_TRUE(graph->add_edge_set("B", targets2)); // No weights provided

    Node* nodeB = graph->get_node("B");
    const auto& children = nodeB->get_children();
    EXPECT_EQ(children.at(graph->get_node("E")), 0);
    EXPECT_EQ(children.at(graph->get_node("F")), 0);
}

// Test 16: Removing edges
TEST_F(GraphTest, RemoveEdge) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_node("C");
    graph->add_edge("A", "B", 10);
    graph->add_edge("A", "C", 20);

    EXPECT_TRUE(graph->remove_edge("A", "B"));

    Node* nodeA = graph->get_node("A");
    Node* nodeB = graph->get_node("B");

    EXPECT_FALSE(nodeA->contains_edge(nodeB));
    EXPECT_EQ(nodeA->get_num_children(), 1);
    EXPECT_EQ(nodeB->get_num_parents(), 0);

    // Try to remove non-existent edge
    EXPECT_FALSE(graph->remove_edge("B", "C"));

    // Try to remove with non-existent nodes
    EXPECT_FALSE(graph->remove_edge("X", "Y"));
}

// Test 17: Changing edge weights
TEST_F(GraphTest, ChangeEdgeWeight) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_edge("A", "B", 10);

    EXPECT_TRUE(graph->change_edge_weight("A", "B", 50));

    Node* nodeA = graph->get_node("A");
    Node* nodeB = graph->get_node("B");
    const auto& children = nodeA->get_children();

    EXPECT_EQ(children.at(nodeB), 50);

    // Try to change non-existent edge
    EXPECT_FALSE(graph->change_edge_weight("B", "A", 100));

    // Try with non-existent nodes
    EXPECT_FALSE(graph->change_edge_weight("X", "Y", 100));
}

// Test 18: DAG detection - valid DAGs
TEST_F(GraphTest, DAGDetectionValid) {
    // Empty graph is a DAG
    EXPECT_TRUE(graph->is_dag());

    // Single node is a DAG
    graph->add_node("A");
    EXPECT_TRUE(graph->is_dag());

    // Linear chain is a DAG
    graph->add_node("B");
    graph->add_node("C");
    graph->add_edge("A", "B", 1);
    graph->add_edge("B", "C", 1);
    EXPECT_TRUE(graph->is_dag());

    // Tree structure is a DAG
    graph->add_node("D");
    graph->add_node("E");
    graph->add_edge("A", "D", 1);
    graph->add_edge("A", "E", 1);
    EXPECT_TRUE(graph->is_dag());
}

// Test 19: DAG detection - cycles
TEST_F(GraphTest, DAGDetectionCycles) {
    graph->add_node("A");
    graph->add_node("B");
    graph->add_node("C");

    // Create a cycle: A -> B -> C -> A
    graph->add_edge("A", "B", 1);
    graph->add_edge("B", "C", 1);
    graph->add_edge("C", "A", 1);

    EXPECT_FALSE(graph->is_dag());

    // Remove one edge to break cycle
    graph->remove_edge("C", "A");
    EXPECT_TRUE(graph->is_dag());

    // Test self-loop - should fail to add, so graph remains DAG
    EXPECT_FALSE(graph->add_edge("B", "B", 1)); // Self-loop should fail
    EXPECT_TRUE(graph->is_dag()); // Should still be DAG
}

// Test 20: Complex graph operations and stress testing
TEST_F(GraphTest, ComplexGraphOperationsAndStressTesting) {
    // Create a complex graph with 100 nodes
    const int num_nodes = 100;
    std::vector<std::string> node_ids;

    for (int i = 0; i < num_nodes; ++i) {
        std::string id = "Node" + std::to_string(i);
        node_ids.push_back(id);
        EXPECT_TRUE(graph->add_node(id));
    }

    EXPECT_EQ(graph->get_num_nodes(), num_nodes);

    // Add edges in a hierarchical pattern to maintain DAG property
    for (int i = 0; i < num_nodes - 1; ++i) {
        for (int j = i + 1; j < std::min(i + 5, num_nodes); ++j) {
            EXPECT_TRUE(graph->add_edge("Node" + std::to_string(i),
                                      "Node" + std::to_string(j),
                                      i + j));
        }
    }

    // Verify it's still a DAG
    EXPECT_TRUE(graph->is_dag());

    // Test bulk operations
    std::vector<std::string> new_nodes = {"Extra1", "Extra2", "Extra3"};
    EXPECT_TRUE(graph->add_node_set(new_nodes));
    EXPECT_EQ(graph->get_num_nodes(), num_nodes + 3);

    // Connect new nodes
    std::vector<std::string> targets = {"Extra2", "Extra3"};
    std::vector<int> weights = {100, 200};
    EXPECT_TRUE(graph->add_edge_set("Extra1", targets, weights));

    // Remove some nodes and verify cleanup
    EXPECT_TRUE(graph->remove_node("Node50"));
    EXPECT_EQ(graph->get_num_nodes(), num_nodes + 2); // 102 total
    EXPECT_EQ(graph->get_node("Node50"), nullptr);

    // Verify graph integrity after removal
    for (const auto& [id, node] : graph->get_nodes()) {
        EXPECT_FALSE(node->contains_edge(nullptr));
        for (const auto& [child, weight] : node->get_children()) {
            EXPECT_NE(child, nullptr);
            EXPECT_NE(graph->get_node(child->get_id()), nullptr);
        }
    }
}
