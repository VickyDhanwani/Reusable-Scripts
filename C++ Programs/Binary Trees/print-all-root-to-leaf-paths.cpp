#include<bits/stdc++.h>
using namespace std;

class StackNode {
    public:
    int val;
    StackNode *next;
};

class TreeNode {
    public :
    int v;
    TreeNode *left, *right;
};

class BinarySearchTree {
    private :
        TreeNode *root;
    public :
        BinarySearchTree() {
            root = NULL;
        }
        void insertBST(int k) {
            TreeNode *temp = new TreeNode;
            TreeNode *ptr, *follower;
            temp->v = k;
            temp->left = NULL;
            temp->right = NULL;
            int dir = 0; //0 = left, 1 = right
            if(root == NULL) {
                root = temp;
                return;
            }
            ptr = root;
            while(ptr) {
                if(k > ptr->v) {
                    dir = 1;
                    follower = ptr;
                    ptr = ptr->right;
                }
                else if(k < ptr->v) {
                    dir = 0;
                    follower = ptr;
                    ptr = ptr->left;
                }
            }
            if(dir == 0) {
                follower->left = temp;
            }
            else if(dir == 1) {
                follower->right = temp;
            }
        }
        TreeNode *getRoot() {
            return root;
        }
};

class Stack {
    StackNode *top;
    public:
        Stack() {
            top = NULL;
        }
        void Sinit() {
            top = NULL;
        }
        void push(int k) {
            StackNode *temp = new StackNode;
            temp->val = k;
            temp->next = NULL;
            if(top == NULL) {
                top = temp;
                return;
            }
            temp->next = top;
            top = temp;
        }
        int pop() {
            int k = top->val;
            StackNode *temp = top;
            if(top->next == NULL) {
                delete(top);
                top = NULL;
                return k;
            }
            temp = top;
            delete(temp);
            top = top->next;
            return k;
        }
        bool isEmpty() {
            if(top == NULL) {
                return true;
            }
            return false;
        }
        void printStack() {
            StackNode *temp = top;
            while(temp != NULL) {
                cout<<temp->val<<" ";
                temp = temp->next;
            }
        }
};

void InorderTraversal(TreeNode *t) {
    static Stack stk;
    Stack temp_stck;
    if(t == NULL) {
        return;
    }
    stk.push(t->v);
    //while(stk.isEmpty() != false) {
        if(t->left == NULL && t->right == NULL) {
            temp_stck.Sinit();
            while(stk.isEmpty() == false) {
                temp_stck.push(stk.pop());
            } 
            temp_stck.printStack();
            cout<<endl;
            while(temp_stck.isEmpty() == false) {
                stk.push(temp_stck.pop());
            }
        }
        if(t->left != NULL) {
            InorderTraversal(t->left);
        }   
        if(t->right != NULL) {
            InorderTraversal(t->right);
        } 
        stk.pop();
    //}
}

int main() {
    BinarySearchTree bst;
    bst.insertBST(6);
    bst.insertBST(2);
    bst.insertBST(15);
    bst.insertBST(1);
    bst.insertBST(24);
    bst.insertBST(10);
    InorderTraversal(bst.getRoot());
    
    return 0;
}