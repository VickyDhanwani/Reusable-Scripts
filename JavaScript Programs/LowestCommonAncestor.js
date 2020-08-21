/**
 * Definition for a binary tree node.
 * function TreeNode(val) {
 *     this.val = val;
 *     this.left = this.right = null;
 * }
 */

/**
 * @param {TreeNode} root
 * @param {TreeNode} p
 * @param {TreeNode} q
 * @return {TreeNode}
 */
var lowestCommonAncestor = function(root, p, q) {
    if(!root || !p || !q) {
        return root
    }
    var t;
    if(p.val > q.val) {
        t = p;
        p = q;
        q = t;
    }
    t = root;
    while(t) {
        if(t.val > p.val && t.val > q.val) {
            t = t.left;
        }
        else if(t.val < p.val && t.val < q.val) {
            t = t.right;
        }
        else if(t.val >= p.val && t.val <= q.val) {
                break;
        }
    }
    return t;
};
