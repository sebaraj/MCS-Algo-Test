#include "mcs/node.h"

#include <limits>
#include <memory>
#include <sstream>

#include "gtest/gtest.h"

class NodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        node_a = std::make_unique<Node>("A");
        node_b = std::make_unique<Node>("B");
        node_c = std::make_unique<Node>("C");
        node_d = std::make_unique<Node>("D");
    }

    void TearDown() override {
        // Clean up any edges to avoid memory issues
        node_a.reset();
        node_b.reset();
        node_c.reset();
        node_d.reset();
    }

    std::unique_ptr<Node> node_a;
    std::unique_ptr<Node> node_b;
    std::unique_ptr<Node> node_c;
    std::unique_ptr<Node> node_d;
};

// Test 1: Basic constructor and initial state
TEST_F(NodeTest, ConstructorAndInitialState) {
    Node node("TestNode");
    EXPECT_EQ(node.get_id(), "TestNode");
    EXPECT_EQ(node.get_num_parents(), 0);
    EXPECT_EQ(node.get_num_children(), 0);
    EXPECT_TRUE(node.is_source());
    EXPECT_TRUE(node.is_sink());
    EXPECT_TRUE(node.get_children().empty());
}

// Test 2: Copy constructor
TEST_F(NodeTest, CopyConstructor) {
    node_a->add_edge(node_b.get(), 10);
    node_a->add_edge(node_c.get(), 20);

    Node copied_node(*node_a);

    EXPECT_EQ(copied_node.get_id(), node_a->get_id());
    EXPECT_EQ(copied_node.get_num_parents(), node_a->get_num_parents());
    EXPECT_EQ(copied_node.get_num_children(), node_a->get_num_children());
    EXPECT_EQ(copied_node.get_children().size(), node_a->get_children().size());

    // Verify edges are copied correctly
    EXPECT_TRUE(copied_node.contains_edge(node_b.get()));
    EXPECT_TRUE(copied_node.contains_edge(node_c.get()));
}

// Test 3: Copy assignment operator
TEST_F(NodeTest, CopyAssignment) {
    node_a->add_edge(node_b.get(), 15);
    Node assigned_node("Different");

    assigned_node = *node_a;

    EXPECT_EQ(assigned_node.get_id(), node_a->get_id());
    EXPECT_EQ(assigned_node.get_num_children(), node_a->get_num_children());
    EXPECT_TRUE(assigned_node.contains_edge(node_b.get()));

    // Test self-assignment
    assigned_node = assigned_node;
    EXPECT_EQ(assigned_node.get_id(), node_a->get_id());
}

// Test 4: Move constructor
TEST_F(NodeTest, MoveConstructor) {
    node_a->add_edge(node_b.get(), 25);
    std::string original_id = node_a->get_id();
    int original_children = node_a->get_num_children();

    Node moved_node(std::move(*node_a));

    EXPECT_EQ(moved_node.get_id(), original_id);
    EXPECT_EQ(moved_node.get_num_children(), original_children);
    EXPECT_TRUE(moved_node.contains_edge(node_b.get()));

    // Original node should be in moved-from state
    EXPECT_EQ(node_a->get_num_parents(), 0);
    EXPECT_EQ(node_a->get_num_children(), 0);
}

// Test 5: Move assignment operator
TEST_F(NodeTest, MoveAssignment) {
    node_a->add_edge(node_b.get(), 30);
    Node assigned_node("Target");
    std::string original_id = node_a->get_id();

    assigned_node = std::move(*node_a);

    EXPECT_EQ(assigned_node.get_id(), original_id);
    EXPECT_TRUE(assigned_node.contains_edge(node_b.get()));

    // Test self-assignment (though it's undefined behavior, we should handle it gracefully)
    Node self_assign("Self");
    Node& ref = self_assign;
    self_assign = std::move(ref);
    EXPECT_EQ(self_assign.get_id(), "Self");
}

