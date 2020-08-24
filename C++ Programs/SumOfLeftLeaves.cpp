//Find the sum of all left leaves in a given binary tree.


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
    int sum = 0;
    void sumOfLeft(TreeNode *root) {
        if(!root) {
            return;
        }
        if(root->left) {
            if(!root->left->left && !root->left->right) {
                
                this->sum += root->left->val;
                
            }
           sumOfLeft(root->left);
        }
        if(root->right) {
            sumOfLeft(root->right);
        }
    }
    
    int sumOfLeftLeaves(TreeNode* root) {
        sumOfLeft(root);
        return this->sum;
    }
};
