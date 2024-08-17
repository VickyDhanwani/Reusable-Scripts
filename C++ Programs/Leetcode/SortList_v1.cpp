/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    vector<int> ll_elems;
    void insertIntoVector(ListNode *head) {
        ListNode *t = head;
        while(t) {
            ll_elems.push_back(t->val);
            t = t->next;
        }
    }
    ListNode *insertNewLL() {
        ListNode *h = NULL, *t;
        
        for(int i = 0; i < ll_elems.size(); i++) {
            ListNode *node = new ListNode();
            node->val = ll_elems[i];
            node->next = NULL;
            if(i == 0) {
                h = node;
                t = node;
                continue;
            }
            t->next = node;
            t = node;

        }
        return h;
    }
    ListNode* sortList(ListNode* head) {
        if(!head || !head->next) return head;
        // Store in vector
        insertIntoVector(head);
        //sort vector
        sort(ll_elems.begin(), ll_elems.end());
        //insert in new LL
        head = insertNewLL();        
        //return New LL
        return head;
    }
};
