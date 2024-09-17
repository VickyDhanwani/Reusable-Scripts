#include <bits/stdc++.h>
using namespace std;


class Node {
    public:
    int val;
    Node *left, *right;
};

class Tree {
  private:
    Node *root;
    public:
        Tree() {
            root = NULL;
        }
        void TreeInsert(int val) {
            Node *temp = new Node();
            temp->val = val;
            temp->left = temp->right = NULL;
            if(!root) {
                root = temp;
                return;
            }
            else {
                TInsert(temp);
                traverse(root);
                cout<<endl;
                
            }
        }
        void TInsert(Node *k) {
            queue<Node *> q;
            q.push(root);
            while(!q.empty()) {
                Node *parent = q.front();
                q.pop();
                if(parent->left == NULL) {
                    parent->left = k;
                    return;
                    
                }
                else if(parent->right == NULL) {
                    parent->right = k;
                    return;
                    
                }
                else {
                    q.push(parent->left);
                    q.push(parent->right);
                }
            }
            
        }
        void BSTInsert(int k) {
            Node *temp = new Node();
            temp->val = k;
            temp->left = temp->right = NULL;
            if(!root) {
                root = temp;
                return;
            }
            Node *prev, *p = root;
            int dir = 1; // 1 = l, 2 = r
            prev = p;
            while(p) {
                if(k < p->val) {
                    dir = 1;
                    prev = p;
                    p = p->left;
                }
                else {
                    dir = 2;
                    prev = p;
                    p = p->right;
                }
            }
            if(dir == 1) prev->left = temp;
            else prev->right = temp;
            traverse(root);
        }
        void deletebst(int k) {
            return deleteNode(root, root, k, 1);
        }
        void deleteNode(Node *p, Node *r, int k, int dir) {
            if(!r) return;
            if(k < r->val) {
                deleteNode(r, r->left, k, 1);
            }
            else if(k > r->val) {
                deleteNode(r, r->right, k, 2);
            }
            else {
                if(r->left) {
                    Node *lc = r->left;
                    Node *p_lc = r->left;
                    while(lc->right) {
                        p_lc = lc; 
                        lc = lc->right;
                    }
                    r->val = lc->val;
                    if(lc != p_lc)
                            p_lc->right = NULL;
                    else r->left = lc->left;
                }
                else if(r->right) {
                    Node *rc= r->right;
                    Node *p_rc = r->right;
                    while(rc->left) {
                        p_rc = rc;
                        rc = rc->left;
                        
                    }
                    r->val = rc->val;
                    if(rc != p_rc) p_rc->left = NULL;
                    else r->right = rc->right;
                }
                else {
                    if(p == r) {
                        r = NULL;
                    }
                    if(dir == 1) {
                        p->left = NULL;
                    }
                    else {
                        p->right = NULL;
                    }
                }
            }
        }
        
        void traverse(Node *t) {
            if(!t) return;
            queue<Node *> q;
            q.push(t);
            while(!q.empty()) {
                Node *p = q.front();
                cout<<p->val<<" ";
                q.pop();
                if(p->left) q.push(p->left);
                if(p->right) q.push(p->right);
                
            }
            cout<<endl;
        }
        Node *getroot() {
            return root;
        }
};

int main() {
	// your code goes here
	Tree tr;
// 	tr.TreeInsert(1);
// 	tr.TreeInsert(2);
// 	tr.TreeInsert(3);
// 	tr.TreeInsert(4);
// 	tr.TreeInsert(5);
// 	tr.TreeInsert(6);
// 	tr.TreeInsert(7);
// 	tr.TreeInsert(8);
// 	tr.TreeInsert(9);
	
	tr.BSTInsert(33);
	tr.BSTInsert(29);
	tr.BSTInsert(25);
	tr.BSTInsert(31);
	tr.BSTInsert(38);
    tr.BSTInsert(19);
	tr.BSTInsert(40);
	tr.deletebst(29);
	
    tr.traverse(tr.getroot());
    
    tr.deletebst(25);
	
    tr.traverse(tr.getroot());
}
