//Nikita Dreizin
//2403124
//dreizin@chapman.edu
//CSPC 350-02
//Programming Assignment 6
//Header file for WGraph class
#ifndef WGraph_H
#define WGraph_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <limits>
#include <set>
#include <stack>
#include <string> 

// an unweighted, undirected WGraph implemented with adjacency matrix
// fixed size

typedef unsigned int VertexID;



class WGraph {
public:
  WGraph();
  WGraph(unsigned int sz);
  ~WGraph();
  void addEdge(VertexID i, VertexID j, double w);
  void removeEdge(VertexID i, VertexID j);
  bool areAdjacent(VertexID i, VertexID j);
  double cheapestCost(VertexID i, VertexID j);
  void calcFW();
  WGraph* computeMST();
  bool initializeFromFile(std::string fileName); 
  void printGraph(); 
  double getCost();
  void InitializeSize(unsigned int sz); 

private:
  double **m_adj;
  double **m_conn;
  unsigned int m_size; 
  double m_graphCost; // total cost of the graph

};

#endif







