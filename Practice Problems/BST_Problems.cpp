#include<bits/stdc++.h>
using namespace std;

class Node {
    public:
        int val;
        Node *left, *right;
        Node(int k) {
            val = k;
            left = NULL;
            right = NULL;
        }
};

class BST {
    private:
        Node *root;
    public:
        BST() {
            root = NULL;
        }
        int is_empty() {
            if(root == NULL) {
                return 1;
            }
            return 0;
        }
        void bst_insert(int k) {
            int dir = 0; // 0 = left, 1 = right;
            Node *x = root;
            Node *y = x;
            Node *temp = new Node(k);
            if(x == NULL) {
                root = temp;
                return;
            }
            while(x != NULL) {
                if(k > x->val) {
                    dir = 1;
                    y = x;
                    x = x->right;
                }
                else if(k < x->val) {
                    dir = 0;
                    y = x;
                    x = x->left;
                }
            }
            if(dir == 1) {
                y->right = temp;
            }
            else {
                y->left = temp;
            }
        }

        Node *getRoot() {
            return root;
        }

        int bst_search(int k) {
            Node *x = root;
            while(x != NULL || x->val != k) {
                if(x->val == k) {
                    return 1;
                }
                if(k > x->val) {
                    x = x->right;
                }
                else {
                    x = x->left;
                }
            }
            return 0;
        }
};

void inorder_traversal(Node *x) {
    if(x == NULL) 
        return;
    inorder_traversal(x->left);
    cout<<x->val<<" ";
    inorder_traversal(x->right);
}

int main() {
    BST b;
    //cout<<b.is_empty();
    b.bst_insert(25);
    b.bst_insert(10);
    b.bst_insert(32);
    b.bst_insert(14);
    b.bst_insert(7);
    b.bst_insert(30);
    //cout<<b.is_empty();
    inorder_traversal(b.getRoot());    
    return 0;
} 