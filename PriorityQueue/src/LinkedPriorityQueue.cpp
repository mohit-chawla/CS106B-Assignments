// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    head = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    // TODO: implement

}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode *node = head;
    while(node->value!=value){
        node=node->next;
    }
    node->priority=newPriority;
}

void LinkedPriorityQueue::clear() {
   head=NULL;
}

string LinkedPriorityQueue::dequeue() {
    string headNodeName = head->value;
    ListNode* prevHead = head;
    head = head->next;
    //free memory to prevent memory leak
    prevHead = NULL;
    return headNodeName;
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
    return head->value;
}

int LinkedPriorityQueue::peekPriority() const {
    return head->priority;
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
        node=node->next;
    }
    out<<"}";
    return out;
}
