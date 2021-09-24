#ifndef GRAPH_H 
#define GRAPH_H

#include <vector>
#include <list>
#include <iostream>

class HyperEdge;
class DirectedHyperEdge;

class Vertex
{
    private:
        int id;
        int inDegree;
    public: 
        Vertex(){}
        explicit Vertex(int id_):id(id_){inDegree=0;}
        ~Vertex(){/*isInHyperEdge.erase(); headOfHyperEdge.erase();*/}

        inline int getId() const {return id;}
        inline bool operator==(const Vertex &v) const {return id==v.getId();}
        inline bool operator<(const Vertex &v) const {return id<v.getId();} //for efficient search
        inline bool operator>(const Vertex &v) const {return id>v.getId();} //for efficient search
        inline void print()const {std::cout<<id;}
        inline int getInDegree() const {return inDegree;}
        inline void decreaseInDegree(){inDegree--;} //TODO check if in-degree is >0
        inline void increaseInDegree(){inDegree++;}
        std::list<HyperEdge*> isInHyperEdge;
        std::list<DirectedHyperEdge*> headOfHyperEdge;
};

class HyperEdge
{
    protected:
        std::vector<Vertex*> vertices;    

    public:
        HyperEdge(const std::vector<Vertex*>& v_):vertices(v_){}

        ~HyperEdge(){/*vertices.clear();*/}
        inline std::vector<Vertex*> getVertices() const {return vertices;}
        inline void print() const {
            for (const auto& v : vertices)
                {
                    v->print();
                    std::cout<<" ";
                }
                std::cout<<std::endl;
        }
};

class DirectedHyperEdge
{
    private:
        Vertex *head;
        HyperEdge *hyperEdge;
    public:
        DirectedHyperEdge(){}
        DirectedHyperEdge(Vertex* head_, HyperEdge* hyperedge_):head(head_),hyperEdge(hyperedge_){}
        ~DirectedHyperEdge(){}
        inline Vertex getHead()const {return *head;}
        inline void setHead(const Vertex& v) { *head = v;}
};


class DirectedHyperGraph
{
    protected:
        std::vector<Vertex> vertices;
        std::list<DirectedHyperEdge> directedHyperEdges;
        std::list<HyperEdge> hyperEdges;
        size_t size;
    public:
        DirectedHyperGraph(){}
        DirectedHyperGraph(size_t n){
            size=n;
            for (size_t i=0; i<n; i++)
            {
                vertices.emplace_back(i);
            }
        }

        ~DirectedHyperGraph(){}
        size_t getNumberOfNodes() const {return size;}
        void addEdge(const Vertex& head);
        void addHyperEdge();
        void changeDirection(const Vertex& from, const Vertex& to); 
        void print() const;

};

#endif