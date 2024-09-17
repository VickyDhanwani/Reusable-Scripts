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
    void deleteNode(TreeNode*& p, TreeNode*& r, int k, int dir) {
        if (!r) return;
        if (k < r->val) {
            deleteNode(r, r->left, k, 1);
        } else if (k > r->val) {
            deleteNode(r, r->right, k, 2);
        } else {
            if (r->left) {
                TreeNode* lc = r->left;
                TreeNode* p_lc = r->left;
                while (lc->right) {
                    p_lc = lc;
                    lc = lc->right;
                }
                r->val = lc->val;
                if (lc != p_lc)
                    p_lc->right = lc->left;
                else
                    r->left = lc->left;
                delete lc;
            } else if (r->right) {
                TreeNode* rc = r->right;
                TreeNode* p_rc = r->right;
                while (rc->left) {
                    p_rc = rc;
                    rc = rc->left;
                }
                r->val = rc->val;
                if (rc != p_rc)
                    p_rc->left = rc->right;
                else
                    r->right = rc->right;
                delete rc;
            } else {
                if (p == r) {
                    r = nullptr;
                } else if (dir == 1) {
                    p->left = nullptr;
                } else {
                    p->right = nullptr;
                }
                delete r;
            }
        }
    }

    TreeNode* deleteNode(TreeNode* root, int key) {
        deleteNode(root, root, key, 1);
        return root;
    }
};
