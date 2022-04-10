//
// Created by ancai on 4/9/2022.
//

#include <random>
#include <queue>
#include <vector>
#include "lib/graph.cpp"
// #include "spanning_tree.h"

using std::queue;
using std::vector;

Tree bfs_st(Graph input_graph, int root) {
    vector<vector<int>> new_adj_list;
    new_adj_list.reserve(input_graph.size());
    for (int i = 0; i < input_graph.size(); i++) {
        vector<int> new_neighbors;
        new_adj_list.push_back(new_neighbors);
    }

    bool visited[input_graph.size()];
    queue<int> frontier;
    frontier.push(root);
    visited[root] = true;

    while (!frontier.empty()) {
        int node = frontier.front();
        frontier.pop();
        vector<int> neighbors = input_graph.neighbors(node);
        new_adj_list[node].reserve(neighbors.size());
        for (int n : neighbors) {
            if (!visited[n]) {
                visited[n] = true;
                new_adj_list[node].push_back(n);
                frontier.push(n);
                printf("succesfully added edge %d to %d\n", node, n);
            }
        }
    }

    Tree output_graph(new_adj_list, input_graph.copy_weights(), root);
    return output_graph;
}

// Tree random_st()

// Tree minimum_st()

int main() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {0, 2};
    vector<int> n2 = {0, 1, 3};
    vector<int> n3 = {2};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Graph G(adj, weights);
    Tree T(adj, weights, 0);
    Tree BFSST = bfs_st(G, 3);
//    Tree MST =
//    vector<int> traversal = T.dfs_postorder_nodes();
//    for (int x: traversal) std::cout << x << " ";
//    std::cout << std::endl;
    return 0;
}
