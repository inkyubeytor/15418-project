//
// Created by ancai on 4/11/2022.
//

#include <iostream>
#include <fstream>
#include <string>
using std::string;
#include <sstream>

#ifndef GRAPH
#define GRAPH
#include "lib/graph.cpp"
#endif


typedef unordered_map<int, set<int>> rev_assignment_t;

Graph<float> tree_from_file(const char* filename) {
    vector<float> weights;
    vector<vector<int>> adj;

    std::ifstream file (filename);
    if (file.is_open()) {
        string size_string, weight_string, edge_string;

        std::getline(file, size_string);
        int size = std::stoi(size_string);

        weights.reserve(size);
        adj.reserve(size);
        for (int i = 0; i < size; i++) {
            std::getline(file, weight_string);
            weights.push_back(std::stof(weight_string));
            adj.push_back(vector<int>());
        }

        while (std::getline(file, edge_string)) {
            std::stringstream ss(edge_string);
            int a, b;
            ss >> a >> b;
            adj[a].push_back(b);
            adj[b].push_back(a);
        }

        file.close();

        Graph<float> G(adj, weights);
        return G;
    } else {
        printf("error opening file\n");
        Graph<float> G(adj, weights);
        return(G);
    }
}

Graph<float> get_G0() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {0, 2};
    vector<int> n2 = {0, 1, 3};
    vector<int> n3 = {2};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Graph<float> G(adj, weights);
    return G;
}

Tree<float> get_T0() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {};
    vector<int> n2 = {3};
    vector<int> n3 = {};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Tree<float> T(adj, weights, 0);
    return T;
}

Graph<float> get_G1() {
    vector<float> weights = {2.0, 3.0, 5.0, 2.0, 3.0};
    vector<int> n0 = {1};
    vector<int> n1 = {0, 2};
    vector<int> n2 = {1, 3};
    vector<int> n3 = {2, 4};
    vector<int> n4 = {3};
    vector <vector<int>> adj = {n0, n1, n2, n3, n4};
    Graph<float> G(adj, weights);
    return G;
}

Tree<float> get_T1() {
    vector<float> weights = {2.0, 3.0, 5.0, 2.0, 3.0};
    vector<int> n0 = {1};
    vector<int> n1 = {2};
    vector<int> n2 = {3};
    vector<int> n3 = {4};
    vector<int> n4 = {};
    vector <vector<int>> adj = {n0, n1, n2, n3, n4};
    Tree<float> T(adj, weights, 0);
    return T;
}

Graph<float> get_G2() {
    vector<float> weights(15, 1.0);
    vector<int> n0 = {1, 2};
    vector<int> n1 = {0, 3, 4};
    vector<int> n2 = {0, 5, 6};
    vector<int> n3 = {1, 7, 8};
    vector<int> n4 = {1, 9, 10};
    vector<int> n5 = {2, 11, 12};
    vector<int> n6 = {2, 13, 14};
    vector<int> n7 = {3};
    vector<int> n8 = {3};
    vector<int> n9 = {4};
    vector<int> n10 = {4};
    vector<int> n11 = {5};
    vector<int> n12 = {5};
    vector<int> n13 = {6};
    vector<int> n14 = {6};
    vector <vector<int>> adj = {n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14};
    Graph<float> G(adj, weights);
    return G;
}

Tree<float> get_T2() {
    vector<float> weights(15, 1.0);
    vector<int> n0 = {1, 2};
    vector<int> n1 = {3, 4};
    vector<int> n2 = {5, 6};
    vector<int> n3 = {7, 8};
    vector<int> n4 = {9, 10};
    vector<int> n5 = {11, 12};
    vector<int> n6 = {13, 14};
    vector<int> n7 = {};
    vector<int> n8 = {};
    vector<int> n9 = {};
    vector<int> n10 = {};
    vector<int> n11 = {};
    vector<int> n12 = {};
    vector<int> n13 = {};
    vector<int> n14 = {};
    vector <vector<int>> adj = {n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14};
    Tree<float> T(adj, weights, 0);
    return T;
}

Graph<float> get_G3() {
    vector<float> weights(7);
    for (int i = 0; i < 7; i++) {
        weights[i] = i;
    }
    vector<int> n0 = {1, 2};
    vector<int> n1 = {0, 3, 4};
    vector<int> n2 = {0, 5, 6};
    vector<int> n3 = {1};
    vector<int> n4 = {1};
    vector<int> n5 = {2};
    vector<int> n6 = {2};
    vector <vector<int>> adj = {n0, n1, n2, n3, n4, n5, n6};
    Graph<float> G(adj, weights);
    return G;
}

Tree<float> get_T3() {
    vector<float> weights(7);
    for (int i = 0; i < 7; i++) {
        weights[i] = i;
    }
    vector<int> n0 = {1, 2};
    vector<int> n1 = {3, 4};
    vector<int> n2 = {5, 6};
    vector<int> n3 = {};
    vector<int> n4 = {};
    vector<int> n5 = {};
    vector<int> n6 = {};
    vector <vector<int>> adj = {n0, n1, n2, n3, n4, n5, n6};
    Tree<float> T(adj, weights, 0);
    return T;
}

Graph<float> get_Gsmall1() {
    Graph<float> G = tree_from_file("test_cases/small1.txt");
    return G;
}

Graph<float> get_Gmed1() {
    Graph<float> G = tree_from_file("test_cases/med1.txt");
    return G;
}

Graph<float> get_Glarge1() {
    Graph<float> G = tree_from_file("test_cases/large1.txt");
    return G;
}

Graph<float> get_Glarge2() {
    Graph<float> G = tree_from_file("test_cases/large2.txt");
    return G;
}

Graph<float> get_Glarge3() {
    Graph<float> G = tree_from_file("test_cases/large3.txt");
    return G;
}

Graph<float> get_Glarge4() {
    Graph<float> G = tree_from_file("test_cases/large4.txt");
    return G;
}

Graph<float> get_Gladder500() {
    Graph<float> G = tree_from_file("test_cases/ladder500.txt");
    return G;
}

Graph<float> get_Gladder1000() {
    Graph<float> G = tree_from_file("test_cases/ladder1000.txt");
    return G;
}

Graph<float> get_Gconnectedsmall() {
    Graph<float> G = tree_from_file("test_cases/connected_small.txt");
    return G;
}

Graph<float> get_Gconnectedmed() {
    Graph<float> G = tree_from_file("test_cases/connected_med.txt");
    return G;
}
