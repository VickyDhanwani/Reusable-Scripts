/*
Given two binary search trees root1 and root2.

Return a list containing all the integers from both trees sorted in ascending order.

Input: root1 = [2,1,4], root2 = [1,0,3]
Output: [0,1,1,2,3,4]
Example 2:

Input: root1 = [0,-10,10], root2 = [5,1,7,0,2]
Output: [-10,0,0,1,2,5,7,10]
Example 3:

Input: root1 = [], root2 = [5,1,7,0,2]
Output: [0,1,2,5,7]
Example 4:

Input: root1 = [0,-10,10], root2 = []
Output: [-10,0,10]

Input: root1 = [1,null,8], root2 = [8,1]
Output: [1,1,8,8]
 

Constraints:

Each tree has at most 5000 nodes.
Each node's value is between [-10^5, 10^5].

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
    vector<int> leftArray;
    vector<int> rightArray;
    
    void Inorder(TreeNode *root) {
        if(!root) {
            return;
        }
        Inorder(root->left);
        leftArray.push_back(root->val);
        Inorder(root->right);
    }
    
    void RInorder(TreeNode *root) {
        if(!root) {
            return;
        }
        RInorder(root->left);
        rightArray.push_back(root->val);
        RInorder(root->right);
    }
    
    vector<int> getAllElements(TreeNode* root1, TreeNode* root2) {
        vector<int> SortedArray;
        Inorder(root1);
        RInorder(root2);
        int n1 = leftArray.size();
        int n2 = rightArray.size();
        int i = 0, j = 0, k;
        while(i < n1 && j < n2) {
            if(leftArray[i] < rightArray[j]) {
                SortedArray.push_back(leftArray[i]);
                i++;
            }
            else if(leftArray[i] > rightArray[j]) {
                SortedArray.push_back(rightArray[j]);
                j++;
            }
            else if(leftArray[i] == rightArray[j]) {
                SortedArray.push_back(leftArray[i]);
                SortedArray.push_back(rightArray[j]);
                i++;
                j++;
            }
        }
        while(i < n1) {
            SortedArray.push_back(leftArray[i]);
            i++;
        }
        while(j < n2) {
            SortedArray.push_back(rightArray[j]);
            j++;
        }
        return SortedArray;
    }
};
