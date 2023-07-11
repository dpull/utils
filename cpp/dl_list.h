/****************************************************************************
Copyright (c) 2013      dpull.com
http://www.dpull.com
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once
#include <cassert>

#ifndef FIELD_OFFSET
// long
// FIELD_OFFSET(
//     IN <typename>   type,
//     IN <fieldname>  field
//     );
#define FIELD_OFFSET(type, field) ((long)&(((type*)0)->field))
#endif

#ifndef CONTAINING_RECORD
// <typename> FAR *
// CONTAINING_RECORD(
//     IN PVOID       address,
//     IN <typename>  type,
//     IN <fieldname> field
//     );
#define CONTAINING_RECORD(address, type, field) ((type*)((char*)(address) - (char*)(&((type*)0)->field)))

#endif

struct dl_list_node {
    union {
        dl_list_node* next = nullptr;
        dl_list_node* front;
    };
    union {
        dl_list_node* prev = nullptr;
        dl_list_node* back;
    };
};

inline int dl_list_init(dl_list_node* head_list)
{
    assert(!head_list->next);
    assert(!head_list->prev);
    head_list->next = head_list->prev = head_list;
    return true;
}

inline int dl_list_empty(dl_list_node* head_list)
{
    return head_list->next == head_list;
}

inline int dl_list_push_front(dl_list_node* head_list, dl_list_node* node)
{
    assert(!node->next);
    assert(!node->prev);

    dl_list_node* list_head = head_list;
    dl_list_node* front = list_head->front;
    node->front = front;
    node->back = list_head;
    front->back = node;
    list_head->front = node;
    return true;
}

inline int dl_list_push_back(dl_list_node* head_list, dl_list_node* node)
{
    assert(!node->next);
    assert(!node->prev);

    dl_list_node* list_head = head_list;
    dl_list_node* back = list_head->back;
    node->front = list_head;
    node->back = back;
    back->front = node;
    list_head->back = node;
    return true;
}

inline int dl_list_erase(dl_list_node* node)
{
    dl_list_node* next = node->next;
    dl_list_node* prev = node->prev;
    prev->next = next;
    next->prev = prev;

    node->next = nullptr;
    node->prev = nullptr;
    return true;
}

inline dl_list_node* dl_list_pop_front(dl_list_node* head_list)
{
    auto node = head_list->front;
    dl_list_erase(node);
    return node;
}

inline dl_list_node* dl_list_pop_back(dl_list_node* head_list)
{
    auto node = head_list->back;
    dl_list_erase(node);
    return node;
}

inline int dl_list_swap(dl_list_node* head_list1, dl_list_node* head_list2)
{
    dl_list_node* next1 = head_list1->next;
    dl_list_node* prev1 = head_list1->prev;
    dl_list_node* next2 = head_list2->next;
    dl_list_node* prev2 = head_list2->prev;

    if (next2 != head_list2) { // is not empty
        head_list1->next = next2;
        head_list1->prev = prev2;

        next2->prev = head_list1;
        prev2->next = head_list1;
    } else { // if empty
        dl_list_init(head_list1);
    }

    if (next1 != head_list1) { // is not empty
        head_list2->next = next1;
        head_list2->prev = prev1;

        next1->prev = head_list2;
        prev1->next = head_list2;
    } else { // if empty
        dl_list_init(head_list2);
    }

    return true;
}
