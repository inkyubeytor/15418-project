# Parallel Cost-Bounded Graph Partitioning
Team Members: Abhishek Vijayakumar (abhishev@andrew.cmu.edu), Anna Cai (annacai@andrew.cmu.edu)

# Summary
We are going to implement a cost-bounded partitioning algorithm for weighted graphs and optimize it for parallelism across multicore CPUs. Given a graph with vertex weights, this algorithm creates a fixed number of graph partitions such that the sum of costs in each partition falls within a set interval. This algorithm can be used in the automatic division of precincts into voting districts, and is thus highly relevant to district map analysis.

# Background
The algorithm at its core is an adapation of a weighted tree partitioning algorithm by Ito et al. to general graphs through the initial computation of a spanning tree of the given graph (minimum or otherwise). The algorithm by Ito et al. to compute a partition of tree *T* with *p* connected components of weight bounded by (*l*, *u*) is as follows:

Define a LU partition of a tree to be one with at most *p* components where each component is connected and meets the weight bound, and a near-extendable partition to be one that meets all the requirements of a LU partition with the exception of the component containing the root, whose weight can be less than the lower bound. To compute the near-extendable partitions of a subtree at vertex *v*, we are given the set of near-extendable partitions for each child *v<sub>i</sub>* of *v*, where the children are arbitrarily numbered. Consider the tree with root *v* and including all subtrees at children numbered up to and including *i* and its "predecessor" (i need a better name for this), the same tree but up to child *i*-1. The near-extendable partitions of this tree can be computed by merging near-extendable partitions of the predecessor and near-extendable partitions of the subtree at *v<sub>i</sub>* such that the total number of components is at most *p*. These partitions can be cased on whether the component containing *v<sub>i</sub>* is merged with the component containing *v*. If yes, the merged component must meet the upper bound; otherwise, the component containing *v<sub>i</sub>* must meet both the lower and upper bounds.

## Pseudocode - TODO

# The Challenge

## Workload

The problem has very high divergent execution if the work division is done by dividing the graph to be partitioned. Each execution context would then be concerned with entirely different portions of the graph. If we divide work along the innermost loop, we may be able to leverage convergent execution at the cost of much higher communication (frequent parallel reductions).

The memory accesses of the program have potentially high locality. Because the algorithm builds up the partitioning iteratively by adding nodes, it will always add nodes on the frontier between the set of included nodes and the rest of the graph. If the graph is ordered such that adjacent nodes in the graph are located together in memory, adjacent memory accesses may occur very close to each other.

The amount of communication (both the scale and frequency) depend on the division of work. If work is divided along the innermost axis of parallelism (the computation for how to add the next node to a partition), there may be very high-frequency, fine-grained communication. If we instead partition work by dividing the graph to be partitioned, the communication will consist of aggregating the final sub-partitions, a much larger message size at a much smaller frequency.

## Constraints
What are the properties of the system that make mapping the workload to it challenging?

One constraint we have is in the system's representation of the graph. If we use a standard graph representation such as an adjacency list, the tree we compute may have many nodes per cache line in memory. This could cause issues via false sharing unless we artificially inflate the size of our graph representation.

Another constraint is in the size of the working set. For sufficiently high differences between lower and upper cost bounds in the standard, "naive" algorithm, we may end up maintaining sets that grow beyond the size of at least L1 and L2 cache levels on a given core. Regardless of the strategy for computation that we use, this could cause L2 capacity misses that reduce the speed of the algorithm.

A third constraint relates to the potential for divergent execution. Even though a multicore CPU is much better at performing divergent operations in parallel, the pieces of work may become differently sized enough that we require a work queue implementation. Such a queue would likely have to store partial graph partitions in order to provide the necessary contexts for pending pieces of work in the queue. In this case, it is possible that the communication of partial partitions via a work queue could be prohibitively expensive in the size of the communication involved, forcing us to use larger granularities of work with potentially uneven work distributions.

# Resources
The paper from which the original algorithms come from is https://link.springer.com/article/10.1007/s00453-010-9485-y
Partitioning a Weighted Tree into Subtrees with Weights in a Given Range
Ito et al.

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


# Schedule - TODO: more details on weeks 2 and 3

Week of 2022-03-27: Create efficient sequential implementation. If in C++, prioritize the naive algorithm. If in Python, consider implementing the faster "interval set" algorithm as well.

Week of 2022-04-03: Implement parallelism in the computation for a single node.

Week of 2022-04-10 (Checkpoint): Experiment with different methods of work division for parallelism within the partition computation.

Week of 2022-04-17: Implement parallel minimum spanning tree computations and experiment with choosing spanning trees that make valid partitions more likely to exist.

Week of 2022-04-24: Implement parallelism by attempting to compute the problem for sub-partitions of the graph and combining them.