// Test 6: Adding edges - success cases
TEST_F(NodeTest, AddEdgeSuccess) {
    // Add new edge
    EXPECT_TRUE(node_a->add_edge(node_b.get(), 5));
    EXPECT_EQ(node_a->get_num_children(), 1);
    EXPECT_EQ(node_b->get_num_parents(), 1);
    EXPECT_FALSE(node_a->is_sink());
    EXPECT_FALSE(node_b->is_source());

    // Add edge with same weight (should succeed)
    EXPECT_TRUE(node_a->add_edge(node_b.get(), 5));
    EXPECT_EQ(node_a->get_num_children(), 1);  // Should not increment again

    // Add different edges
    EXPECT_TRUE(node_a->add_edge(node_c.get(), 10));
    EXPECT_TRUE(node_a->add_edge(node_d.get(), 15));
    EXPECT_EQ(node_a->get_num_children(), 3);
}

// Test 7: Adding edges - failure cases
TEST_F(NodeTest, AddEdgeFailure) {
    // Add edge with existing neighbor but different weight
    node_a->add_edge(node_b.get(), 5);
    EXPECT_FALSE(node_a->add_edge(node_b.get(), 10));  // Should fail

    // Self-loop should fail
    EXPECT_FALSE(node_a->add_edge(node_a.get(), 5));
    EXPECT_EQ(node_a->get_num_children(), 1);  // Should still be 1 from previous test
}

// Test 8: Removing edges
TEST_F(NodeTest, RemoveEdge) {
    // Setup edges
    node_a->add_edge(node_b.get(), 5);
    node_a->add_edge(node_c.get(), 10);

    // Remove existing edge
    EXPECT_TRUE(node_a->remove_edge(node_b.get()));
    EXPECT_EQ(node_a->get_num_children(), 1);
    EXPECT_EQ(node_b->get_num_parents(), 0);
    EXPECT_FALSE(node_a->contains_edge(node_b.get()));

    // Try to remove non-existent edge
    EXPECT_FALSE(node_a->remove_edge(node_d.get()));
    EXPECT_EQ(node_a->get_num_children(), 1);  // Should remain unchanged

    // Remove last edge
    EXPECT_TRUE(node_a->remove_edge(node_c.get()));
    EXPECT_EQ(node_a->get_num_children(), 0);
    EXPECT_TRUE(node_a->is_sink());
}

// Test 9: Changing edge weights
TEST_F(NodeTest, ChangeEdgeWeight) {
    node_a->add_edge(node_b.get(), 5);

    EXPECT_TRUE(node_a->change_edge_weight(node_b.get(), 15));
    auto& children = node_a->get_children();
    EXPECT_EQ(children.at(node_b.get()), 15);

    EXPECT_FALSE(node_a->change_edge_weight(node_c.get(), 20));

    // Change to negative weight (should be allowed)
    EXPECT_TRUE(node_a->change_edge_weight(node_b.get(), -5));
    EXPECT_EQ(children.at(node_b.get()), -5);
}

// Test 10: Edge queries and contains_edge
TEST_F(NodeTest, EdgeQueries) {
    node_a->add_edge(node_b.get(), 5);
    node_a->add_edge(node_c.get(), 10);

    // Test contains_edge
    EXPECT_TRUE(node_a->contains_edge(node_b.get()));
    EXPECT_TRUE(node_a->contains_edge(node_c.get()));
    EXPECT_FALSE(node_a->contains_edge(node_d.get()));

    // Test get_children
    const auto& children = node_a->get_children();
    EXPECT_EQ(children.size(), 2u);
    EXPECT_EQ(children.at(node_b.get()), 5);
    EXPECT_EQ(children.at(node_c.get()), 10);
}

// Test 11: Source and sink node identification
TEST_F(NodeTest, SourceAndSinkIdentification) {
    EXPECT_TRUE(node_a->is_source() && node_a->is_sink());

    node_a->add_edge(node_b.get(), 5);
    EXPECT_TRUE(node_a->is_source());
    EXPECT_FALSE(node_a->is_sink());
    EXPECT_FALSE(node_b->is_source());
    EXPECT_TRUE(node_b->is_sink());

    node_b->add_edge(node_c.get(), 10);
    EXPECT_FALSE(node_b->is_source());
    EXPECT_FALSE(node_b->is_sink());
    EXPECT_TRUE(node_a->is_source());
    EXPECT_TRUE(node_c->is_sink());
}

