//
// Created by ancai on 4/9/2022.
//

#include <random>
#include <queue>
#include <vector>
#include <utility>
#include <functional>
#include <algorithm>
#include "lib/graph.cpp"
// #include "spanning_tree.h"

using std::queue;
using std::vector;
using std::priority_queue;

typedef std::pair<float, std::pair<int, int>> edge_t;

Tree<float> bfs_st(Graph<float> input_graph, int root) {
    vector<vector<int>> new_adj_list;
    new_adj_list.reserve(input_graph.size());
    for (int i = 0; i < input_graph.size(); i++) {
        vector<int> new_neighbors;
        new_adj_list.push_back(new_neighbors);
    }

    bool visited[input_graph.size()];
    std::fill_n(visited, input_graph.size(), false);
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
                printf("added edge %d to %d\n", node, n);
            }
        }
    }
    Tree<float> output_graph(new_adj_list, input_graph.copy_weights(), root);
    return output_graph;
}

//bool edge_compare(const edge_t& a, const edge_t& b) {
//    return a.first > b.first;
//}

float simple_edge_weight(int a, int b) {
    return static_cast<float>(a + b);
}

// Prim's algorithm
Tree<float> min_st(Graph<float> input_graph, int root, float (*edge_weight)(int, int)) {
    vector<vector<int>> new_adj_list;
    new_adj_list.reserve(input_graph.size());
    for (int i = 0; i < input_graph.size(); i++) {
        vector<int> new_neighbors;
        new_adj_list.push_back(new_neighbors);
    }

    bool visited[input_graph.size()];
    std::fill_n(visited, input_graph.size(), false);
    visited[root] = true;
    int num_visited = 1;
    auto cmp = [](edge_t a, edge_t b) { return a.first > b.first; };
    priority_queue<edge_t, vector<edge_t>, decltype(cmp)> edges(cmp);
    for (int n : input_graph.neighbors(root)) {
        auto p = std::make_pair(edge_weight(n, root), std::make_pair(root, n));
        // n < add_vertex ? std::make_pair(n, add_vertex) : std::make_pair(add_vertex, n)
        edges.push(p);
    }

    while (num_visited < input_graph.size()) {
        edge_t min_edge = edges.top();
        edges.pop();
        int inset = min_edge.second.first;
        int notinset = min_edge.second.second;
        if (visited[inset] && visited[notinset]) {
//            printf("vertices %d, %d already covered\n", inset, notinset);
            continue;
        } else if (visited[notinset]) {
//            printf("swap\n");
            std::swap(inset, notinset);
        }
        visited[notinset] = true;
        new_adj_list[inset].push_back(notinset);
        printf("added edge %d to %d\n", inset, notinset);
        num_visited++;
        for (int n : input_graph.neighbors(notinset)) {
            if (!visited[n]) {
                auto p = std::make_pair(edge_weight(n, notinset), std::make_pair(notinset, n));
                // n < add_vertex ? std::make_pair(n, add_vertex) : std::make_pair(add_vertex, n)
                edges.push(p);
            }
        }
    }
    Tree<float> output_graph(new_adj_list, input_graph.copy_weights(), root);
    return output_graph;
}

int main() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {0, 2};
    vector<int> n2 = {0, 1, 3};
    vector<int> n3 = {2};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Graph<float> G(adj, weights);
    Tree<float> T(adj, weights, 0);
    Tree<float> BFSST = bfs_st(G, 3);
    Tree<float> MST = min_st(G, 3, simple_edge_weight);
    return 0;
}
