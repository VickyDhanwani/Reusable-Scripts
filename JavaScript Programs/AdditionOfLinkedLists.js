class Node {
    constructor(digit) {
        this.digit = digit;
        this.next = null;
    }
};

class LinkedList {
    constructor() {
        this.head = null;
        this.tail = null;
    }
    insertElement = (k) => {
        let temp = new Node(k);
        if(this.head === null) {
            this.head = temp;
            this.tail = temp;
            return;
        }
        this.tail.next = temp;
        this.tail = temp;
    }
    printList = () => {
        let temp = this.head;
        while(temp !== null) {
            console.log(temp.digit);
            temp = temp.next;
        }
    }
}

class Stack {
    constructor() {
        this.top = null;
    }
    Spush = (elem) => {
        let temp = new Node(elem);
        if(this.top === null) {
            this.top = temp;
            return;
        }
        temp.next = this.top;
        this.top = temp;
    }
    Spop = () => {
        let elem = this.top.digit;
        if(this.top.next === null) {
            this.top = null;
            return elem;
        }
        this.top = this.top.next;
        return elem;
    }
    isEmpty = () => {
        if(this.top === null) {
            return true;
        }
        return false;
    }
}

firstNum = new Stack();
secondNum = new Stack();

let i = 0;
for(i = 0; i < 10; i++) {
    firstNum.Spush(Math.floor(Math.random() * 10));
}

for(i = 0; i < 10; i++) {
    secondNum.Spush(Math.floor(Math.random() * 10));
}

