# Parallel Cost-Bounded Graph Partitioning
Team Members: Abhishek Vijayakumar (abhishev@andrew.cmu.edu), Anna Cai (annacai@andrew.cmu.edu)

# Summary
We are going to implement a cost-bounded partitioning algorithm for weighted graphs and optimize it for parallelism across multicore CPUs. Given a graph with vertex weights, this algorithm creates a fixed number of graph partitions such that the sum of costs in each partition falls within a set interval. This algorithm can be used in the automatic division of precincts into voting districts, and is thus highly relevant to district map analysis.

# Background
The algorithm at its core is an adapation of a weighted tree partitioning algorithm by Ito et al. (linked below) to general graphs through the initial computation of a spanning tree of the given graph (minimum or otherwise). The algorithm by Ito et al. to compute a partition of tree *T* with *p* connected components of weight bounded by (*l*, *u*) is as follows (pseudocode in section below):

Define a LU partition of a tree to be one with at most *p* components where each component is connected and meets the weight bound, and a near-extendable partition to be one that meets all the requirements of a LU partition with the exception of the component containing the root, whose weight can be less than the lower bound. To compute the near-extendable partitions of a subtree at vertex *v*, we are given the set of near-extendable partitions for each child *v<sub>i</sub>* of *v*, where the children are arbitrarily numbered. Consider the tree with root *v* and including all subtrees at children numbered up to and including *i* and its "predecessor tree", the same tree but up to child *i*-1. The near-extendable partitions of this tree can be computed by merging near-extendable partitions of the predecessor tree and near-extendable partitions of the subtree at *v<sub>i</sub>* such that the total number of components is at most *p*. These partitions can be cased on whether the component containing *v<sub>i</sub>* is merged with the component containing *v*. If yes, the merged component must meet the upper bound; otherwise, the component containing *v<sub>i</sub>* must meet both the lower and upper bounds.

Two possible axes of parallelism in this algorithm are within levels of the tree (since the only dependencies between parent and child) and across trying combinations of partitions (the loops through `k_prime` in the pseudocode). The speedup due to the former would be highly dependent on the depth and branching factor of the tree, while the latter is likely to be greatly limited by high contention to shared data structures.

Additionally, not all spanning trees of a graph with a valid LU partition are guaranteed to have valid LU partitions, so a more involved algorithm to generate spanning trees may be required to optimize the chances of finding a valid partition, or multiple spanning trees may need to be generated and tested. The later has high potential to be parallelized, as the partition computation for each spanning tree is separate.

## Pseudocode

```
/* G: graph
 * p: number of partitions
 * l, u: lower, upper bound
 * num_iters: number of iterations */

for iter in num_iters:
    T = spanning_tree(G)  # any algorithm that returns a spanning tree (should return a different spanning tree in each iteration)
    for v in postorder_traverse(T):
        parts_0 = {k: {} for k in range(2, p + 1)} + {1: {v.weight: {(None, 0)}}}  # base case
        v.table = [parts_0]  # list of dictionaries where keys correspond to number of components and values are sets of tuples describing partitions (weight of root component, number of components)
        for vi in v.children:
            parts_dict = {}
            for k in range(1, p + 1):  # loop through potential number of components
                z_dict = {}
                # case: merge root components
                for k_prime in range(1, k + 1):  # loop through ways to divide number of components among two partitions
                    left = v.table[-1][k_prime]  # near-extendable partitions of v and children 1 to i-1
                    right = vi.table[-1][k - k_prime + 1]  # near-extendable partitions of child i
                    for a in left:
                        for b in right:
                            z_dict[a + b].add((a, k_prime))  # merge partitions, merge root components
                # case: do not merge root components
                for k_prime in range(1, k):
                    left = v.table[-1][k_prime]  # near-extendable partitions of v and children 1 to i-1
                    right = vi.table[-1][k - k_prime]  # near-extendable partitions of child i
                    for b in right:
                        if l <= b <= u:  # only select valid LU partitions
                            for a in left:
                                z_dict[a].add((None, k_prime))  # merge partitions, new root component is left root component
                parts_dict[k] = z_dict
            v.table.append(parts_dict)

    # find valid LU partition of root
    for a in T.root.table[-1]["parts"][p]:
        if l <= a <= u:
            return generate_assignment(a, T)

return None  # no valid partition found
```

