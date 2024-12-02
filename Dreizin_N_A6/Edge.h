//Nikita Dreizin
//2403124
//dreizin@chapman.edu
//CSPC 350-02
//Programming Assignment 6
//Header file for Edge cpp 
#ifndef EDGE_H
#define EDGE_H

#include "WGraph.h"

class Edge {
public:
  // Source Node, Destination Node, Link/Edge Weight
  Edge();
  Edge(VertexID, VertexID, double);
  void Initialize(VertexID, VertexID, double);
  void checkRepeatingEdge();
  int Compare(Edge *, Edge *);
  bool EdgeExists(Edge *, VertexID, VertexID, int);
  static int CompareEdge(const void *EdgeA, const void *EdgeB);
  VertexID NodeFrom;
  VertexID NodeTo;
  double edgeWeight;
};


#endif