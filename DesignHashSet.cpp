/*Design a HashSet without using any built-in hash table libraries.

To be specific, your design should include these functions:

add(value): Insert a value into the HashSet. 
contains(value) : Return whether the value exists in the HashSet or not.
remove(value): Remove a value in the HashSet. If the value does not exist in the HashSet, do nothing.
*/

class MyHashSet {
public:
    /** Initialize your data structure here. */
    int hash[1000000];
    MyHashSet() {
        for(int i = 0; i < 100000; i++) {
            hash[i] = 0;
        }
    }
    
    void add(int key) {
        hash[key] = 1;
    }
    
    void remove(int key) {
        hash[key] = 0;
    }
    
    /** Returns true if this set contains the specified element */
    bool contains(int key) {
        if(hash[key] == 1){
            return true;
        }
        return false;
    }
};
