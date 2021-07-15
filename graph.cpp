#include "graph.h"


Graph::Graph(const std::vector<Edge>& edgeList)
{   
        edges = edgeList;
        std::vector<Vertex> endVertices;
        for (const Edge& e: edgeList)
        {
            endVertices=e.getEndVertices();
            //we know that it has 2 vertices u and v. We we use this now, later by hypergraph it might be different
            Vertex u = endVertices[0];
            Vertex v = endVertices[1];
            vertexNeighbors[v].push_back(u);
            vertexNeighbors[u].push_back(v);
        }
        size = vertexNeighbors.size();
}


void Graph::print()
{
    std::cout<<"Size of graph: "<<numberOfNodes()<<std::endl;
    std::cout<<"Edges:"<<std::endl;
    for (const Edge& e: edges)
    {
        e.print();
    }
}


DirectedGraph::DirectedGraph(const std::vector<DirectedEdge>& edgeList)
{   
        edges = edgeList;
        std::vector<Vertex> endVertices;
        for (const DirectedEdge& e: edgeList)
        {
            endVertices=e.getEndVertices();
            //we know that it has 2 vertices u and v. We we use this now, later by hypergraph it might be different
            Vertex u = endVertices[0];
            Vertex v = endVertices[1];
            vertexOutwards[u].push_back(v);
        }
        size = vertexOutwards.size();
}


void DirectedGraph::print()
{
    std::cout<<"Size of graph: "<<numberOfNodes()<<std::endl;
    std::cout<<"Edges:"<<std::endl;
    for (const DirectedEdge& e: edges)
    {
        e.print();
    }
}


int main()
{
    std::vector<Vertex> V;
    for (int i=0; i<10; i++)
    {
        V.emplace_back(i);
    }
    std::vector<DirectedEdge> EL;
    for (int i=0; i<9; i++)
    {
        EL.emplace_back(V[i],V[i+1]);
    }
    DirectedGraph G(EL);
    G.print();
    return 0;
}