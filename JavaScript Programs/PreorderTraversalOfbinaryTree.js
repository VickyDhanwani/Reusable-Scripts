//Given a binary tree, return the preorder traversal of its nodes' values.

/**
 * Definition for a binary tree node.
 * function TreeNode(val, left, right) {
 *     this.val = (val===undefined ? 0 : val)
 *     this.left = (left===undefined ? null : left)
 *     this.right = (right===undefined ? null : right)
 * }
 */
/**
 * @param {TreeNode} root
 * @return {number[]}
 */

var preOrder = function(root, pre) {
    if(!root || !root.val) {
        return;
    }
    pre.push(root.val);
    preOrder(root.left, pre);
    preOrder(root.right, pre);
}

var preorderTraversal = function(root) {
    if(!root) {
        return [];
    }
    let pre = []
    preOrder(root, pre);
    return pre;
};
