// Copyright 2021 András Mihálykó MIT License

#pragma once

#include <vector>
#include <list>
#include <iostream>
#include <map>
#include <memory>


#include "linkedlist.h"

struct Edge {
 private:
        int epi, epj;

 public:
        Edge(int i, int j) : epi(i), epj(j) {}
        std::vector<int> getEdge() const {
            std::vector<int> v = {epi, epj};
            return v;
        }
        void print() const {
            std::cout << epi << " " << epj <<std::endl;
        }
};

class SimpleGraph {
// the very simple basic graph implementation, nodes indexed by ints
 private:
    int numberOfVertices;
    std::vector<Edge> edgeList;
    std::map<int, std::vector<int> > neighborLists;

 public:
    SimpleGraph() {}
    explicit SimpleGraph(int n) : numberOfVertices(n) {
        neighborLists = std::map<int, std::vector<int> >();}
    SimpleGraph(int n, const std::vector<int>& I, const std::vector<int>& J) {
        numberOfVertices = n;
        neighborLists = std::map<int, std::vector<int> >();

        if (I.size() == J.size()) {
            for (int i = 0; i < I.size(); i++) {
                edgeList.emplace_back(I[i], J[i]);
                neighborLists[I[i]].push_back(J[i]);
                neighborLists[J[i]].push_back(I[i]);
            }
        } else {
            std::cerr << "Two list not the same length" << std::endl;
            throw 10;
        }
    }

    inline int getNumberOfNodes() const {return numberOfVertices;}
    inline void addEdge(int i, int j) {
        edgeList.emplace_back(i, j);
        neighborLists[i].push_back(j);
        neighborLists[j].push_back(i);
    }
    inline size_t getNumberOfEdges() const {return edgeList.size();}
    std::vector<Edge> getEdges() const {return edgeList;}
    std::vector<int> getNeighbors(int i) {return neighborLists[i];}

    void readFromInput();
};


class HyperEdge;
class DirectedHyperEdge;


class Vertex {
    /* 
    Basic vertex class.
    Uniquely identified by integer ID.
    */
 private:
        int name;  // id from outside
        int id;  // position in the vectors

 public:
        Vertex() {}
        explicit Vertex(int id_):name(id_), id(id_) {}
        Vertex(int name_, int id_):name(name_), id(id_) {}

        ~Vertex() {}

        inline int getId() const {return id;}
        inline int getName() const {return name;}


        inline bool operator==(const Vertex &v) const {return id == v.getId();}
        inline bool operator<(const Vertex &v) const {return id < v.getId();}
        inline bool operator>(const Vertex &v) const {return id > v.getId();}

        inline void print() const {
                std::cout << name;
        }
};

class HyperEdge {
 protected:
        std::vector<std::shared_ptr<Vertex> > vertices;
        bool stillExistsing;
        int id;  // id for identification in the vectors
        // we don't need to delete the hyperedges "physically"
        // just note that they are not useful any more
        // and we clean up the vectors for freeing up space


 public:
        HyperEdge(const std::vector<std::shared_ptr<Vertex> >& v_, int id_) :
                vertices(v_), id(id_) {stillExistsing = true;}

        ~HyperEdge() {}

        int getId() const {return id;}


        inline std::vector<std::shared_ptr<Vertex> > getVertices() const {return vertices;}
        inline void print() const {
            for (const auto& v : vertices) {
                    v->print();
                    std::cout << " ";
                }
                std::cout << std::endl;
        }
        inline bool isStillExistsing() const {return stillExistsing;}
        inline void deleteHyperedge() {
            stillExistsing = false;
            vertices = std::vector<std::shared_ptr<Vertex> >();
        }
};

class DirectedHyperEdge {
 private:
        std::shared_ptr<HyperEdge>hyperEdge;
        std::shared_ptr<Vertex> head;

 public:
        DirectedHyperEdge() {}
        DirectedHyperEdge(std::shared_ptr<Vertex> head_, std::shared_ptr<HyperEdge> hyperedge_)
         : head(head_), hyperEdge(hyperedge_) {}
        ~DirectedHyperEdge() {}
        inline std::shared_ptr<Vertex> getHead() const {return head;}
        inline void setHead(std::shared_ptr<Vertex> v) {head = v;}
        inline std::shared_ptr<HyperEdge> getHyperEdge() {return hyperEdge;}
        void changeUnderlyingEdge(std::shared_ptr<HyperEdge> hyperEdge_);

