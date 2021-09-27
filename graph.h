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
        HyperEdge *hyperEdge;
    public:
            Vertex *head;

        DirectedHyperEdge(){}
        DirectedHyperEdge(Vertex* head_, HyperEdge* hyperedge_):head(head_),hyperEdge(hyperedge_){}
        ~DirectedHyperEdge(){}
        inline Vertex getHead()const {return *head;}
        inline void setHead(const Vertex& v) {*head = v;}
        inline void print() const {
            std::cout<<"Head: ";
            head->print();
            std::cout<<std::endl;
            std::cout<<"Hyperedge: ";
            hyperEdge->print();
        }
};

struct Edge
{
    private:
        int epi, epj;
            
    public:
        Edge(int i, int j):epi(i),epj(j){}
        std::vector<int> getEdge() const{std::vector<int> v={epi, epj}; return v;}
};


class SimpleGraph
{// the very simple basic graph implementation, easy nodes, simple edge list
    private:
        int numberOfVertices;
        std::vector<Edge> edgeList;

    public:
    SimpleGraph(){}
    SimpleGraph(int n):numberOfVertices(n){}
    SimpleGraph(int n, const std::vector<int>& I, const std::vector<int>& J)
    {
        numberOfVertices = n;
        if (I.size() == J.size())
            for (int i = 0; i<=I.size(); i++ )
                edgeList.emplace_back(I[i],J[i]);
        else
        {
            //EXCEPTION
            std::cout<<"Two list not the same length"<<std::endl;
        }
    }

    inline int getNumberOfNodes() const {return numberOfVertices;}
    inline void addEdge( int i, int j){edgeList.emplace_back(i,j);}
    inline size_t getNumberOfEdges() const {return edgeList.size();}
    std::vector<Edge> getEdges() const {return edgeList;}
    void readFromInput();
};

class DirectedHyperGraph
{
    protected:
        std::vector<Vertex> vertices;
        std::list<DirectedHyperEdge> directedHyperEdges;
        std::list<HyperEdge> undirectedHyperEdges;
        size_t size;

    public:
        DirectedHyperGraph(){}
        DirectedHyperGraph(size_t n){
            size=n;
            for (size_t i=0; i<size; i++)
            {
                vertices.emplace_back(i);
            }
        }

        DirectedHyperGraph(SimpleGraph G){
            size=G.getNumberOfNodes();
            for (size_t i=0; i<size; i++)
            {
                vertices.emplace_back(i);
            }
            std::vector<Edge> graphEdges=G.getEdges();

            for (const auto& e: graphEdges){
                std::vector<int> edge=e.getEdge();
                std::vector<Vertex*> edgeVertices={&vertices[edge[0]],&vertices[edge[1]]};

                HyperEdge newHyperEgde(edgeVertices);

                addHyperEdge(newHyperEgde,vertices[edge[0]]);
            }
        }

        ~DirectedHyperGraph(){}
        inline size_t getNumberOfNodes() const {return size;}
        void print() const;

        void addHyperEdge(HyperEdge& edge, Vertex& head);
        void changeDirection(DirectedHyperEdge& edge, Vertex& to); 

        void readFromInput();
};

#endif