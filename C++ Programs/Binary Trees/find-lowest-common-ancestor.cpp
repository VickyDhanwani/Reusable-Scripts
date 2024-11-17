/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    bool checkIfExists(TreeNode *root, TreeNode *p) {
        if(!root) return false;
        if(root == p) return true;
        return checkIfExists(root->left, p) || checkIfExists(root->right, p); 
    }
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(root == p || root == q) {
            return root;   
        }
        else if(checkIfExists(root->left, p) && checkIfExists(root->left, q)) {
            return lowestCommonAncestor(root->left, p, q);
        }
        else if(checkIfExists(root->right, p) && checkIfExists(root->right, q)) {
            return lowestCommonAncestor(root->right, p, q);
        }
        
            return root;
        
    }
};
