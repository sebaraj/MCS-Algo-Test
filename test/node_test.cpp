#include "mcs/node.h"

#include "gtest/gtest.h"

// TEST(Node, Create) { EXPECT_EQ(1, 1); }

TEST(Node, Create) {
    Node node("A");
    EXPECT_EQ(node.get_id(), "A");
    EXPECT_EQ(node.get_num_parents(), 0);
    EXPECT_EQ(node.get_num_children(), 0);
    EXPECT_TRUE(node.is_source());
    EXPECT_TRUE(node.is_sink());

    std::vector<Node*> nodes;
    std::vector<std::string> ids = {"A", "B", "C", "D"};
    for (const auto& id : ids) {
        nodes.push_back(new Node(id));
    }
    // add new edge
    EXPECT_EQ(nodes[0]->add_edge(nodes[1], 5), true);
    // add existing edge with same weight
    EXPECT_EQ(nodes[0]->add_edge(nodes[1], 5), true);
    // add existing edge with different weight
    EXPECT_EQ(nodes[0]->add_edge(nodes[1], 6), false);
    // add self-loop edge
    EXPECT_EQ(nodes[0]->add_edge(nodes[0], 5), false);
    EXPECT_EQ(nodes[0]->get_num_children(), 1);
    EXPECT_EQ(nodes[0]->get_num_parents(), 0);
}
