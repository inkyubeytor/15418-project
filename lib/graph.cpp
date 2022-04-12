#include <vector>
#include <cassert>
#include <stack>
#include <algorithm>

using std::stack;
using std::reverse;
using std::vector;

template <class T> class Graph {
private:
    vector <vector<int>> adj_list;
    vector<T> weights;
public:
    Graph(vector <vector<int>> adj_list, vector<T> weights)
            : adj_list(adj_list), weights(weights) {
        assert(adj_list.size() == weights.size());
    }

    int size() { return this->weights.size(); }

    float weight(int node) { return this->weights[node]; }

    vector<T> copy_weights() {
        vector<T> weights_copy(this->weights);
        return weights_copy;
    }

    const vector<int> &neighbors(int node) { return this->adj_list[node]; }
};

template <class T> class Tree : public Graph<T> {
private:
    int root;
public:
    Tree(vector <vector<int>> adj_list, vector<T> weights,
         int root_node) : Graph<T>(adj_list, weights),
                          root(root_node) {}

    vector<int> dfs_postorder_nodes() {
        vector<int> traversal;
        traversal.reserve(this->size());
        stack<int> stack;
        stack.push(this->root);
        while (!stack.empty()) {
            int curr = stack.top();
            stack.pop();
            traversal.push_back(curr);
            for (const int& child: this->neighbors(curr)) stack.push(child);
        }
        reverse(traversal.begin(), traversal.end());
        return traversal;
    }

    vector<vector<int>> dfs_postorder_levels() {
        vector<vector<int>> traversal {{this->root}};
        while (!traversal.back().empty()) {
            vector<int> next_vec;
            for (int v: traversal.back()) {
                for (int child: this->neighbors(v)) {
                    next_vec.push_back(child);
                }
            }
            traversal.push_back(next_vec);
        }
        reverse(traversal.begin(), traversal.end());
        return traversal;
    }
};
