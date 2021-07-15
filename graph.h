#ifndef GRAPH_H 
#define GRAPH_H

#include <vector>
#include <map>
#include <iostream>

class Vertex
{
    private:
        int id;
    public: 
        Vertex(){}
        Vertex(int id_):id(id_){}
        inline int getId() const {return id;}
        inline bool operator==(const Vertex &v) const {return id==v.getId();}
        inline bool operator<(const Vertex &v) const {return id<v.getId();}//for efficient search
        inline bool operator>(const Vertex &v) const {return id>v.getId();}//for efficient search
        inline void print()const {std::cout<<id;}


};

//edge structure. May be the basis of a hyperedge structure later on
class Edge
{
    protected:
        Vertex u, v;

    public:
        Edge(){};
        inline Edge(Vertex u_, Vertex v_):u(u_), v(v_){};
        inline std::vector<Vertex> getEndVertices() const {return {u,v};}
        inline void print() const {
            u.print();
            std::cout<<" ";
            v.print();
            std::cout<<std::endl;
        }
};

class Graph
{
    private:
        std::map<Vertex,std::vector<Vertex> > vertexNeighbors;//needed for any effective search
        std::vector<Edge> edges;
        size_t size;
    public:
        inline Graph(){
            size = 0;
            vertexNeighbors = std::map<Vertex,std::vector<Vertex> >();
            edges = std::vector<Edge>();
        }

        Graph(const std::vector<Edge>& edgeList);

        size_t numberOfNodes(){return size;}
        void print();

};


class DirectedEdge: public Edge
{   /*Direction always from u to v */
    public:
        inline DirectedEdge(Vertex u_, Vertex v_):Edge(u_,v_){};
        inline void reverse(){
            Vertex h=u; u=v; v=h;
        }
        inline void print() const {
            u.print();
            std::cout<<"->";
            v.print();
            std::cout<<std::endl;
        }
};


class DirectedGraph
{/*should be more like Graph with Directed Edge included, that is, we need a super-edge that will include directed hyperedges, too.
And then restrictive inheritance for each type */
    private:
        std::map<Vertex,std::vector<Vertex> > vertexOutwards;//outwards
        std::vector<DirectedEdge> edges;
        size_t size;
    public:
        inline DirectedGraph(){
            size = 0;
            vertexOutwards = std::map<Vertex,std::vector<Vertex> >();
            edges = std::vector<DirectedEdge>();
        }

        DirectedGraph(const std::vector<DirectedEdge>& edgeList);

        size_t numberOfNodes(){return size;}
        void print();

};

#endif