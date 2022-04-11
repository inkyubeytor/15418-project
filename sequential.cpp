#include <vector>
using std::vector;
#include <queue>
using std::queue;
#include <utility>
using std::pair;
using std::make_pair;
#include <tuple>
using std::tuple;
using std::make_tuple;
#include <unordered_map>
using std::unordered_map;
#include <unordered_set>
using std::unordered_set;
#include <iostream>

#include "lib/graph.cpp"


typedef float cost_t;
typedef unordered_map<int, unordered_set<cost_t>> set1_t;
typedef unordered_set<int> set2_t;
typedef unordered_map<cost_t, pair<set1_t, set2_t>> zdict_t;
typedef unordered_map<int, zdict_t> kdict_t;
typedef tuple<int, cost_t, int, int, int> partition_info_t;

vector<int> naive_partition(Tree<cost_t> tree, int parts, cost_t lower, cost_t upper) {
    // first vector: vertex index v
    // second vector: child index i for that vertex
    // first map: parts index k ->
    // unordered_map: z -> pair<s1, s2>
    // s1 maps k' -> z' (the z', k' pairs earlier)
    // s2 is just k',   (the None, k' pairs earlier)
    vector<vector<pair<int, kdict_t>>> dp_table;

    // initialize empty dp table so we can refer to vs out of order
    int n = tree.size();
    for (int i = 0; i < n; i++) {
        vector<pair<int, kdict_t>> vec;
        dp_table.push_back(vec);
    }

    unordered_set<int> processed;
    vector<int> postorder = tree.dfs_postorder_nodes();
    int root = postorder.back();
    for (int& v: postorder) {
        // create children set
        const vector<int>& neighbors = tree.neighbors(v);
        vector<int> children;
        for (const int& neighbor: neighbors) {
            if (processed.count(neighbor)) children.push_back(neighbor);
        }

        // create parts_0 dict
        kdict_t parts_0 {{1, {{tree.weight(v), {{}, {0}}}}}};
        for (int k = 2; k <= parts; k++) {
            zdict_t empty;
            parts_0[k] = empty;
        };

        // add parts_0 to dp_table[v]
        dp_table[v].push_back(make_pair(-1, parts_0));

        for (int child : children) {
            kdict_t parts_i;
            for (int k = 1; k < parts + 1; k++) {
                zdict_t z_dict;
                // S1
                for (int k_prime = 1; k_prime < k + 1; k_prime++) {
                    zdict_t left = dp_table[v].back().second[k_prime];
                    zdict_t right = dp_table[child].back().second[k - k_prime + 1];
                    for (auto l_it = left.begin(); l_it != left.end(); ++l_it) {
                        cost_t a = l_it->first;
                        for (auto r_it = right.begin(); r_it != right.end(); ++r_it) {
                            cost_t b = r_it->first;
                            if (!z_dict.count(a + b)) {
                                set1_t s1;
                                set2_t s2;
                                z_dict[a + b] = make_pair(s1, s2);
                            }
                            if (!z_dict[a + b].first.count(k_prime)) {
                                unordered_set<cost_t> inner_set;
                                z_dict[a + b].first[k_prime] = inner_set;
                            }
                            z_dict[a + b].first[k_prime].insert(a);
                        }
                    }
                }

                // S2
                for (int k_prime = 1; k_prime < k; k_prime++) {
                    zdict_t left = dp_table[v].back().second[k_prime];
                    zdict_t right = dp_table[child].back().second[k - k_prime];
                    for (auto r_it = right.begin(); r_it != right.end(); ++r_it) {
                        cost_t b = r_it->first;
                        if (lower <= b && b <= upper) {
                            for (auto l_it = left.begin(); l_it != left.end(); ++l_it) {
                                cost_t a = l_it->first;
                                if (!z_dict.count(a)) {
                                    set1_t s1;
                                    set2_t s2;
                                    z_dict[a] = make_pair(s1, s2);
                                }
                                z_dict[a].second.insert(k_prime);
                            }
                        }
                    }
                }
                // add z_dict to parts_i
                parts_i[k] = z_dict;
            }
            // add parts_i to dp_table[v][child] (correct for off by one idx?)
            dp_table[v].push_back(make_pair(child, parts_i));
        }
        processed.insert(v);
    }

    vector<int> assignment(tree.size(), -1);
    bool exists = false;
    cost_t final_z;
    zdict_t final_zd = dp_table[root].back().second[parts];
    for (auto it = final_zd.begin(); it != final_zd.end(); ++it) {
        cost_t z = it->first;
        if (lower <= z && z <= upper) {
            exists = true;
            final_z = z;
            break;
        }
    }
    if (!exists) {
        return assignment;  // figure out what we want this to be
    }

    assignment[root] = 0;
    int max_part_num = 0;

    queue<partition_info_t> input_queue;
    input_queue.push(make_tuple(root, -1, parts, dp_table[root].size() - 1, 0));


    while (!input_queue.empty()) {
        int v, k, i, v_part_num;
        cost_t z;
        std::tie(v, z, k, i, v_part_num) = input_queue.front();
        input_queue.pop();

        if (i == 0) {
            continue;
        }

        if (z == -1) {  // start new partition
            zdict_t zd = dp_table[v][i].second[k];
            for (auto it = zd.begin(); it != zd.end(); ++it) {
                cost_t z1 = it->first;
                if (lower <= z1 && z1 <= upper) {
                    z = z1;
                    break;
                }
            }
            if (z == -1) {  // comment out after testing
                printf("error: z not found\n");
                break;
            }
        }

        int vp = dp_table[v][i].first;
        set1_t s1 = dp_table[v][i].second[k][z].first;
        set2_t s2 = dp_table[v][i].second[k][z].second;

        if (!s2.empty()) {
            int kp = *s2.begin();
            int new_part_num = ++max_part_num;
            assignment[vp] = new_part_num;
            input_queue.push(make_tuple(v, z, kp, i - 1, v_part_num));
            input_queue.push(make_tuple(vp, -1, k - kp, dp_table[vp].size() - 1, new_part_num));
        } else {
            int kp = s1.begin()->first;
            cost_t zp = *s1.begin()->second.begin();
            assignment[vp] = v_part_num;
            input_queue.push(make_tuple(v, zp, kp, i - 1, v_part_num));
            input_queue.push(make_tuple(vp, z - zp, k - kp + 1, dp_table[vp].size() - 1, v_part_num));
        }
    }

    return assignment;
}


