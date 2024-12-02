//Nikita Dreizin
//2403124
//dreizin@chapman.edu
//CSPC 350-02
//Programming Assignment 6
//Class that creates a undirected, weighted graph
#include "WGraph.h"
#include "Edge.h"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <limits>
#include <set>
#include <stack>
#include <vector>
#include <fstream>


using namespace std;

WGraph::WGraph() {
  m_size = 0;
  m_adj = NULL;
  m_conn = NULL; 
  m_graphCost = 0;
}

WGraph::WGraph(unsigned int sz) {
  InitializeSize(sz);
}

void WGraph::InitializeSize(unsigned int sz){
  m_size = sz;
  m_graphCost = 0;
  // allocate sz * sz adj matrix
  m_adj = new double *[sz];
  m_conn = new double *[sz];
  for (int i = 0; i < m_size; ++i) {
    m_adj[i] = new double[sz];
    m_conn[i] = new double[sz];
  }
  // start with edges
  for (int i = 0; i < m_size; ++i) {
    for (int j = 0; j < m_size; ++j) {
      m_adj[i][j] = numeric_limits<double>::max(); // MAX_DOUBLE
      m_conn[i][j] = numeric_limits<double>::max();
    }
  }
}

WGraph::~WGraph() {
  // release memory
  for (int i = 0; i < m_size; ++i) {
    delete[] m_adj[i];
    delete[] m_conn[i];
  }
  delete[] m_adj;
  delete[] m_conn;
}



bool WGraph::initializeFromFile(string fileName) {
  ifstream graphFile(fileName);
  string line; 
  double weight = 0; 
  string end; 

  if (graphFile.is_open()) {
    // first line is our size of the NxN graph
    graphFile >> m_size; 
    InitializeSize(m_size); // need to allocate memory for matrix

    for (int i = 0; i < m_size; i++) { // each row
      for (int j = 0; j < m_size; j++) { // each column 
        graphFile >> weight; 
    
        if (weight != 0) {
          addEdge(i, j, weight); 
        }
        weight = 0; 
      }
    }
    
    graphFile >> end; 
    if (end == "END") {
      cout << "File read and initialized succesfully" << "\n";
      graphFile.close(); 
      return true;  
    }
  }
  else {
    cout << "File not initialized" << "\n";
    return false;
  }

  cout << "File not opened." << "\n";
  return false; 
}

void WGraph::printGraph() {
  
  // print to terminal
  for (int i = 0; i < m_size; i++) {
    for (int j = 0; j < m_size; j++) {
      if (m_adj[i][j] == numeric_limits<double>::max()) {
        cout << 0.0 << " ";
      }
      else {
        cout << m_adj[i][j] << " ";
      }
    }
    cout << "\n";
  }

}

double WGraph::getCost() { return m_graphCost; }

void WGraph::addEdge(VertexID i, VertexID j, double w) {
  if (i < m_size && j < m_size) {
    m_adj[i][j] = w;
    m_adj[j][i] = w;
    m_graphCost += w;
  }
}

void WGraph::removeEdge(VertexID i, VertexID j) {
  if (i < m_size && j < m_size) {
    m_graphCost -= m_adj[i][j];
    m_adj[i][j] = numeric_limits<double>::max();
    m_adj[j][i] = numeric_limits<double>::max();
  }
}

bool WGraph::areAdjacent(VertexID i, VertexID j) {
  return (m_adj[i][j] < numeric_limits<double>::max());
}

// floyd warshall algo
void WGraph::calcFW() {
  for (int i = 0; i < m_size; ++i) {
    for (int j = 0; j < m_size; ++j) {
      m_conn[i][j] = m_adj[i][j]; // start with conn == adj matrix
    }
  }

  for (int im = 0; im < m_size; ++im) { // transitive closure
    for (int source = 0; source < m_size; ++source) {
      for (int sink = 0; sink < m_size; ++sink) {
        // every possible pair of source,sink and intermediate node
        if (source == sink) {
          continue;
        }
        if (m_conn[source][im] != numeric_limits<double>::max() &&
            m_conn[im][sink] != numeric_limits<double>::max() &&
            m_conn[source][sink] > m_conn[source][im] + m_conn[im][sink]) {
          m_conn[source][sink] = m_conn[source][im] + m_conn[im][sink];
        }
      }
    }
  }
}


