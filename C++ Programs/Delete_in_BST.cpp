/* 
Given a root node reference of a BST and a key, delete the node with the given key in the BST. Return the root node reference (possibly updated) of the BST.
*/

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        TreeNode *x = root, *y, *searchNode, *z;
        int dir = 0;
        if(!root) {
            return root;
        }
        if(root->val == key) {
            if(!root->left && !root->right) {
                return NULL;
            }
        }
        x = root;
        y = x;
        while(x) {
            if(x->val == key) {
                break;
            }
            else if(x->val > key) {
                y = x;
                dir = 1;
                x = x->left;
            }
            else if(x->val < key) {
                y = x;
                dir = 2;
                x = x->right;
            }
        }
        if(!x) {
            return root;
        }
        searchNode = x;
        //cout<<"Node  is "<<x->val<<" With Parent as "<<y->val;
        //Search Node has no children
        if(!searchNode->right && !searchNode->left) {
            if(dir == 1) {
                y->left = NULL;
            }
            else if(dir == 2) {
                y->right = NULL;
            }
        }
        //End of No Chlidren Node deletion
        
        //For Only Right Children 
        
        if(searchNode->right && !searchNode->left) {
            if(dir == 0) {
                root = searchNode->right;
            }
            else if(dir == 1) {
                y->left = searchNode->right;
            }
            else if(dir == 2) {
                y->right = searchNode->right;
            }
        }
        
        //End of Right Children
        
        //For Only LEFT child
        if(!searchNode->right && searchNode->left) {
            if(dir == 0) {
                root = searchNode->left;
            }
            else if(dir == 1) {
                y->left = searchNode->left;
            }
            else if(dir == 2) {
                y->right = searchNode->right;
            }
        }
        //End of only left child
        
        //For Node Having Both Children
        
        if(searchNode->left && searchNode->right) {
            x = searchNode->right;
            if(!x->right && !x->left) {
                searchNode->val = x->val;
                searchNode->right = NULL;
            }
            else if(x->right && !x->left) {
                searchNode->val = x->val;
                searchNode->right = x->right;
            }
            else {
                y = x;
                while(x->left) {
                    y = x;
                    x = x->left;
                }
                y->left = NULL;
                searchNode->val = x->val;
            }
        }
        
        return root;
    }
};