// Test 12: Equality operator
TEST_F(NodeTest, EqualityOperator) {
    Node node1("Equal");
    Node node2("Equal");
    Node node3("Different");

    // Same ID, same state
    EXPECT_TRUE(node1 == node2);
    EXPECT_FALSE(node1 == node3);

    // Same ID, different edges
    Node helper("Helper");
    node1.add_edge(&helper, 5);
    EXPECT_FALSE(node1 == node2);

    // Same ID, same edges
    node2.add_edge(&helper, 5);
    EXPECT_TRUE(node1 == node2);

    // Same ID, different edge weights
    Node node4("Equal");
    node4.add_edge(&helper, 10);
    EXPECT_FALSE(node1 == node4);
}

// Test 13: ID comparison
TEST_F(NodeTest, SameIdComparison) {
    Node node1("SameID");
    Node node2("SameID");
    Node node3("DifferentID");

    EXPECT_TRUE(node1.same_id(node2));
    EXPECT_FALSE(node1.same_id(node3));

    // Should work regardless of node state
    Node helper("Helper");
    node1.add_edge(&helper, 5);
    EXPECT_TRUE(node1.same_id(node2));
}

// Test 14: Stream output operator
TEST_F(NodeTest, StreamOutputOperator) {
    std::ostringstream oss;

    oss << *node_a;
    std::string output = oss.str();
    EXPECT_TRUE(output.find("A -> { }") != std::string::npos);

    node_a->add_edge(node_b.get(), 5);
    node_a->add_edge(node_c.get(), 10);

    oss.str("");
    oss << *node_a;
    output = oss.str();

    EXPECT_TRUE(output.find("A ->") != std::string::npos);
    EXPECT_TRUE(output.find("\"B\"(5)") != std::string::npos);
    EXPECT_TRUE(output.find("\"C\"(10)") != std::string::npos);
}

// Test 15: Complex graph scenarios and stress testing
TEST_F(NodeTest, ComplexGraphScenarios) {
    std::vector<std::unique_ptr<Node>> nodes;
    for (int i = 0; i < 10; ++i) {
        nodes.push_back(std::make_unique<Node>("Node" + std::to_string(i)));
    }

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i != j) {
                nodes[i]->add_edge(nodes[j].get(), i * 10 + j);
            }
        }
    }

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(nodes[i]->get_num_children(), 4);
        EXPECT_EQ(nodes[i]->get_num_parents(), 4);
        EXPECT_FALSE(nodes[i]->is_source());
        EXPECT_FALSE(nodes[i]->is_sink());
    }

    auto start_node = nodes[5].get();
    for (int i = 6; i < 10; ++i) {
        EXPECT_TRUE(start_node->add_edge(nodes[i].get(), i));
    }
    EXPECT_EQ(start_node->get_num_children(), 4);

    for (int i = 6; i < 10; ++i) {
        EXPECT_TRUE(start_node->remove_edge(nodes[i].get()));
    }
    EXPECT_EQ(start_node->get_num_children(), 0);
    EXPECT_TRUE(start_node->is_sink());
}

// Test 16: Memory management and dangling pointer safety
TEST_F(NodeTest, MemoryManagementAndDanglingPointers) {
    Node* temp_node = new Node("TempNode");
    node_a->add_edge(temp_node, 42);

    EXPECT_TRUE(node_a->contains_edge(temp_node));
    EXPECT_EQ(node_a->get_num_children(), 1);
    EXPECT_EQ(temp_node->get_num_parents(), 1);

    // Verify edge weight is correct
    const auto& children = node_a->get_children();
    EXPECT_EQ(children.at(temp_node), 42);

    // Remove edge before deleting the node to maintain graph integrity
    EXPECT_TRUE(node_a->remove_edge(temp_node));
    EXPECT_EQ(node_a->get_num_children(), 0);
    EXPECT_EQ(temp_node->get_num_parents(), 0);

    delete temp_node;

    // Verify node_a is now a sink after edge removal
    EXPECT_TRUE(node_a->is_sink());
    EXPECT_TRUE(node_a->get_children().empty());
}

