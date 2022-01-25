#include<bits/stdc++.h>
using namespace std;

class Node {
    public:
        int val;
        Node *next;
        Node() {
            next = NULL;
        }
};

class LinkedList {
    private :
        Node *head, *tail;
        int length;
    public :
        LinkedList() {
            head = NULL;
            tail = NULL;
            length = 0;
        }

        void llinsert(int k) {
            Node *temp = new Node();
            temp->val = k;
            temp->next = NULL;
            if(head == NULL) {
                head = temp;
                tail = temp;
                length = 1;
                return;
            }
            tail->next = temp;
            tail = temp;
            length += 1;
        }

        int is_empty() {
            if(head == NULL) {
                return 1;
            }
            return 0;
        }

        Node *getHead() {
            return head;
        }

        void Printlist() {
            Node *temp = head;
            while(temp) {
                cout<<temp->val<<" ";
                temp = temp->next;
            }
            cout<<endl;
        }

        void DeleteList() {
            head = NULL;
            tail = NULL;
        }

};

void MergeLinkedLists(vector<Node *> arr, int size) {
    LinkedList c;
    Node *nexthead;
    Node *head = arr[0];
    if(size == 1) {
        while(head) {
            cout<<head->val<<" ";
            head = head->next;
        }
        cout<<endl;
    }
    else {
        int i = 1;
        while(i < size) {
            nexthead = arr[i];
            head = arr[0];
            while(head && nexthead) {
                //cout<<"Comparing "<<head->val<<" with "<<nexthead->val<<endl;
                if(head->val < nexthead->val) {
                    c.llinsert(head->val);
                    head = head->next;
                }
                else if(head->val > nexthead->val) {
                    c.llinsert(nexthead->val);
                    nexthead = nexthead->next;
                }
                else if(head->val == nexthead->val) {
                    c.llinsert(nexthead->val);
                    c.llinsert(nexthead->val);
                    head = head->next;
                    nexthead = nexthead->next;
                }
                //c.Printlist();
            }
            while(head) {
                c.llinsert(head->val);
                head = head->next;
            }
            while(nexthead) {
                c.llinsert(nexthead->val);
                nexthead = nexthead->next;
            }
            if(i == size - 1) {
                c.Printlist();
            }
            //c.Printlist();
            arr[0] = c.getHead();
            c.DeleteList();
            i++;
        }
    }
    
}

int main() {
    LinkedList ll;
    vector<Node *> arr;
    ll.llinsert(5);
    ll.llinsert(8);
    ll.llinsert(9);
    arr.push_back(ll.getHead());
    ll.Printlist();
    ll.DeleteList();
    ll.llinsert(4);
    ll.llinsert(7);
    arr.push_back(ll.getHead());
    ll.Printlist();
    ll.DeleteList();
    ll.llinsert(1);
    ll.llinsert(4);
    ll.llinsert(10);
    arr.push_back(ll.getHead());
    ll.Printlist();
    ll.DeleteList();
    cout<<arr.size()<<endl;
    MergeLinkedLists(arr, arr.size());
    return 0;
}