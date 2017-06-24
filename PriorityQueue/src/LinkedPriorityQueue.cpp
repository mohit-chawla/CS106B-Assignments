// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    ListNode *head = NULL;
    LinkedPriorityQueue::queue = head;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    // TODO: implement

}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    // TODO: implement

}

void LinkedPriorityQueue::clear() {
    LinkedPriorityQueue::queue = NULL;

}

string LinkedPriorityQueue::dequeue() {
    string headNodeName = LinkedPriorityQueue::queue->value;
    ListNode* prevHead = LinkedPriorityQueue::queue;
    LinkedPriorityQueue::queue = LinkedPriorityQueue::queue->next;
    //free memory to prevent memory leak
    prevHead = NULL;
    return headNodeName;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode newNode = ListNode::ListNode(value,priority,NULL);
    ListNode* node = LinkedPriorityQueue::queue;
    while(node->priority<=priority || node->next!=NULL){
        newNode.next=node->next==NULL?NULL:node->next;
        node->next = &newNode;
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    return LinkedPriorityQueue::queue ==NULL;
}

string LinkedPriorityQueue::peek() const {
    return LinkedPriorityQueue::queue->value;
}

int LinkedPriorityQueue::peekPriority() const {
    return LinkedPriorityQueue::queue->priority;
}

int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* node = LinkedPriorityQueue::queue;
    while(node!=NULL){
        size++;
        node=node->next;
    }
    return size;
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    // TODO: implement
    return out;
}
