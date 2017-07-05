/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-5-priorityQueue-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/pqueue.html
 * date: 29 June 2017
 */

#include "HeapPriorityQueue.h"
#include "queue.h"

static string QUEUE_EMPTY_EXCEPTION = "Queue Empty";
static string ELEMENT_NOT_FOUND_EXCEPTION = "Element not found";

HeapPriorityQueue::HeapPriorityQueue() {
    arrayCapacity = 1000;
    node q[1000];
    queueSize = 1;
    queue = q;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    node* arr = (node*)malloc(queueSize*(sizeof(node)));
    free(arr);
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    bool isFound = false;
    if(isEmpty()){
        for(int i=1;i<queueSize;i++){
            if(queue[i].name==value){
                isFound = true;
                queue[i].priority=newPriority;
                break;
            }
        }
        if(!isFound)
            throw ELEMENT_NOT_FOUND_EXCEPTION;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }

}

void HeapPriorityQueue::clear() {
    queueSize = 1;
}


string HeapPriorityQueue::dequeue() {
    if(!isEmpty()){
        string toReturn = queue[1].name;
        queueSize--;
        queue[1].name = queue[queueSize].name;
        queue[1].priority = queue[queueSize].priority;
        int temp = 1;
        int child;
        if(queue[temp*2].priority==queue[temp*2+1].priority){
            child = queue[temp*2].name<queue[temp*2+1].name?temp*2:temp*2+1;
        }else{
            child = queue[temp*2].priority<queue[temp*2+1].priority?temp*2:temp*2+1;
        }
        int tempP;
        string tempVal;
        while(child < queueSize && queue[child].priority<=queue[temp].priority){
            //Check for tie and break tie appropriately
            if(queue[child].priority==queue[temp].priority && queue[child].name>queue[temp].name)
                break;
            //swap otherwise
            tempP = queue[temp].priority;
            tempVal = queue[temp].name;
            queue[temp].name = queue[child].name;
            queue[temp].priority = queue[child].priority;
            queue[child].name = tempVal;
            queue[child].priority = tempP;
            temp = child;
            if(queue[temp*2].priority==queue[temp*2+1].priority){
                child = queue[temp*2].name<queue[temp*2+1].name?temp*2:temp*2+1;
            }else{
                child = queue[temp*2].priority<queue[temp*2+1].priority?temp*2:temp*2+1;
            }
        }
        return toReturn;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    queue[queueSize].name=value;
    queue[queueSize].priority=priority;
    int parent = queueSize/2;
    int tempP;
    int temp = queueSize;
    string tempVal;
    while(parent>0 && queue[parent].priority>=priority){
        //Check for tie and break tie appropriately
        if(queue[parent].priority == priority && queue[parent].name<value)
            break;
        //swap otherwise
        tempP = queue[parent].priority;
        tempVal = queue[parent].name;
        queue[parent].priority = priority;
        queue[parent].name = value;
        queue[temp].name = tempVal;
        queue[temp].priority = tempP;
        temp = parent;
        parent = parent/2;
    }

    queueSize++;
}

bool HeapPriorityQueue::isEmpty() const {
    return queueSize == 1;   // remove this
}

string HeapPriorityQueue::peek() const {
    if(!isEmpty()){
        return queue[1].name;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

int HeapPriorityQueue::peekPriority() const {
    if(!isEmpty()){
        return queue[1].priority;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
    return queue[1].priority;
}

int HeapPriorityQueue::size() const {
    return queueSize-1;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out<<"{";
    for(int i=1;i<queue.queueSize;i++){
        out<<"\""+queue.queue[i].name+"\":"+std::to_string(queue.queue[i].priority);
        out<< (i<queue.queueSize-1?",":"");
    }
    out<<"}";
    return out;
}
