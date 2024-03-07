# M-component hypregraph and redundant augmentation

This project is a C++ implementation of some of the most important algorithms regarding the (k,l)-sparsity matroids. Given a graph, we can find a maximal 
(k,l)-sparse subgraph, the (k,l)-M-component hypergraph, or a minimal edge-set that makes a (k,l)-tight (hyper)graph (k,l)-redundant. All of these algorithms run in 
O(|V|^2+|E|) time, which is O(|V|^2) in case of simple graphs. 

### Theory

The theoretical background of these algorithms is presented in the papers [Sparse graphs and an augmentation problem](https://link.springer.com/article/10.1007/s10107-021-01689-0) 
and [Globally rigid augmentation of rigid graphs](https://web.cs.elte.hu/egres/tr/egres-21-04.pdf) Section 3. This implementation is desribed in details in the [PhD dissertation](https://mandras27.web.elte.hu/documents/doktori_updated.pdf) of András Mihálykó, Chapter 8.  

### Usage

The code is tested with C++14. 
Can be compiled with g++ by 

>g++ *.cpp -std=c++14 -O3 -o rigidAugmentations

The driver (main.cpp) accepts two parameters, k and l. Default value is (k,l) = (2,3). For example with the previous compile running

>./rigidAugmentations 1 1 

will work on the (1,1)-sparsity matroid, (i.e. graphical matroid, aka tree).

The program reads the graph in edge-list form from the standard input and writes to the standard input. 

### Contribution

The code is under construction, some features are still missing. Any remark on bugs, realization, style or any other contribution is highly appreciated. 
