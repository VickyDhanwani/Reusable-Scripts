"""
Given a singly linked list L: L0→L1→…→Ln-1→Ln,
reorder it to: L0→Ln→L1→Ln-1→L2→Ln-2→…

You may not modify the values in the list's nodes, only nodes itself may be changed.
"""

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
    void reorderList(ListNode* head) {
        if(!head) {
            return;
        }
        if(head->next == NULL) {
            return;
        }
        ListNode *x = head, *y, *z, *w;
        w = head->next;
        if(w->next == NULL) {
            return;
        }
        //y = head;
        while(x->next != NULL) {
            y = w;
            z = y;
            while(y->next != NULL) {
                z = y;
                y = y->next;
            }
            x->next = y;
            y->next = w;
            z->next = NULL;
            x = w;
            w = w->next;
        }
       
    }
};
