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
    private:
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
            edges = std::vector<Edge>(0);
        }

        Graph(const std::vector<Edge>& edgeList);

        size_t numberOfNodes(){return size;}
        void print();

};


/*class DirectedEdge: public Edge
{
    private:
        Vertex u, v;
        
    public:
        edge(){};
        inline edge(Vertex u_, Vertex v_):u(u_), v(v_){};
        inline std::vector<Vertex> getEndVertices() const {return {u,v};}
        inline void print() const {
            u.print();
            std::cout<<" ";
            v.print();
            std::cout<<std::endl;
        }
};
*/

#endif