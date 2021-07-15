#include "graph.h"


graph::graph(const std::vector<edge>& edgeList)
{   
        edges = edgeList;
        std::vector<vertex> endVertices;
        for (const edge& e: edgeList)
        {
            endVertices=e.getEndVertices();
            //we know that it has 2 vertices u and v. We we use this now, later by hypergraph it might be different
            vertex u = endVertices[0];
            vertex v = endVertices[1];
            vertexNeighbors[v].push_back(u);
            vertexNeighbors[u].push_back(v);
        }
        size = vertexNeighbors.size();
}


void graph::print()
{
    std::cout<<"Size of graph: "<<numberOfNodes()<<std::endl;
    std::cout<<"Edges:"<<std::endl;
    for (const edge& e: edges)
    {
        e.print();
    }
}


int main()
{
    std::vector<vertex> V;
    for (int i=0; i<10; i++)
    {
        V.emplace_back(i);
    }
    std::vector<edge> EL;
    for (int i=0; i<9; i++)
    {
        EL.emplace_back(V[i],V[i+1],false);
    }
    graph G(EL);
    G.print();
    return 0;
}