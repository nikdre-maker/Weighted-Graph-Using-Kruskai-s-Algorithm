//Nikita Dreizin
//2403124
//dreizin@chapman.edu
//CSPC 350-02
//Programming Assignment 6
//Main class that runs the program utilizing WGraph methods
#include "WGraph.h"
#include <fstream>
#include <iostream>

using namespace std;

// method to get cmd line argument of text file used for program
bool GetUserInput(string &inputFile, char **argv) {

  cout << "Please input the Graph File name: ";
  cin >> inputFile;
 
  // input file name can't be empty
  if (inputFile.size() == 0) {
    cout << "Input Graph File can not be empty. " << "\n";
    return false;
  }

  return true;
}

int main(int argc, char **argv) {
  
  WGraph inputGraph;       // Graph made from file
  WGraph *mstGraph = NULL; // Pointer to MST graph created by MST algorithm

  string graphFile;
  
  if (GetUserInput(graphFile, argv) == false) {
    cout << "ERROR: invalid user input";
    return 0;
  }

  // take file input and create graph from file input
  if (inputGraph.initializeFromFile(graphFile) == false) {
    cout << "ERROR: failed to initialize graph from file " << graphFile;
    return 0;
  }

  

  // create the MST from input graph. Returns pointer to MST Graph
  mstGraph = inputGraph.computeMST();
  if (mstGraph == NULL) {
    cout << "ERROR: Failed to create MST Graph";
    return 0;
  }

  // print out the mst 
  mstGraph->printGraph();

  // print out the total cost
  cout << "Graph MST Cost is: " << mstGraph->getCost();

  // dont' forget to delete the MST graph we created inside computeMST
  if (mstGraph != NULL) {
    delete mstGraph;
  }

  return 0;
}

// compile command: g++ *.cpp -o myprogram