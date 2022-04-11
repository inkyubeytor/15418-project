//
// Created by ancai on 4/11/2022.
//

#include <iostream>
#include <unordered_map>
using std::unordered_map;
#include <set>
using std::set;
#include <stack>
using std::stack;
#include <numeric>

#ifndef GRAPH
#define GRAPH
#include "lib/graph.cpp"
#endif

#include "sample_graphs.cpp"
#include "spanning_tree.cpp"
#include "sequential.cpp"


int test_graph() {
    Tree<float> T = get_T0();
    vector<int> traversal = T.dfs_postorder_nodes();
    for (int x: traversal) std::cout << x << " ";
    std::cout << std::endl;
    return 0;
}

int test_spanning_tree() {
    Graph<float> G0 = get_G0();
    Tree<float> BFSST = bfs_st(G0, 3);
    Tree<float> MST = min_st(G0, 3, simple_edge_weight);
    Tree<float> rand_ST = min_st(G0, 3, rand_edge_weight);
    return 0;
}

bool check_connectivity(Graph<float> G, set<int> nodes) {
    bool visited[G.size()];
    std::fill_n(visited, G.size(), false);
    int root = *nodes.begin();
    stack<int> frontier;
    frontier.push(root);
    visited[root] = true;
    int num_visited = 1;

    while (!frontier.empty()) {
        int node = frontier.top();
        frontier.pop();
        vector<int> neighbors = G.neighbors(node);
        for (int n : neighbors) {
            if (!visited[n] && nodes.find(n) != nodes.end()) {
                visited[n] = true;
                num_visited++;
                frontier.push(n);
            }
        }
    }
    return num_visited == nodes.size();
}

int check_partition(Graph<float> G, vector<int> assignment, int parts, cost_t lower, cost_t upper) {
    if (*assignment.begin() == -1) {
        printf("failed: no assignment\n");
        return -1;
    }

    rev_assignment_t rev_assign;
    for (int v = 0; v < assignment.size(); v++) {
        int part_num = assignment[v];
        rev_assign[part_num].insert(v);
    }

    // check number of parts lol

    for (auto it = rev_assign.begin(); it != rev_assign.end(); ++it) {
        int part_num = it->first;
        set<int> nodes = it->second;
        if (!check_connectivity(G, nodes)) {
            printf("failed*: part %d not connected\n", part_num);
            return 1;
        }
        cost_t z = 0;
        for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it) {
            z += G.weight(*node_it);
        }
        if (!(lower <= z && z <= upper)) {
            printf("failed*: part %d not in cost bound\n", part_num);
            return 1;
        }
    }
    printf("success\n");
    return 0;
}

int test_partition(Graph<cost_t> G, Tree<cost_t> T, int parts, cost_t lower, cost_t upper) {
    return check_partition(G, naive_partition(T, parts, lower, upper), parts, lower, upper);
}

int test_sequential_simple() {
    printf("-----\n");
    printf("simple graphs\n");

    Graph<float> G0 = get_G0();
    Tree<float> T0 = get_T0();
    printf("test 0.1 (PASS): ");
    test_partition(G0, T0, 3, 0.9, 3.1);

    Graph<float> G1 = get_G1();
    Tree<float> T1 = get_T1();
    printf("test 1.1 (PASS): ");
    test_partition(G1, T1, 3, 4.0, 6.0);
    printf("test 1.2 (PASS): ");
    test_partition(G1, T1, 2, 5.0, 10.0);
    printf("test 1.3 (FAIL): ");
    test_partition(G1, T1, 1, 4.0, 6.0);
    printf("test 1.4 (FAIL): ");
    test_partition(G1, T1, 4, 2.0, 3.0);
    printf("test 1.5 (FAIL): ");
    test_partition(G1, T1, 5, 2.0, 3.0);

    Graph<float> G2 = get_G2();
    Tree<float> T2 = get_T2();
    printf("test 2.1 (PASS): ");
    test_partition(G2, T2, 15, 1.0, 2.0);
    printf("test 2.2 (PASS): ");
    test_partition(G2, T2, 5, 3.0, 3.0);
    printf("test 2.3 (FAIL): ");
    test_partition(G2, T2, 4, 2.0, 3.0);

    Graph<float> G3 = get_G3();
    Tree<float> T3 = get_T3();
    printf("test 3.1 (PASS): ");
    test_partition(G3, T3, 3, 5.5, 8.5);
    printf("test 3.2 (FAIL): ");
    test_partition(G3, T3, 7, 0.5, 5.5);
    return 0;
}

int test_rand_st() {
    Graph<float> G0 = get_G0();
    Tree<float> T0 = rand_st(G0);
    printf("test 0.1 (PASS): ");
    test_partition(G0, T0, 3, 0.9, 3.1);

    Graph<float> G1 = get_G1();
    Tree<float> T1 = rand_st(G1);
    printf("test 1.1 (PASS): ");
    test_partition(G1, T1, 3, 4.0, 6.0);
    printf("test 1.2 (PASS): ");
    test_partition(G1, T1, 2, 5.0, 10.0);
    printf("test 1.3 (FAIL): ");
    test_partition(G1, T1, 1, 4.0, 6.0);
    printf("test 1.4 (FAIL): ");
    test_partition(G1, T1, 4, 2.0, 3.0);
    printf("test 1.5 (FAIL): ");
    test_partition(G1, T1, 5, 2.0, 3.0);

    Graph<float> G2 = get_G2();
    Tree<float> T2 = rand_st(G2);
    printf("test 2.1 (PASS): ");
    test_partition(G2, T2, 15, 1.0, 2.0);
    printf("test 2.2 (PASS): ");
    test_partition(G2, T2, 5, 3.0, 3.0);
    printf("test 2.3 (FAIL): ");
    test_partition(G2, T2, 4, 2.0, 3.0);
    return 0;
}

int main() {
//    printf("testing graph\n");
//    test_graph();
//    printf("testing spanning tree\n");
//    test_spanning_tree();
//    printf("testing random spanning tree\n");
//    test_rand_st();
    printf("testing sequential algo\n");
    test_sequential_simple();
    return 0;
};
