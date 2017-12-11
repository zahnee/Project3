// Digraph.cpp -- directed graph class
// c. 2017 T. O'Neil

#include "Digraph.hpp"
#include <climits>
#include <vector>
#include <map>
#include <iostream>

unsigned int Digraph::noVertices(){
    return numberOfVertices;
}

unsigned int Digraph::noEdges(){
    return numberOfEdges;
}

void Digraph::resetEdges(){
    for(unsigned int i=0; i < numberOfVertices; i++){
        for(unsigned int j=0; j < numberOfVertices; j++){
            if(i==j){
                distMatrix[i][j] = 0;
            }else{
                distMatrix[i][j] = INT_MAX;
            }
        }
    }
    numberOfEdges = 0;
}

void Digraph::addEdge(unsigned int source,unsigned int dest,unsigned int wt){
    if(source==dest)
        distMatrix[source][dest] = 0;
    else{
    distMatrix[source][dest] = wt;
    numberOfEdges++;
    }
}

void Digraph::delEdge(unsigned int source,unsigned int dest){
    if(source==dest)
        distMatrix[source][dest] = 0;
    else{
        distMatrix[source][dest] = INT_MAX;
        numberOfEdges--;
    }
}

unsigned int Digraph::isEdge(unsigned int source,unsigned int dest){
    if(distMatrix[source][dest] == INT_MAX)
        return 0;
    else
        return distMatrix[source][dest];
}

unsigned int Digraph::dijkstra(unsigned int source,unsigned int dest){
    //used for keeping track of the final path and the shortest distance from a node to the source
    std::map<unsigned int, unsigned int> path; //(starting node, ending node)
    std::map<unsigned int, unsigned int> distance;
    //starting with source node
    path.insert(std::pair<unsigned int, unsigned int>(source,source));
    distance.insert(std::pair<unsigned int,unsigned int>(source,distMatrix[source][source]));
    //
    std::map<unsigned int, unsigned int> boarderDistance;
    std::map<unsigned int, unsigned int> boarder; //(starting node, ending node) but does not include it's self or nodes already added
    unsigned int currentNode = source;

    for(unsigned int i = 0; i < numberOfVertices; i++){
        for(unsigned int k = 0; k < numberOfVertices; k++){
            if(distMatrix[currentNode][k] != INT_MAX && distMatrix[currentNode][k] != 0){
                if(path.find(k) == path.end()){  //checking if it's already part of the final shortest path
                    if(boarder.find(k) != boarder.end()){
                        if(boarderDistance[k] > (distMatrix[currentNode][k] + distance.find(currentNode)->second)){
                            //we need to erase current the value
                            boarderDistance.erase(k);
                            boarder.erase(k);
                            //replace with the new value
                            boarderDistance.insert(std::pair<unsigned int, unsigned int>(k, (distMatrix[currentNode][k] + distance.find(currentNode)->second)));
                            boarder.insert(std::pair<unsigned int, unsigned int>(k, currentNode));
                        }
                    }else{
                        boarderDistance.insert(std::pair<unsigned int, unsigned int>(k, (distMatrix[currentNode][k] + distance.find(currentNode)->second)));
                        boarder.insert(std::pair<unsigned int, unsigned int>(k, currentNode));
                    }
                }
            }
        }
        //find the next node to add and evaluate from
        unsigned int minDistance = boarderDistance.begin()->second;
        unsigned int minNode = boarderDistance.begin()->first;
        for(std::map<unsigned int, unsigned int>::iterator iter=boarderDistance.begin(); iter!=boarderDistance.end(); ++iter){
            if(minDistance > iter->second){
                minDistance = iter->second;
                minNode = iter->first;
            }
        }
        //update and remove temp node
        path.insert(std::pair<unsigned int, unsigned int>(minNode,currentNode));
        distance.insert(std::pair<unsigned int,unsigned int>(minNode,minDistance));
        boarderDistance.erase(minNode);
        boarder.erase(minNode);
        currentNode = minNode;
    }//end of building the shortest graph
    //start output section
    currentNode = dest;
    std::vector<std::string> pathing;
    std::cout << vertex[source]->getName() << " to " << vertex[dest]->getName() << "\n\nPath:\n";
    while(path.find(currentNode)->second != currentNode){
        pathing.push_back(vertex[path.find(currentNode)->second]->getName());
        currentNode = path.find(currentNode)->second;
    }
    while(pathing.size() != 0){
        std::cout << pathing.back() << "\n";
        pathing.pop_back();
    }
    std::cout << vertex[dest]->getName() << "\n";
    std::cout << "\nDistance: " << distance.find(dest)->second << " miles\n";
    return distance.find(dest)->second;
}
//might need to make a reference to how far the current node is from the source and then add that in the previous structure above
