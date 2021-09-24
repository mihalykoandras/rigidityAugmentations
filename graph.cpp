#include "graph.h"


void DirectedHyperGraph::print() const
{
    std::cout<<"Number of Nodes: "<<getNumberOfNodes()<<std::endl;
    std::cout<<"Edges:"<<std::endl;
    for (auto const& e: hyperEdges)
    {
        e.print();
    }
}




int main()
{
    size_t n;
    std::cin>>n;
    DirectedHyperGraph G(n);
    G.print();
}