// def naive_partition(tree, key, parts, lower, upper):
//    dp_tree = copy_partition(tree, key)
//
//    # dp_tree                          - whole tree
//    # dp_tree.nodes[v]                 - vertex v
//    # dp_tree.nodes[v]["table"][i] - vertex v, child i is a dict with values
//    #   "vertex": v_i,
//    #   "parts": k -> Dict { z -> Set{ (z', k') for s1 | (None, k') for s2 } } }
//
//    # Traverse the graph bottom up, defining children as
//    # already-processed neighbors
//    processed = set()
//    root = None
//    for v in nx.dfs_postorder_nodes(dp_tree):
//        children = set(dp_tree.neighbors(v)) & processed
//        parts_0 = {k: {} for k in range(2, parts + 1)}
//        parts_0[1] = {dp_tree.nodes[v]["weight"]: {(None, 0)}}
//        dp_tree.nodes[v]["table"] = [{"vertex": None, "parts": parts_0}]
//        for child in children:
//            parts_dict = {}
//            for k in range(1, parts + 1):
//                z_dict = {}
//                # S1 stuff
//                for k_prime in range(1, k + 1):
//                    left = dp_tree.nodes[v]["table"][-1]["parts"][k_prime]
//                    right = dp_tree.nodes[child]["table"][-1]["parts"][
//                        k - k_prime + 1]
//                    for a in left:
//                        for b in right:
//                            try:
//                                z_dict[a + b].add((a, k_prime))
//                            except KeyError:
//                                z_dict[a + b] = {(a, k_prime)}
//                # S2 stuff
//                for k_prime in range(1, k):
//                    left = dp_tree.nodes[v]["table"][-1]["parts"][k_prime]
//                    right = dp_tree.nodes[child]["table"][-1]["parts"][
//                        k - k_prime]
//                    for b in right:
//                        if lower <= b <= upper:
//                            for a in left:
//                                try:
//                                    z_dict[a].add((None, k_prime))
//                                except KeyError:
//                                    z_dict[a] = {(None, k_prime)}
//                parts_dict[k] = z_dict
//            dp_tree.nodes[v]["table"].append(
//                {"vertex": child, "parts": parts_dict})
//        processed.add(v)
//        root = v
//

//    # backtracking
//    try:
//        next(filter(lambda y: lower <= y <= upper,
//                    dp_tree.nodes[root]["table"][-1]["parts"][parts]))
//    except StopIteration:
//        return None
//
//    assignment = {root: 0}
//    input_queue = [(root, None, parts, len(dp_tree.nodes[v]["table"]) - 1, 0)]
//
//    def process(v, z, k, i, v_part_num):
//        if i == 0:
//            return
//
//        if z is None:  # make a new partition
//            z = next(filter(lambda y: lower <= y <= upper,
//                            dp_tree.nodes[v]["table"][i]["parts"][k]))
//
//        vp = dp_tree.nodes[v]["table"][i]["vertex"]
//        zp, kp = dp_tree.nodes[v]["table"][i]["parts"][k][z].pop()
//        if zp is None:
//            new_part_num = 1 + max(assignment.values(), default=0)
//            assignment[vp] = new_part_num
//            input_queue.append((v, z, kp, i - 1, v_part_num))
//            input_queue.append((vp, None, k - kp,
//                                len(dp_tree.nodes[vp]["table"]) - 1,
//                                new_part_num))
//        else:
//            assignment[vp] = v_part_num
//            input_queue.append((v, zp, kp, i - 1, v_part_num))
//            input_queue.append((vp, z - zp, k - kp + 1,
//                                len(dp_tree.nodes[vp]["table"]) - 1,
//                                v_part_num))
//
//    while input_queue:
//        process(*input_queue.pop())
//
//    print(assignment)
//    return assignment

int main() {
    vector<float> weights = {0.0, 1.0, 2.0, 3.0};
    vector<int> n0 = {1, 2};
    vector<int> n1 = {};
    vector<int> n2 = {3};
    vector<int> n3 = {};
    vector <vector<int>> adj = {n0, n1, n2, n3};
    Tree<float> tree(adj, weights, 0);
    vector<int> partition = naive_partition(tree, 3, 0.9, 3.1);
    for (int p: partition)
        std::cout << p << std::endl;
    return 0;
}
