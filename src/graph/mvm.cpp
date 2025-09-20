#include <mcis/graph.h>

Graph Graph::create_mvm_graph_from_mat_vec(const std::vector<std::vector<std::string>>& mat,
                                           const std::vector<std::string>& vec) {
    Graph graph;
    int m = static_cast<int>(mat.size());
    int n = static_cast<int>(vec.size());
    if (m <= 0 || n <= 0 || mat[0].size() != static_cast<size_t>(n)) {
        return graph;
    }

    // Reserve space for better performance
    graph.reserve_nodes(m * n + n + m * n + m * (n - 1));

    // S1: Add input nodes (matrix elements and vector elements)
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            graph.add_node(mat[i][j]);
        }
    }
    for (int j = 0; j < n; ++j) {
        graph.add_node(vec[j]);
    }

    // S2: Add product nodes
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::string product_node = "p" + std::to_string(i) + "," + std::to_string(j);
            graph.add_node(product_node);
        }
    }

    // S3 to S_{n+1}: Add accumulation nodes
    for (int set = 3; set <= n + 1; ++set) {
        for (int i = 0; i < m; ++i) {
            std::string acc_node = "acc" + std::to_string(set) + "," + std::to_string(i);
            graph.add_node(acc_node);
        }
    }

    // Rule 1: Edges from S1 inputs to S2 products
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::string product_node = "p" + std::to_string(i) + "," + std::to_string(j);
            // Matrix element to product
            graph.add_edge(mat[i][j], product_node, 0);
            // Vector element to product
            graph.add_edge(vec[j], product_node, 0);
        }
    }

    // Rule 2: Edges from S2 products to S3 accumulation
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::string product_node = "p" + std::to_string(i) + "," + std::to_string(j);
            std::string acc_node = "acc3," + std::to_string(i);
            graph.add_edge(product_node, acc_node, 0);
        }
    }

    // Rule 3: Edges between accumulation sets (S3 -> S4 -> ... -> S_{n+1})
    for (int set = 3; set < n + 1; ++set) {
        for (int i = 0; i < m; ++i) {
            std::string from_node = "acc" + std::to_string(set) + "," + std::to_string(i);
            std::string to_node = "acc" + std::to_string(set + 1) + "," + std::to_string(i);
            graph.add_edge(from_node, to_node, 0);
        }
    }

    return graph;
}

Graph Graph::create_mvm_graph_from_dimensions(int m, int n) {
    if (m <= 0 || n <= 0) {
        return Graph();
    }
    std::vector<std::vector<std::string>> mat(m, std::vector<std::string>(n));
    std::vector<std::string> vec(n);

    const bool use_parallel = (m * n >= MVM_PARALLEL_THRESHOLD);

    if (use_parallel) {
#pragma omp parallel for collapse(2)
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                mat[i][j] = "m" + std::to_string(i) + "," + std::to_string(j);
            }
        }

#pragma omp parallel for
        for (int j = 0; j < n; ++j) {
            vec[j] = "v" + std::to_string(j);
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                mat[i][j] = "m" + std::to_string(i) + "," + std::to_string(j);
            }
        }

        for (int j = 0; j < n; ++j) {
            vec[j] = "v" + std::to_string(j);
        }
    }

    return create_mvm_graph_from_mat_vec(mat, vec);
}
