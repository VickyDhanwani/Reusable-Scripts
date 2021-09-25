#include<bits/stdc++.h>
using namespace std;

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

        int GetSecondMaximum() {
            TreeNode *t = root;
            TreeNode *follow;
            int maxval = root->v;
            if(t->left == NULL && t->right == NULL) {
                return -1;
            }
            if(t->right == NULL) {
                follow = t->left;
                while(follow->right != NULL) {
                    follow = follow->right;
                }
                return follow->v;
            }
            else {
                follow = t;
                while(t->right != NULL) {
                    follow = t;
                    t = t->right;
                }
                if(t->left == NULL && t->right == NULL) {
                    return follow->v;
                }
                if(t->left) {
                    follow = t->left;
                    while(follow->right) {
                        follow = follow->right;
                    }
                    return follow->v;
                }
            }
            return follow->v;
        }
};


void InorderTraversal(TreeNode *t) {
    if(t == NULL) {
        return;
    }
    InorderTraversal(t->left);
    cout<<t->v<<" ";
    InorderTraversal(t->right);
}

int main() {
    BinarySearchTree bst;
    bst.insertBST(27);
    bst.insertBST(20);
    bst.insertBST(42);
    //bst.insertBST(37);
    //bst.insertBST(29);
    //bst.insertBST(41);
    bst.insertBST(17);
    InorderTraversal(bst.getRoot());
    cout<<bst.GetSecondMaximum();
}