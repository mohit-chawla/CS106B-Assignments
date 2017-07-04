/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-7-trailblazer-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/trailblazer.html
 * date: 3 July 2017
 */
#include "trailblazer.h"
#include "queue.h"
#include "hashmap.h"
#include "stack.h"
using namespace std;

//Function to get index of an element in a vector
int getElementIndex(Vertex* elem, Vector<Vertex*> vec){
    Vector<Vertex*>::iterator it;
    int i=0;
    for(it=vec.begin();it!=vec.end();++it){
        if(*it == elem){
            return i;
        }
        i++;
    }
    return -1;
}

bool dfs(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*> &path, HashMap<Vertex*,bool> visited){
   //add v1 to path and mark as visited -> GREEN
   start->setColor(GREEN);
   if(start == end){
       return true;
   }else{
        Set<Vertex*> neighbours = graph.getNeighbors(start);
        Set<Vertex*>::iterator n;
        for(n=neighbours.begin();n!=neighbours.end();++n){
            Vertex* thisNeighbour = *n;
            if(!visited.containsKey(thisNeighbour)){
                visited.add(thisNeighbour,true);
                if(dfs(graph,thisNeighbour,end,path,visited)){
                    path.add(thisNeighbour);
                    return true;
                }
                //remove element from path -> GREY
                thisNeighbour->setColor(GRAY);
                visited.remove(thisNeighbour);
            }

        }
   }
   return false;
}

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    //reset graph before starting
    graph.resetData();
    Vector<Vertex*> path;
    HashMap<Vertex*,bool> visited;
    visited.add(start,true);
    dfs(graph,start,end,path,visited);
    //Path contains reverse elements
    Vector<Vertex*> pathToReturn;
    pathToReturn.add(start);
    for(int i=path.size()-1;i>=0;i--)
        pathToReturn.add(path[i]);
    return pathToReturn;
}

//ITERATIVE
//Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
//    //reset graph before starting
//    graph.resetData();
//    Vector<Vertex*> path;
//    Stack<Vertex *> stack;
//    stack.push(start);
//    while(!stack.isEmpty()){
//        Vertex* v = stack.pop();

//        if(!v->visited){
//            v->setColor(GREEN);
//            v->visited = true;
//            path.add(v);
//        }
//        if(v==end){
//            return path;
//        }
//        Set<Vertex*> neighbours = graph.getNeighbors(v);
//        Set<Vertex*>::iterator n;
//        for(n=neighbours.begin();n!=neighbours.end();++n){
//            Vertex* thisNeighbour = *n;
//            if(!thisNeighbour->visited){
//                stack.push(thisNeighbour);
//            }
//       }

//    }

//    return path;
//}
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    //reset graph before starting
    graph.resetData();
    Set<Vertex*> vertices = graph.getVertexSet();
    Set<Vertex*>::iterator n;
    Vector<Vertex*> path;
    Queue<Vertex *> queue;

    queue.add(start);
    start->visited = true;
    while(!queue.isEmpty()){
        Vertex *v = queue.dequeue(); //->GREEN
        v->setColor(GREEN);
        if(v == end){
            break;
        }
        Set<Vertex*> neighbours = graph.getNeighbors(v);
        Set<Vertex*>::iterator n;
        for(n=neighbours.begin();n!=neighbours.end();++n){
            //for each neighbour of this vertex
            Vertex* thisNeighbour = *n;
            if(!thisNeighbour->visited){
                thisNeighbour->visited = true; //mark as visited
                thisNeighbour->previous = v;
                queue.add(thisNeighbour); //add to queue -->YELLOW
                thisNeighbour->setColor(YELLOW);
            }
        }
    }
    Vertex* prev = end;
    Vector<Vertex *> reversePath;
    while(prev!=start){
        reversePath.add(prev);
        prev = prev->previous;
    }
    reversePath.add(start);
    for(int i=reversePath.size()-1;i>=0;i--)
        path.add(reversePath[i]);
    return path;
}

Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    //reset graph before starting
    graph.resetData();
    Vector<Vertex*> path;
    return path;
}

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty vector so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    Vector<Vertex*> path;
    return path;
}

Set<Edge*> kruskal(BasicGraph& graph) {
    // TODO: implement this function; remove these comments
    //       (The function body code provided below is just a stub that returns
    //        an empty set so that the overall project will compile.
    //        You should remove that code and replace it with your implementation.)
    Set<Edge*> mst;
    return mst;
}
