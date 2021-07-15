#ifndef GRAPH_H 
#define GRAPH_H

#include <vector>
#include <map>
#include <iostream>

class vertex
{
    private:
        int id;
    public: 
        vertex(){}
        vertex(int id_):id(id_){}
        inline int getId() const {return id;}
        inline bool operator==(const vertex &v) const {return id==v.getId();}
        inline bool operator<(const vertex &v) const {return id<v.getId();}//for efficient search
        inline bool operator>(const vertex &v) const {return id>v.getId();}//for efficient search
        inline void print()const {std::cout<<id;}


};

//edge structure. May be the basis of a hyperedge structure later on
class edge
{
    private:
        vertex u, v;
        bool directed;//always u is the tail, v is the head
    
    public:
        edge(){};
        inline edge(vertex u_, vertex v_, bool directed_):u(u_), v(v_),directed (directed_){};
        inline std::vector<vertex> getEndVertices() const {return {u,v};}
        inline void print() const {
            u.print();
            std::cout<<" ";
            v.print();
            std::cout<<std::endl;
        }
};

class graph
{
    private:
        std::map<vertex,std::vector<vertex> > vertexNeighbors;//needed for any effective search
        std::vector<edge> edges;
        size_t size;
    public:
        graph(){
            size = 0;
            vertexNeighbors = std::map<vertex,std::vector<vertex> >();
            edges = std::vector<edge>(0);
        }

        graph(const std::vector<edge>& edgeList);

        size_t numberOfNodes(){return size;}
        void print();

};


#endif