# The Challenge

## Workload
The problem has very high divergent execution if the work division is done by dividing the graph to be partitioned. Each execution context would then be concerned with entirely different portions of the graph. If we divide work along the innermost loop, we may be able to leverage convergent execution at the cost of much higher communication (frequent parallel reductions).

The memory accesses of the program have potentially high locality. Because the algorithm builds up the partitioning iteratively by adding nodes, it will always add nodes on the frontier between the set of included nodes and the rest of the graph. If the graph is ordered such that adjacent nodes in the graph are located together in memory, adjacent memory accesses may occur very close to each other.

The amount of communication (both the scale and frequency) depend on the division of work. If work is divided along the innermost axis of parallelism (the computation for how to add the next node to a partition), there may be very high-frequency, fine-grained communication. If we instead partition work by dividing the graph to be partitioned, the communication will consist of aggregating the final sub-partitions, a much larger message size at a much smaller frequency.

## Constraints
One constraint we have is in the system's representation of the graph. If we use a standard graph representation such as an adjacency list, the tree we compute may have many nodes per cache line in memory. This could cause issues via false sharing unless we artificially inflate the size of our graph representation.

Another constraint is in the size of the working set. For sufficiently high differences between lower and upper cost bounds in the standard, "naive" algorithm, we may end up maintaining sets that grow beyond the size of at least L1 and L2 cache levels on a given core. Regardless of the strategy for computation that we use, this could cause L2 capacity misses that reduce the speed of the algorithm.

A third constraint relates to the potential for divergent execution. Even though a multicore CPU is much better at performing divergent operations in parallel, the pieces of work may become differently sized enough that we require a work queue implementation. Such a queue would likely have to store partial graph partitions in order to provide the necessary contexts for pending pieces of work in the queue. In this case, it is possible that the communication of partial partitions via a work queue could be prohibitively expensive in the size of the communication involved, forcing us to use larger granularities of work with potentially uneven work distributions.

# Resources
The paper from which the original algorithms come from is https://link.springer.com/article/10.1007/s00453-010-9485-y
(Partitioning a Weighted Tree into Subtrees with Weights in a Given Range, Ito et al.).

We have previously implemented the "naive," sequential version of this algorithm in Python at https://github.com/inkyubeytor/lupartition, and we will likely use this as our starting point.

In order to test our parallel solution, we would require at a minimum access to the GHC machines. If possible, we would also like access to the PSC machines in order to test with higher thread counts.

# Goals and Deliverables

## 75% Goals
 * Achieve increasing speedups with up to 8 cores using parallelism within the computation for a given tree. We believe this is possible because for sufficiently different lower and upper cost bounds, there are many computations involving mapping functions over sequences with far more than 8 elements.
 * Implement the "trivial" parallel solution of having each core compute the problem for a different spanning tree of the same graph.
 * Speedup tables and plots for all parallel solutions tried.

## 100% Goals
 * Develop a visual demonstration for how graphs are being partitioned by the algorithm (may be more applicable to some versions of parallelism than others).
 * Combine orthogonal parallelism strategies to increase speedup beyond each method's speedup alone.

## 125% Goals
 * Achieve increasing speedups on up to 64 cores (using the PSC machines).
 * Comparative analysis of different parallel spanning tree algorithms and how choices of spanning tree can lead to fewer partitioning attempts.

# Platform Choice
Our first choice of language would be CPython, so that we could work off of our sequential solution and focus on parallelization. In this case, we would use the multiprocessing library. If this is not an option for our project, we plan to work in C++, as we have covered several implementations of parallelism in C++. We would use either MPI or OpenMP to parallelize our solution, depending on which we feel is more suited to the portions of the code we feel that we can parallelize effectively.

We plan to deploy our solution on multicore CPUs (our computers, the GHC machines, and PSC).


# Schedule

Week of 2022-03-27: Create efficient sequential implementation. If in C++, prioritize the naive algorithm. If in Python, consider implementing the faster "interval set" algorithm as well.

Week of 2022-04-03: Implement parallelism within tree partition computation (across nodes in the same level in the postorder traversal of a tree). If viable, implement parallelism over the `k_prime` innermost loops (likely too high contention for data structures to achieve performance improvements, but worth testing).

