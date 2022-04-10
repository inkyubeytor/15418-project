#include <iostream>
#include <vector>
#include <cassert>
#include <stack>
#include <algorithm>

using std::stack;
using std::reverse;
using std::vector;

class Graph {
private:
    vector <vector<int>> adj_list;
    vector<float> weights;
public:
    Graph(vector <vector<int>> input_adj_list, vector<float> input_weights)
            : adj_list(input_adj_list), weights(input_weights) {
        assert(input_adj_list.size() == weights.size());
    }

    int size() { return weights.size(); }

    float weight(int node) { return weights[node]; }

    vector<float> copy_weights() {
        vector<float> weights_copy(weights);
        return weights_copy;
    }

    const vector<int> &neighbors(int node) { return adj_list[node]; }
};

class Tree : public Graph {
private:
    int root;
public:
    Tree(vector <vector<int>> input_adj_list, vector<float> input_weights,
         int root_node) : Graph(input_adj_list, input_weights),
                          root(root_node) {}

    vector<int> dfs_postorder_nodes() {
        vector<int> traversal;
        traversal.reserve(size());
        stack<int> stack;
        stack.push(root);
        while (!stack.empty()) {
            int curr = stack.top();
            stack.pop();
            traversal.push_back(curr);
            for (const int& child: neighbors(curr)) stack.push(child);
        }
        reverse(traversal.begin(), traversal.end());
        return traversal;
    }
};

int main() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {};
    vector<int> n2 = {3};
    vector<int> n3 = {};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Tree T(adj, weights, 0);
    vector<int> traversal = T.dfs_postorder_nodes();
    for (int x: traversal) std::cout << x << " ";
    std::cout << std::endl;
    return 0;
}
