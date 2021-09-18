class Node {
    constructor(elem) {
        this.elem = elem;
        this.right = null;
        this.left = null;
    }
}

class BinaryTree {
    constructor() {
        this.root = null;
    }
    createBinaryTree = () => {
        let rootnode = new Node(1);
        let l1left = new Node(2);
        let l1right = new Node(3);
        let l2a = new Node(7);
        let l2b = new Node(6);
        let l2c = new Node(5);
        let l2d = new Node(4);
        rootnode.left = l1left;
        rootnode.right = l1right;
        l1left.left = l2a;
        l1left.right = l2b;
        l1right.left = l2c;
        l1right.right = l2d;
    }
}