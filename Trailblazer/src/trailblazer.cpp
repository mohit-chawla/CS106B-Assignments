/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-7-trailblazer-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/trailblazer.html
 * date: 3 July 2017
 */
#include "trailblazer.h"
#include "queue.h"
#include "hashmap.h"
#include "stack.h"
#include "pqueue.h"

using namespace std;

//UTILITY FUNCTIONS START HERE

/*
 * Visit a vertex, set cost, set prev, set color
 * @param
 * Vertex*:vertex
 * @return
*/
void visitVertex(Vertex* vertex) {
    vertex->visited = true;
    vertex->setColor(GREEN);
}

/*
 * Enqueue a vertex, set cost, set prev, set color
 * @param
 * Vertex*:vertex
 * Vertex*:previous
 * double:cost
 * @return
*/
void enqueueVertex(Vertex* vertex, Vertex* previous, double cost) {
    vertex->cost = cost;
    vertex->previous = previous;
    vertex->setColor(YELLOW);
}

/*
 * Reverse a given vector
 * @param
 * Vector<Vertex*>& : path
 * @return
*/
void reverseVector(Vector<Vertex*>& path) {
    for(int i = 0; i < path.size() / 2; i++) {
        int j = path.size() - 1 - i;
        Vertex* tmp = path[i];
        path[i] = path[j];
        path[j] = tmp;
    }
}

/*
 * Set the path of a given graph
 * @param
 * Vertex*:start
 * Vertex*:end
 * Vector<Vertex*>&:path
 * @return
*/
void setPath(Vertex* start, Vertex* end, Vector<Vertex*>& path) {
    Vertex* vertex = end;
    while(true) {
        path.add(vertex);
        if(BasicGraph::compare(vertex, start) == 0) break;
        if(!vertex->previous) {
            path.clear();
            break;
        }
        vertex = vertex->previous;
    }
    reverseVector(path);
}

/*
 * Recursive helper function for depthFirstSearch
 * @param
 * BasicGraph&:graph
 * Vertex*:start
 * Vertex*:end
 * Vector<Vertex*>:&path
 * HashMap<Vertex*,bool>:visited
 * @return
 * bool
*/
bool dfs(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*> &path, HashMap<Vertex*,bool> visited){
   //add v1 to path and mark as visited -> GREEN
   visitVertex(start);
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


/*
 * Initialize clusters for Kruskal
 * @param
 * BasicGraph&:graph
 * Map<Vertex*, Set<Vertex*>>: &clusters
 * @return
*/
void initializeClusters(BasicGraph& graph, Map<Vertex*, Set<Vertex*>>& clusters) {
    Set<Vertex*> graphVertices = graph.getVertexSet();
    for(Vertex* vertex : graphVertices) {
        Set<Vertex*> otherVertices;  //Does a cluster include itself in connecting Vertices?
        otherVertices.add(vertex);
        clusters[vertex] = otherVertices;
    }
}

/*
 * Enqueues the edge set into the priority queue
 * @param
 * BasicGraph&:graph
 * PriorityQueue<Edge*>:&pqueue
 * @return
*/
void enqueueEdgeSet(BasicGraph& graph, PriorityQueue<Edge*>& pqueue) {
    Set<Edge*> graphEdges = graph.getEdgeSet();
    for(Edge* edge : graphEdges) {
        pqueue.enqueue(edge, edge->cost);
    }
}

/*
 * Enqueues the edge set into the priority queue
 * @param
 * Map<Vertex*, Set<Vertex*> >:&clusters
 * Vertex*:start
 * Vertex*:finish
 * @return
*/
void mergeClusters(Map<Vertex*, Set<Vertex*> >& clusters, Vertex* start, Vertex* finish) {
    Set<Vertex*> otherVertices= clusters[start] + clusters[finish];
    for(Vertex* vertex : clusters) {
        if(otherVertices.contains(vertex)) clusters[vertex] = otherVertices;
    }
}

//UTILITY FUNCTIONS END HERE

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
                thisNeighbour->setColor(YELLOW);
                queue.add(thisNeighbour); //add to queue -->YELLOW

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

    PriorityQueue<Vertex*> pqueue;
        pqueue.enqueue(start, 0.0);
        while(!pqueue.isEmpty()) {
            Vertex* current = pqueue.dequeue();
            visitVertex(current);
            if(BasicGraph::compare(current, end) == 0) break;
            for(Edge* edge : current->edges) {
                double cost = current->cost + edge->cost;
                if(!edge->finish->visited &&
                   edge->finish->getColor() != YELLOW) {
                    enqueueVertex(edge->finish, current, cost);
                    pqueue.enqueue(edge->finish, cost);
                }
                if(edge->finish->getColor() == YELLOW &&
                   cost < edge->finish->cost) {
                    enqueueVertex(edge->finish, current, cost);
                    pqueue.changePriority(edge->finish, cost);
                }
            }
        }
        setPath(start, end, path);
        return path;
}

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
     graph.resetData();
     Vector<Vertex*> path;
     PriorityQueue<Vertex*> pqueue;
     pqueue.enqueue(start, heuristicFunction(start, end));
     while(!pqueue.isEmpty()) {
         Vertex* current = pqueue.dequeue();
         visitVertex(current);
         if(BasicGraph::compare(current, end) == 0) break;
         for(Edge* edge : current->edges) {
             double cost = current->cost + edge->cost;
             if(!edge->finish->visited &&
                edge->finish->getColor() != YELLOW) {
                 enqueueVertex(edge->finish, current, cost);
                 pqueue.enqueue(edge->finish, cost +
                                heuristicFunction(edge->finish, end));
             }
             if(edge->finish->getColor() == YELLOW &&
                cost < edge->finish->cost) {
                 enqueueVertex(edge->finish, current, cost);
                 pqueue.changePriority(edge->finish, cost +
                                       heuristicFunction(edge->finish, end));
             }
         }
     }
     setPath(start, end, path);
     return path;
}

Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    Map<Vertex*, Set<Vertex*> > clusters;
    PriorityQueue<Edge*> pqueue;
    initializeClusters(graph, clusters);
    enqueueEdgeSet(graph, pqueue);
    while(!pqueue.isEmpty()) {
        Edge* edge = pqueue.dequeue();
        if(!clusters[edge->start].contains(edge->finish)) {
            mergeClusters(clusters, edge->start, edge->finish);
            mst.add(edge);
        }
    }
    return mst;
}
