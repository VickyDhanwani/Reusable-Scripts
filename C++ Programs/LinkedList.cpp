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
   
    return 0;

}
