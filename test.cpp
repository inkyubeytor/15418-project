//
// Created by ancai on 4/11/2022.
//

#ifndef GRAPH
#define GRAPH
#include "lib/graph.cpp"
#endif

#include "spanning_tree.cpp"
#include "sequential.cpp"

Graph<float> get_G1() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {0, 2};
    vector<int> n2 = {0, 1, 3};
    vector<int> n3 = {2};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Graph<float> G(adj, weights);
    return G;
}

Tree<float> get_T1() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {};
    vector<int> n2 = {3};
    vector<int> n3 = {};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Tree<float> T(adj, weights, 0);
    return T;
}

int test_graph() {
    Tree<float> T = get_T1();
    vector<int> traversal = T.dfs_postorder_nodes();
    for (int x: traversal) std::cout << x << " ";
    std::cout << std::endl;
    return 0;
}

int test_spanning_tree() {
    Graph<float> G = get_G1();
    Tree<float> BFSST = bfs_st(G, 3);
    Tree<float> MST = min_st(G, 3, simple_edge_weight);
    return 0;
}

int test_sequential() {
    Tree<float> tree = get_T1();
    vector<int> partition = naive_partition(tree, 3, 0.9, 3.1);
    for (int p: partition)
        std::cout << p << std::endl;
    return 0;
}

int main() {
    printf("testing graph\n");
    test_graph();
    printf("testing spanning tree\n");
    test_spanning_tree();
    printf("testing sequential algo\n");
    test_sequential();
    return 0;
};
