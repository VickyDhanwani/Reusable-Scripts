/*
You are given a binary tree in which each node contains an integer value.

Find the number of paths that sum to a given value.

The path does not need to start or end at the root or a leaf, but it must go downwards (traveling only from parent nodes to child nodes).

The tree has no more than 1,000 nodes and the values are in the range -1,000,000 to 1,000,000.
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
    int getPaths(TreeNode *root, int sum, int curr) {
        if(root == NULL) {
            return 0;
        }
        if(curr + root->val == sum) {
            return 1 + getPaths(root->left, sum,curr + root->val) + getPaths(root->right,sum, curr + root->val);
        }
        
        
            return getPaths(root->left, sum, curr + root->val) + getPaths(root->right, sum, curr + root->val);
        
    }
    
    int pathSum(TreeNode* root, int sum) {
       if(!root) {
           return 0;
       }
        return getPaths(root, sum, 0) + pathSum(root->left, sum) + pathSum(root->right, sum);
    }
};
