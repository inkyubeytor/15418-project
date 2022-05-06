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

    Tree<T> dfs_reorder() {
        int n = this->size();

        // if you index new to old with a new index it gives you the old index
        vector<int> new_to_old = this->dfs_postorder_nodes();
        vector<int> old_to_new(n, -1);
        for (int i = 0; i < n; i++)
            old_to_new[new_to_old[i]] = i;

        vector<T> new_weights;
        for (int i = 0; i < n; i++)
            new_weights.push_back(this->weight(new_to_old[i]));

        vector<vector<int>> new_adj_list;
        for (int i = 0; i < n; i++) {
            vector<int> v;
            for (int neighbor: this->neighbors(new_to_old[i]))
                v.push_back(old_to_new[neighbor]);
            new_adj_list.push_back(v);
        }

        return Tree(new_adj_list, new_weights, old_to_new[root]);
    }
};
