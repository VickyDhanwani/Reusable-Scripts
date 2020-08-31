/*

Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.

An input string is valid if:

Open brackets must be closed by the same type of brackets.
Open brackets must be closed in the correct order.

*/

class Node {
    public:
    char val;
    Node *next;
};

class Stack {
    Node *top;   
    public:
        void init() {
            top = NULL;
        }
        
        void push(char ch) {
            Node *temp = new Node();
            temp->val = ch;
            temp->next = NULL;
            if(top == NULL) {
                top = temp;
                return;
            }
            temp->next = top;
            top = temp;
        }
        
        char pop() {
            char ch = top->val;
            if(top->next == NULL) {
                delete(top);
                top = NULL;
                return ch;
            }
            Node *temp = top;
            top = top->next;
            delete(temp);
            return ch;
        } 
    
        bool isEmpty() {
            if(top == NULL) {
                return true;
            }
            return false;
        }
};

class Solution {
public:
    bool isValid(string s) {
        int n = s.size();
        int i = 0;
        Stack stck;
        char ch;
        stck.init();
        int flag = false;
        for(i = 0; i < n; i++) {
            
            if(s[i] == '(' || s[i] == '[' || s[i] == '{') {
                stck.push(s[i]);
                
            }
            else {
                if(!stck.isEmpty()) {
                    ch = stck.pop();    
                }
                else {
                    flag = true;
                    break;
                }
                
                
                if(s[i] == ')' && ch == '(') {
                    
                }
                else if(s[i] == ']' && ch == '[') {
                    
                }
                else if(s[i] == '}' && ch == '{') {
                    
                }
                else {
                    flag = true;
                    break;
                }
            }
        }
        if(flag == true) {
            return false;
        }
        else {
            if(stck.isEmpty()) {
                return true;     
            }
            
        }
        return false;
    }
};
