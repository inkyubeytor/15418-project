# Parallel Cost-Bounded Graph Partitioning
Team Members: Abhishek Vijayakumar (abhishev@andrew.cmu.edu), Anna Cai (annacai@andrew.cmu.edu)

# Summary
We are going to implement a cost-bounded partitioning algorithm for weighted graphs and optimize it for parallelism across multicore CPUs. Given a graph with vertex weights, this algorithm creates a fixed number of graph partitions such that the sum of costs in each partition falls within a set interval. This algorithm can be used in the automatic division of precincts into voting districts, and is thus highly relevant to district map analysis.

# Background
The algorithm at its core is an adapation of a weighted tree partitioning algorithm by Ito et al. to general graphs through the initial computation of a spanning tree of the given graph (minimum or otherwise). The algorithm by Ito et al. to compute a partition of tree *T* with *p* connected components of weight bounded by (*l*, *u*) is as follows:

Define a LU partition of a tree to be one with at most *p* components where each component is connected and meets the weight bound, and a near-extendable partition to be one that meets all the requirements of a LU partition with the exception of the component containing the root, whose weight can be less than the lower bound. To compute the near-extendable partitions of a subtree at vertex *v*, we are given the set of near-extendable partitions for each child *v<sub>i</sub>* of *v*, where the children are arbitrarily numbered. Consider the tree with root *v* and including all subtrees at children numbered up to and including *i* and its "predecessor" (i need a better name for this), the same tree but up to child *i*-1. The near-extendable partitions of this tree can be computed by merging near-extendable partitions of the predecessor and near-extendable partitions of the subtree at *v<sub>i</sub>* such that the total number of components is at most *p*. These partitions can be cased on whether the component containing *v<sub>i</sub>* is merged with the component containing *v*. If yes, the merged component must meet the upper bound; otherwise, the component containing *v<sub>i</sub>* must meet both the lower and upper bounds.

is iset parallelizable?

"how might application benefit from parallelism"

redistricting applications (direct sampling as opposed to MCMC) (this part might not be necessary)

# The Challenge

## Workload
what are the dependencies, what are its memory access characteristics? (is there locality? is there a high communication to computation ratio?), is there divergent execution?
idk this requires thinking

dependencies: every level of the tree depends on the levels below it

can parallelize among nodes within a level? unsure about parallelism within vertex

## Constraints
What are the properties of the system that make mapping the workload to it challenging?

# Resources
https://link.springer.com/article/10.1007/s00453-010-9485-y
Partitioning a Weighted Tree into Subtrees with Weights in a Given Range
Ito et al.

We have previously implemented an inefficient, sequential version of this algorithm in Python at https://github.com/inkyubeytor/lupartition.

In order to test our parallel solution, we would require at a minimum access to the GHC machines. If possible, we would also like access to the PSC machines in order to test with higher thread counts.

# Goals and Deliverables

## 75% Goals
something parallel that has existent speedup tm??

## 100% Goals
have a parallel implementation that scales to at least 4/8 cores?

"justification as to why you can achieve it"

demo: speedup graphs? demonstrate on real map(s)?

## 125% Goals
scale to even more cores (like PSC level number of cores)

analysis of different spanning tree selections on performance (map quality) and runtime (focus on runtime)

make it into a package? lol

# Platform Choice

Our first choice of language would be CPython, so that we could work off of our sequential solution and focus on parallelization. If this is not an option for our project, we plan to work in C++, as we have covered several implementations of parallelism in C++. We would use either MPI or OpenMP to parallelize our solution, depending on which we feel is more suited to the portions of the code we feel that we can parallelize effectively.

We plan to deploy our solution on multicore CPUs (our computers, the GHC machines, and PSC).

can we please have python .-.
if no python, c/c++ so we can make a python library out of it
we want redistricting researchers to be able to run it with the resources they usually use (i.e. personal computers, colab notebooks, university provided resources)

# Schedule
we definitely need to implement it in sequential first (unless we're allowed to do it in python)
then parallelize the tree algo
then maybe spanning tree shenanigans (parallelize spanning tree computation)

