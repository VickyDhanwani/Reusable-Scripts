#include <bits/stdc++.h>

using namespace std;

class Node {
    public:
        int val;
        class Node *next;
};

class LinkedList {
    private:
        Node *head;
        
    public:
        
        //Initialization
        LinkedList() {
            head = NULL;
        }
        
        //How to add an element in LL 
        
        void AddElement(int k) {
            Node *temp = new Node();
            temp->val = k;
            temp->next = NULL;
            if(!head) {
                head = temp;
                return;
            }
            Node *traverse = head;
            while(traverse->next) {
                traverse = traverse->next;
            }
            traverse->next = temp;
            
        }
        
        void PrintList() {
            Node *temp = head;
            while(temp) {
                cout<<temp->val<<" ";
                temp = temp->next;
            }
            cout<<endl;
        }
        
        int getLength() {
            Node *t = head;
            int len = 0;
            while(t) {
                len++;
                t = t->next;
            }
            return len;
        }
        
        Node *Reverse() {
            // 1-> 2 -> 3 -> 4 
            Node *current = head->next;
            Node *prev = head;
            Node *future;
            prev->next = NULL;
            while(current) {
                future = current->next;
                current->next = prev;
                prev= current;
                current = future;
            }
            return prev;
        }
        void PrintReverse() {
            head = Reverse(); // 
            PrintList(); //
            head = Reverse(); 
            
        }
        void InsertAt(int pos, int val) {
            if(pos - 1 > getLength()) return;
            Node *temp = head;
            Node *new_node = new Node();
            new_node->val = val;
            new_node->next = NULL;
            if(pos == 1) {
                if(head == NULL) {
                    head = new_node;
                    return;
                }
                new_node->next = head;
                head = new_node;
                return;
            }
            while(pos > 2) {
                temp = temp->next;
                pos--;
            }
            new_node->next = temp->next;
            temp->next = new_node;
        }
        
        void pop() {
            if(head == NULL) return;
            if(head->next == NULL) {
                head = NULL;
                return;
            }
            Node *t = head;
            while(t->next->next) {
                t = t->next;
            }
            t->next = NULL;
        }
        void RemoveAt(int pos) {
            if(pos > getLength() || pos < 1) return;
            if(pos == 1) {
                head = head->next;
                return;
            }
            Node *t = head;
            while(pos > 2) {
                t = t->next;
                pos--;
            }
            if(t->next->next) {
                t->next = t->next->next;
                return;
            }
            t->next = NULL;
        }
};

int main() {
    // your code goes here
    LinkedList ll;
    ll.AddElement(5);
    ll.AddElement(10);
    ll.PrintReverse();
    ll.PrintList();
    cout<<ll.getLength()<<endl;
    ll.AddElement(15);
    ll.AddElement(20);
    ll.PrintList();
    cout<<ll.getLength()<<endl;
    ll.PrintReverse();
    ll.PrintList();
    ll.InsertAt(5, 7);
    ll.pop();
    ll.PrintList(); // 5 10 15 20
    ll.RemoveAt(1); // 10 ,15, 20
    ll.RemoveAt(3); // 10 15
    ll.RemoveAt(1); // 
    ll.PrintList();
    return 0;

}
