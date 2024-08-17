class Node {
    public:
    int key, val;
    class Node *next;
};

class LinkedList {
    private:
       Node *head, *tail;
    public:
        LinkedList() {
            head= NULL;
            tail = NULL;
        }
        void Insert(int key, int val) {
            Node *temp = new Node();
            temp->val = val;
            temp->key = key;
            temp->next = NULL;
            if(!head) {
                head = temp;
                tail = temp;
                return;
            }
            tail->next = temp;
            tail = temp;
        }
        void putValue(int key, int val) {
            Node *t = head;
            while(t) {
                if(t->key == key) {
                    t->val = val;
                    return;
                }
                t = t->next;
            }
            Insert(key, val);
        }
        int getValue(int key) {
            Node *t = head;
            while(t) {
                if(t->key == key) return t->val;
                t = t->next;
            }
            return -1;
        }
        void removeKey(int key) {
        if (!head) return;

        // If the head needs to be removed
        if (head->key == key) {
            Node *toDelete = head;
            head = head->next;
            delete toDelete;
            // If the list had only one node
            if (!head) {
                tail = NULL;
            }
            return;
        }

        Node *prev = head;
        Node *curr = head->next;

        while (curr) {
            if (curr->key == key) {
                prev->next = curr->next;
                if (curr == tail) {
                    tail = prev;
                }
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

};

class MyHashMap {
public:
    LinkedList ll;
    MyHashMap() {
        
    }
    
    void put(int key, int value) {
        ll.putValue(key, value);
    }
    
    int get(int key) {
        
        return ll.getValue(key);
    }
    
    void remove(int key) {
        ll.removeKey(key);
    }
};

/**
 * Your MyHashMap object will be instantiated and called as such:
 * MyHashMap* obj = new MyHashMap();
 * obj->put(key,value);
 * int param_2 = obj->get(key);
 * obj->remove(key);
 */
