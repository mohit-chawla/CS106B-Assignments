/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-7-trailblazer-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/trailblazer.html
 * date: 3 July 2017
 */
#include "trailblazer.h"
#include "queue.h"

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

void dfs(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*> &path){
   //add v1 to path and mark as visited -> GREEN
   path.add(start);
   start->setColor(GREEN);
//   start->visited = true;
   graph.getVertex(start->name)->visited = true;
   if(start == end){
       cout<<"YAY";
       return;
   }else{
        Set<Vertex*> neighbours = graph.getNeighbors(start);
        Set<Vertex*>::iterator n;
        for(n=neighbours.begin();n!=neighbours.end();++n){
            Vertex* thisNeighbour = *n;
            cout<<thisNeighbour->name<<endl;
            if(!thisNeighbour->visited){
                dfs(graph,thisNeighbour,end,path);
            }
            //remove element from path -> GREY
            start->setColor(GRAY);
//            start->visited = false;
//            path.remove(getElementIndex(start,path));
        }
   }
   return;
}

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    //reset graph before starting
    graph.resetData();
    Vector<Vertex*> path;
    dfs(graph,start,end,path);
    return path;
}

void bfs(BasicGraph& graph, Vertex* start, Vertex* end){
    Queue<Vertex *> queue;
    queue.add(start);
    start->visited = true;
    while(!queue.isEmpty()){
        Vertex *v = queue.dequeue(); //->GREEN
        v->setColor(GREEN);
        if(v == end){
            return;
        }
        Set<Vertex*> neighbours = graph.getNeighbors(start);
        Set<Vertex*>::iterator n;
        for(n=neighbours.begin();n!=neighbours.end();++n){
            //for each neighbour of this vertex
            Vertex* thisNeighbour = *n;
            thisNeighbour->visited = true; //mark as visited
            queue.add(thisNeighbour); //add to queue -->YELLOW
            thisNeighbour->setColor(YELLOW);
        }
    }
}

Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    //reset graph before starting
    graph.resetData();
    bfs(graph,start,end);
    Set<Vertex*> vertices = graph.getVertexSet();
    Set<Vertex*>::iterator n;
    Vector<Vertex*> path;

    //construct path
    for(n=vertices.begin();n!=vertices.end();++n){
        Vertex* v = *n;
        if(v->getColor() == GREEN)
            path.add(v);
    }
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
