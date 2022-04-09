#include <iostream>
#include <vector>
using std::vector;

class Graph {
private:
    vector< vector<int> > adj_list;
    vector<float> weights;
public:
    Graph(vector < vector<int> > input_adj_list, vector<float> input_weights)
            : adj_list(input_adj_list), weights(input_weights) {}

    int size() { return weights.size(); }
    float weight(int node) { return weights[node]; }
    const vector<int>& neighbors(int node) { return adj_list[node]; }
};

//int main() {
//    vector<float> weights = {1.0, 2.0, 3.0};
//    vector<int> n0 = {};
//    vector<int> n1 = {2};
//    vector<int> n2 = {1};
//    vector<vector<int>> adj = {n0, n1, n2};
//    Graph G(adj, weights);
//    std::cout << G.neighbors(1)[0] << std::endl;
//    return 0;
//}