WGraph *WGraph::computeMST() {

  // create a dynamic array of edges that we will sort 
  Edge *EdgesArray = new Edge[m_size * m_size / 2];
  // check to make sure its been allocated
  if (EdgesArray == NULL) {
    cout << "ERROR: Out of memory\n";
    return NULL;
  }

  // MST graph to return, intialized with m_size
  WGraph *mstGraph = new WGraph(m_size);
  if (mstGraph == NULL) {
    cout << "ERROR: Out of Memory\n";
    return NULL;
  }

  int EdgeCount = 0;

  for (int i = 0; i < m_size; i++) {
    for (int j = 0; j < m_size; j++) {
      // if there is an edge between these vertexes I and J
      if (m_adj[i][j] < numeric_limits<double>::max()) {
        // check if inserted already in other direction
        // only populated array to the edgescount, so will be checking up to it
        // for duplicates not the full allocated size of the array
        if (EdgesArray[EdgeCount].EdgeExists(EdgesArray, i, j, EdgeCount) ==
            false) {
          EdgesArray[EdgeCount].Initialize(i, j, m_adj[i][j]);
          EdgeCount++;
        }
      }
    }
  }


  // Sort the array in increasing order based of weight
  // assuming the array is the size of the number of actually inserted edges
  qsort(EdgesArray, EdgeCount, sizeof(Edge), Edge::CompareEdge);

  // we will implement union-find using a vector container of sets of VertexID
  vector<set<VertexID>> VertixGroupArray(m_size);

  // for each edge in the edges array we just sorted in ascending order
  for (int edgeCounter = 0; edgeCounter < EdgeCount; edgeCounter++) {

    int edgNodeFromGrp;
    int edgNodeToGrp;

    // check if source vertex is part of any existing group
    for (edgNodeFromGrp = 0; edgNodeFromGrp < m_size; edgNodeFromGrp++) {
      // if we found it in one of the sets don't look for any others, will
      // onlyexist in one
      if (VertixGroupArray[edgNodeFromGrp].find(
              EdgesArray[edgeCounter].NodeFrom) !=
          VertixGroupArray[edgNodeFromGrp].end())
        break;
    }

    // and if the destination vertex is part of any existing group
    for (edgNodeToGrp = 0; edgNodeToGrp < m_size; edgNodeToGrp++) {
      if (VertixGroupArray[edgNodeToGrp].find(EdgesArray[edgeCounter].NodeTo) !=
          VertixGroupArray[edgNodeToGrp].end())
        break;
    }

    // now we have groups for source and destination verxes respectiveley.

    // the edge vertexes do not exist in any group have to create a new group.
    // by default will create a group based on the source vertex
    if (edgNodeFromGrp == m_size && edgNodeToGrp == m_size) {
      int NodeFrom = EdgesArray[edgeCounter].NodeFrom;
      VertixGroupArray[NodeFrom].insert(EdgesArray[edgeCounter].NodeFrom);
      VertixGroupArray[NodeFrom].insert(EdgesArray[edgeCounter].NodeTo);
    }

    // the source vertix exists in some group. Destination does not exist in any
    // group. Will add destination vertex to the group where source vertix is
    // already present
    if (edgNodeFromGrp != m_size && edgNodeToGrp == m_size) {
      VertixGroupArray[edgNodeFromGrp].insert(EdgesArray[edgeCounter].NodeTo);
    }

    // now destnation vertix exists and source does not exist. insert source
    // into dest group
    if (edgNodeToGrp != m_size && edgNodeFromGrp == m_size) {
      VertixGroupArray[edgNodeToGrp].insert(EdgesArray[edgeCounter].NodeFrom);
    }

    // if the source exists in some group and destination exists in some group
    // but they are not the same we need to merge them in to a new group. By
    // default will merge destanation droup in to source group.
    if (edgNodeFromGrp != m_size && edgNodeToGrp != m_size && edgNodeFromGrp != edgNodeToGrp) {
      set<VertexID>::iterator iterator =
          VertixGroupArray[edgNodeToGrp].begin();
      while (iterator != VertixGroupArray[edgNodeToGrp].cend()) {
        VertixGroupArray[edgNodeFromGrp].insert(*iterator);
        iterator++;
      }
      VertixGroupArray[edgNodeToGrp].clear();
    }

    // source and destination vertexes exist and are in the same group
    // Can't add vertexes to any group and must discard this edge as it'll create
    // a cycle in the graph
    if (edgNodeFromGrp == edgNodeToGrp && edgNodeFromGrp != m_size && edgNodeToGrp != m_size) {
      continue;
    }

    // if we are here the edge is good and we need to add it to the MST
    mstGraph->addEdge(EdgesArray[edgeCounter].NodeFrom,
                      EdgesArray[edgeCounter].NodeTo,
                      EdgesArray[edgeCounter].edgeWeight);
  }

  return mstGraph; // return new MST graph
}



double WGraph::cheapestCost(VertexID i, VertexID j) { return m_conn[i][j]; }