// Test 17: Edge weight extremes and boundary conditions
TEST_F(NodeTest, EdgeWeightExtremesAndBoundaryConditions) {
    // Test with maximum and minimum integer values
    const int max_weight = std::numeric_limits<int>::max();
    const int min_weight = std::numeric_limits<int>::min();
    const int zero_weight = 0;

    // Add edges with extreme weights
    EXPECT_TRUE(node_a->add_edge(node_b.get(), max_weight));
    EXPECT_TRUE(node_a->add_edge(node_c.get(), min_weight));
    EXPECT_TRUE(node_a->add_edge(node_d.get(), zero_weight));

    // Verify weights are stored correctly
    const auto& children = node_a->get_children();
    EXPECT_EQ(children.at(node_b.get()), max_weight);
    EXPECT_EQ(children.at(node_c.get()), min_weight);
    EXPECT_EQ(children.at(node_d.get()), zero_weight);

    // Test changing to extreme values
    EXPECT_TRUE(node_a->change_edge_weight(node_b.get(), min_weight));
    EXPECT_TRUE(node_a->change_edge_weight(node_c.get(), max_weight));
    EXPECT_TRUE(node_a->change_edge_weight(node_d.get(), -1));

    // Verify changes
    EXPECT_EQ(children.at(node_b.get()), min_weight);
    EXPECT_EQ(children.at(node_c.get()), max_weight);
    EXPECT_EQ(children.at(node_d.get()), -1);

    // Test parent/child counts remain consistent
    EXPECT_EQ(node_a->get_num_children(), 3);
    EXPECT_EQ(node_b->get_num_parents(), 1);
    EXPECT_EQ(node_c->get_num_parents(), 1);
    EXPECT_EQ(node_d->get_num_parents(), 1);
}

// Test 18: Comprehensive equality and comparison edge cases
TEST_F(NodeTest, ComprehensiveEqualityAndComparisonEdgeCases) {
    // Create identical nodes with complex edge structures
    Node node1("Complex");
    Node node2("Complex");
    Node helper1("Helper1");
    Node helper2("Helper2");
    Node helper3("Helper3");

    // Build identical structures
    node1.add_edge(&helper1, 10);
    node1.add_edge(&helper2, -5);
    node1.add_edge(&helper3, 0);

    node2.add_edge(&helper1, 10);
    node2.add_edge(&helper2, -5);
    node2.add_edge(&helper3, 0);

    // Manually set parent counts to match (simulating real graph scenario)
    // This tests the equality operator's comprehensiveness
    EXPECT_TRUE(node1 == node2);

    // Test inequality with different edge weights
    EXPECT_TRUE(node1.change_edge_weight(&helper1, 15));
    EXPECT_FALSE(node1 == node2);

    // Restore and test inequality with different edge targets
    EXPECT_TRUE(node1.change_edge_weight(&helper1, 10));
    EXPECT_TRUE(node1 == node2); // Should be equal again

    // Add edge to only one node
    Node helper4("Helper4");
    node1.add_edge(&helper4, 100);
    EXPECT_FALSE(node1 == node2);

    // Test same_id method with complex scenarios
    Node different_id("Different");
    different_id.add_edge(&helper1, 10);
    different_id.add_edge(&helper2, -5);
    different_id.add_edge(&helper3, 0);

    EXPECT_TRUE(node1.same_id(node2));
    EXPECT_FALSE(node1.same_id(different_id));

    // Test with empty vs non-empty nodes of same ID
    Node empty1("Empty");
    Node empty2("Empty");
    helper1.add_edge(&empty1, 1);

    EXPECT_TRUE(empty1.same_id(empty2));
    EXPECT_FALSE(empty1 == empty2); // Different parent counts
}
