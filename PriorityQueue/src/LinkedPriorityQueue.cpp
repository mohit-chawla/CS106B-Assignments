/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program decription:CS106b-assignment-5-priorityQueue-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/pqueue.html
 * date: 29 June 2017
 */

#include "LinkedPriorityQueue.h"

static string QUEUE_EMPTY_EXCEPTION = "Queue Empty";
static string ELEMENT_NOT_FOUND_EXCEPTION = "Element not found";

LinkedPriorityQueue::LinkedPriorityQueue() {
    head = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    while (head != NULL) {
           ListNode *next = head->next;
           delete head;
           head = next;
       }
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode *node = head;
    if(!isEmpty()){
        while(node->value!=value && node!=NULL){
            node=node->next;
        }
        node->priority=newPriority;
        if(node==NULL)
            throw ELEMENT_NOT_FOUND_EXCEPTION;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

void LinkedPriorityQueue::clear() {
   head=NULL;
}

string LinkedPriorityQueue::dequeue() {
    if(!isEmpty()){
        string headNodeName = head->value;
        ListNode* prevHead = head;
        head = head->next;
        //free memory to prevent memory leak
        prevHead = NULL;
        return headNodeName;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* newNode = new ListNode(value,priority);

    if(head==NULL){
        head=newNode;
    }else{
        ListNode* node = head;
        ListNode*prev;
        while(node->priority<priority || (node->value<value && node->priority==priority)){
            prev=node;
            node=node->next;
            if(node==NULL)
                break;
        }
        if(node==head){
            head=newNode;
            newNode->next=node;
        }else{
            prev->next=newNode;
            newNode->next=node;
        }
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    return head ==NULL;
}

string LinkedPriorityQueue::peek() const {
    if(!isEmpty()){
        return head->value;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

int LinkedPriorityQueue::peekPriority() const {
    if(!isEmpty()){
        return head->priority;
    }else{
        throw QUEUE_EMPTY_EXCEPTION;
    }
}

int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* node = head;
    while(node!=NULL){
        size++;
        node=node->next;
    }
    return size;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    ListNode *node =queue.head;
    out<<"{";
    while(node!=NULL){
        out<<"\""+node->value+"\":"+std::to_string(node->priority);
        if(node->next)
            out<<",";
        node=node->next;
    }
    out<<"}";
    return out;
}
