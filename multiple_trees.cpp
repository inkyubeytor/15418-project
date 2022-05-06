#include <iostream>
#include <vector>
using std::vector;
#include <numeric>
#include <chrono>
#include "omp.h"

#ifndef GRAPH
#define GRAPH
#include "lib/graph.cpp"
#endif

#ifndef PARTITION
#define PARTITION
#include "spanning_tree.cpp"
//#include "sequential.cpp"
//#include "parallelism_within_levels.cpp"
#include "omp_within_child.cpp"
#endif

vector<int> naive_partition_graph(Graph<float> graph, int max_iter, int parts, cost_t lower, cost_t upper, std::string assignment_filename) {
    using namespace std::chrono;
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double> dsec;

    auto total_start = Clock::now();
    double total_time = 0;

    for (int iter = 0; iter < max_iter; iter++) {
        Tree<float> tree = rand_st(graph);
        std::string new_filename = assignment_filename.substr(0, assignment_filename.length() - 4) + "_iter" + std::to_string(iter) + ".txt";
        vector<int> assignment = naive_partition(tree, parts, lower, upper, new_filename);
        if (*assignment.begin() != -1) {
            total_time += duration_cast<dsec>(Clock::now() - total_start).count();
            printf("total time: %lf.\n", total_time);

            printf("success on iter %d\n", iter);
            return assignment;
        }
    }
    total_time += duration_cast<dsec>(Clock::now() - total_start).count();
    printf("total time: %lf.\n", total_time);

    return {-1};
}