        inline void print() const {
            std::cout << "Head: ";
            head->print();
            std::cout << std::endl;
            std::cout << "Hyperedge: ";
            hyperEdge->print();
        }
};

class DirectedHyperGraph {
    /*
        Class for directed hypergraph. Every hyperedge has one head and multiple tail.
        It does not implement any funcionality over the basics.
        Funconalities regarding (k,l)-sparsity concern the M_compHyperGraph class
    */
 protected:
        std::vector<std::shared_ptr<Vertex> > vertices;  // indexed by the ID of the vertex
        std::map<int, int> vertexNameToID;
        std::vector<std::shared_ptr<List <std::shared_ptr<DirectedHyperEdge> > > > headOfHyperEdge;
        // indexed by the ID of the vertex

        std::vector<int> inDegree;  // indexed by the ID of the vertex

        std::list<std::shared_ptr<DirectedHyperEdge> > directedHyperEdges;
        std::vector<std::shared_ptr<HyperEdge> > undirectedHyperEdges;
        size_t size;

        inline List<std::shared_ptr<DirectedHyperEdge> >* headOf(int id) {
            return headOfHyperEdge[id].get();
        }

        inline List<std::shared_ptr<DirectedHyperEdge> >* headOf(const std::shared_ptr<Vertex>& v) {
            return headOf(v->getId());
        }

        inline void decreaseInDegree(int id) {
            if (inDegree[id] > 0) {
                inDegree[id]--;
            } else {
                std::cerr << "Degree of vertex ";
                vertices[id]->print();
                std::cerr << " decreased under 0." << std::endl;
                throw 11;
            }
        }
        inline void decreaseInDegree(const std::shared_ptr<Vertex>& v) {decreaseInDegree(v->getId());}

        inline void increaseInDegree(int id) {inDegree[id]++;}
        inline void increaseInDegree(const std::shared_ptr<Vertex>& v) {increaseInDegree(v->getId());}

 public:
        DirectedHyperGraph() {}
        explicit DirectedHyperGraph(size_t n) {
            size = n;
            for (size_t i = 0; i < size; i++) {
                insertNewVertex(i);
            }
        }

        explicit DirectedHyperGraph(const SimpleGraph& G) {
            size = G.getNumberOfNodes();

            std::vector<Edge> graphEdges = G.getEdges();

            for (const auto& e : graphEdges) {
                std::vector<int> edge = e.getEdge();
                insertNewVertex(edge[0]);
                insertNewVertex(edge[1]);
                addDirEdge(vertices[edge[0]], vertices[edge[1]]);
            }
        }

        DirectedHyperGraph(const DirectedHyperGraph& HG) {
            vertices = HG.vertices;
            directedHyperEdges = HG.directedHyperEdges;
            undirectedHyperEdges = HG.undirectedHyperEdges;
            size = HG.size;
            headOfHyperEdge = HG.headOfHyperEdge;
            inDegree = HG.inDegree;
        }

        ~DirectedHyperGraph() {}

        void inline insertNewVertex(int name) {  // insert new vertex only if id is not contained
            int id = static_cast<int>(vertices.size());
            if (vertexNameToID.find(name) == vertexNameToID.end()) {
                vertexNameToID[name] = id;
                vertices.push_back(std::make_shared<Vertex>(name, id));
                headOfHyperEdge.push_back(std::make_shared<List <std::shared_ptr<DirectedHyperEdge> > >());
                // add new List of hyperedges it can be head of, too
                inDegree.push_back(0);
            }
        }

        inline size_t getNumberOfVertices() const {return size;}
        inline size_t getNumberOfEdges() const {return directedHyperEdges.size();}

        inline std::shared_ptr<Vertex> getVertex(int i) {return vertices[i];}
        inline int getInDegree(int id) {return inDegree[id];}
        inline int getInDegree(const std::shared_ptr<Vertex>& v) {return inDegree[v->getId()];}

        inline std::vector<std::shared_ptr<HyperEdge> >* getUndirectedHyperEdges() {return &undirectedHyperEdges;}

        void addHyperEdge(std::shared_ptr<HyperEdge> edge, const std::shared_ptr<Vertex>& head);
        void addDirEdge(const std::shared_ptr<Vertex>& head, const std::shared_ptr<Vertex>& tail);
        void changeDirection(DirectedHyperEdge& edge, const std::shared_ptr<Vertex>& to);

        void readFromInput();
        void print() const;
        void printUndirectedHyperedges() const;
};