Week of 2022-04-10 (Checkpoint): Experiment with different methods of work division for parallelism within the partition computation.

Week of 2022-04-17: Implement parallel minimum spanning tree computations and experiment with choosing spanning trees that make valid partitions more likely to exist.

Week of 2022-04-24: Implement parallelism by attempting to compute the problem for sub-partitions of the graph and combining them.


# MILESTONE

The first piece of work we did was create an efficient sequential implementation of our algorithm in C++.
We wrote our own graph (and tree) representations and designed a way to read them from files.
We also wrote graph traversals and sequential spanning tree algorithms.
We then implemented the efficient naive partition algorithm.
We chose not to implement the interval set algorithm because the naive algorithm is more generalizable to other tasks involving partitions.

The second piece of work we did was implement and test two forms of parallelism with OpenMP.
The first form of parallelism involves parallelizing an existing loop over part counts k.
The second form of parallelism involves changing our DFS postorder traversal of our tree into a "levels" based traversal, where each node's parent is the in preceding level.
We then parallelized our computation over each level of vertices, because these sets can be dealt with independently.
Both forms of parallelism involve critical sections (updates to shared data structures) that must be handled separately.

## Deliverables and Progress

We are currently on track for our schedule. We have finished both deliverables from the previous two weeks and we have begun to implement code for spanning tree algorithms that will be parallelized in week 4.
We believe we will be able to achieve all of our 100% goals at this time, though not necessarily our 125% goals.
We may be able to achieve some 125% goals (e.g. spanning tree) depending on how much time we put into our 100% visualization system.
Our current list of goals for the poster session is:

## Poster Session Goals
* Achieve increasing speedups with up to 8 cores using parallelism within the computation for a given tree.
* Implement the "trivial" parallel solution of having each core compute the problem for a different spanning tree of the same graph.
* Comparative analysis of different parallel spanning tree algorithms and how choices of spanning tree can lead to fewer partitioning attempts.
* Combine orthogonal parallelism strategies to increase speedup (e.g. multiple trees and parallelism across levels).
* Speedup tables and plots for all parallel solutions tried.
* Develop a visual demonstration for how graphs are being partitioned by the algorithm.

At the poster session, we plan to show speedup plots for all parallel solutions tried, focusing on those that produced the best results.
We also plan to show some visualization (at the very least a series of static visualizations) of a graph being partitioned, attempting to indicate where parallelism exists in the partitioning process.

## Preliminary Results - TODO

## Concerns

Our greatest concern is our lack of knowledge with C++ visualization technologies.
We plan to do some research into the different options available, but we may also simply generate a trace for our algorithm and then visualize this with Python.

We are also somewhat concerned that our tests may not be comprehensive enough to describe the problem space.
We are currently resolving this by generating many types of random graphs.
If we discover that some forms of parallelism are much better for certain types of graphs than others, we may hand engineer some test cases to demonstrate this.

## Updated Schedule

Week of 2022-04-10 1st half:
 * Anna: Implement parallelism over multiple possible spanning trees for an arbitrary graph partitioning algorithm.
 * Abhi: Experiment with different types of OMP work division in the levels-based and within-child parallelism.

Week of 2022-04-10 2nd half:
 * Anna: Look into lock-free concurrent hash maps to allow finer-grained parallelism.
 * Abhi: Experiment with OpenMP nested parallelism for computing sets S1 and S2.

Week of 2022-04-17 1st half:
 * Abhi: Experiment with reordering graph based on traversal to improve locality.
 * Anna: Experiment with priority-queueing with multiple spanning trees when finding a good spanning tree is hard: create more trees than the number of execution contexts, then have threads switch to different pieces of work if their piece is looking bad.

Week of 2022-04-17 2nd half:
 * Abhi: Experiment with MPI as an alternative to OpenMP when parallelizing over multiple spanning trees.
 * Anna: Experiment with parallelizing backtracking via a work queue system.
Week of 2022-04-24 1st half:
 * Anna: Implement parallel minimum spanning tree algorithms and test the performance of different spanning tree methods.
 * Abhi: Work on instrumenting code for visualization.

Week of 2022-04-24 2nd half:
 * Abhi: Work on producing visualizations.
