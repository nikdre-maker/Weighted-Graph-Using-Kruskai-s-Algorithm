//Nikita Dreizin
//2403124
//dreizin@chapman.edu
//CSPC 350-02
//Programming Assignment 6
//Class that creates an Edge object which holds weight of each edge and the node indexes 
#include "Edge.h"
#include "WGraph.h"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <limits>
#include <set>
#include <stack>

using namespace std;


int Edge::Compare(Edge *EdgeA, Edge *EdgeB) {
  return CompareEdge(EdgeA, EdgeB);
}

// default
Edge::Edge() {
  NodeFrom = -1;
  NodeTo = -1;
  edgeWeight = numeric_limits<double>::max();
}

Edge::Edge(VertexID source, VertexID dest, double weight) {
  Initialize(source, dest, weight);
}

// to directly access array object and assign it values
void Edge::Initialize(VertexID source, VertexID dest, double weight) {
  NodeFrom = source;
  NodeTo = dest;
  edgeWeight = weight;
}

//qsort will use this 
int Edge::CompareEdge(const void *EdgeA, const void *EdgeB) {
  if (((Edge *)EdgeA)->edgeWeight < ((Edge *)EdgeB)->edgeWeight) {
    return -1;
  } else {
    return 1;
  }
}

// iterate through array and check for insertion values both ways
bool Edge::EdgeExists(Edge *Array, VertexID i, VertexID j, int arraySize) {
  for (int i = 0; i < arraySize; i++) {
    if (Array[i].NodeFrom == i && Array[i].NodeTo == j)
      return true;
    
    if (Array[i].NodeFrom == j && Array[i].NodeTo == i)
      return true;
  }
  return false;
}