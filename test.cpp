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

#include "spanning_tree.cpp"
#include "sequential.cpp"

typedef unordered_map<int, set<int>> rev_assignment_t;

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
    Graph<float> G1 = get_G1();
    Tree<float> BFSST = bfs_st(G1, 3);
    Tree<float> MST = min_st(G1, 3, simple_edge_weight);
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

int check_partition(Graph<float> G, vector<int> assignment, cost_t lower, cost_t upper) {
    rev_assignment_t rev_assign;
    for (int v = 0; v < assignment.size(); v++) {
        int part_num = assignment[v];
        rev_assign[part_num].insert(v);
    }

    for (auto it = rev_assign.begin(); it != rev_assign.end(); ++it) {
        int part_num = it->first;
        set<int> nodes = it->second;
        if (!check_connectivity(G, nodes)) {
            printf("failed: part %d not connected\n", part_num);
            return 1;
        }
        cost_t z = 0;
        for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it) {
            z += G.weight(*node_it);
        }
        if (!(lower <= z && z <= upper)) {
            printf("failed: part %d not in cost bound\n", part_num);
            return 1;
        }
    }
    printf("success\n");
    return 0;
}

int test_sequential() {
    Graph<float> G1 = get_G1();
    Tree<float> T1 = get_T1();
    vector<int> assignment = naive_partition(T1, 3, 0.9, 3.1);
    printf("test 1: ");
    check_partition(G1, assignment, 0.9, 3.1);
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
