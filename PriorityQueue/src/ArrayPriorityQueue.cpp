/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-5-priorityQueue-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/pqueue.html
 * date: 29 June 2017
 */


#include "ArrayPriorityQueue.h"

static string QUEUE_EMPTY_EXCEPTION = "Queue Empty";
static string ELEMENT_NOT_FOUND_EXCEPTION = "Element not found";

ArrayPriorityQueue::ArrayPriorityQueue() {
    pointer = 0; //points to last elements in array
    qSize = 0; //number of "eligible" elements in array
    node queue[10000];
    patients = queue;
}

ArrayPriorityQueue::~ArrayPriorityQueue() {
    node* arr = (node*)malloc(qSize*(sizeof(node)));
    free(arr);
}

void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    if(!isEmpty()){
        bool isFound = false;
        for(int i=0; i<pointer;i++){
            if(patients[i].name == value){
                patients[i].priority=newPriority;
                isFound = true;
                break;
            }
        }
        if(!isFound)
            throw ELEMENT_NOT_FOUND_EXCEPTION;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

void ArrayPriorityQueue::clear() {
    qSize=0;
    pointer=0;

}

ArrayPriorityQueue::node* ArrayPriorityQueue::getTopPriorityNode() const{
    int minPriority = 10000000;
    node* mostUrgentNode;
    for(int i=0; i<pointer;i++){
        if(patients[i].priority<minPriority && patients[i].priority!=-1){
            minPriority = patients[i].priority;
            mostUrgentNode = &patients[i];
        }
    }
    return mostUrgentNode;
}

string ArrayPriorityQueue::dequeue() {
    if(!isEmpty()){
        node* mostUrgentNode = getTopPriorityNode();
        qSize--;
        string toReturn = mostUrgentNode->name;
        mostUrgentNode->priority=-1;
        return toReturn;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

void ArrayPriorityQueue::enqueue(string value, int priority) {
    patients[pointer].name=value;
    patients[pointer].priority=priority;
    pointer++;
    qSize++;
}

bool ArrayPriorityQueue::isEmpty() const {
    return qSize==0;
}

string ArrayPriorityQueue::peek() const {
    if(!isEmpty()){
        node* mostUrgentNode = getTopPriorityNode();
        return mostUrgentNode->name;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

int ArrayPriorityQueue::peekPriority() const {
    if(!isEmpty()){
        int minPriority = 10000000;
        for(int i=0; i<pointer;i++){
            if(patients[i].priority<minPriority){
                minPriority = patients[i].priority;
            }
        }
        return minPriority;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

int ArrayPriorityQueue::size() const {
    return qSize;
}

ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    out<<"{";
    for(int i=0;i<queue.pointer;i++){
        if(queue.patients[i].priority!=-1){
            out<<"\""+queue.patients[i].name+"\":"+std::to_string(queue.patients[i].priority);
            out<< (i<queue.pointer-1?",":"");
        }
    }
    out<<"}";
    return out;
}
