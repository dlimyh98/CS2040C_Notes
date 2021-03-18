#include <iostream>

using namespace std;

// Single Linked List Node
class ListNode
{
private:
    int data;
    ListNode *next;

public:
    ListNode(int n)
    {
        data = n;
        next = NULL;
    }

    friend class List;
};

// List
class List
{
private:
    int size;    // Size of Linked List
    ListNode *head;
    ListNode *tail;

public:
    List()     // Initializing Size, Head Pointer, Tail Pointer for List
    {
        size = 0;
        head = NULL;
        tail = NULL;
    }

    // Get Function for Node at Index n
    ListNode* get_node(int n);

    // Print Function for Linked List Values from Start to End
    void printing_values (void);

    // Print Function for Linked List size
    void printing_size(void);

    // Print Function for Tail Value of Linked List
    void printing_tail(void);

    // Insert Function for Node at Head
    void insert_head (int x);

    // Insert Function for Node at Index with Data N
    void insert_body(int index, int n);

    // Insert Function for Node beyond Tail with Data N
    void insert_beyond_tail(int n);

    // Remove Function for Node at Head
    void remove_head();

    // Remove Function for Node at Index N
    void remove_body(int n);

    // Remove Function for Node at Tail
    void remove_tail();
};

int main (void)
{
    List chain;
}

void List::remove_tail()
{
    ListNode* pre = get_node(size-2);    // Pre points to Node JUST BEFORE tail

    pre->next = NULL;
    delete(tail);
    tail = pre;
}

void List::remove_body(int n)
{
    ListNode* pre = head;               // Point pre to start of Linked List
    for (long i = 0; i < n-1; i += 1)
    {
        pre = pre->next;
    }

    ListNode* node_remove = pre->next;
    ListNode* post = (node_remove)->next;

    pre->next = post;
    delete(node_remove);

    size--;
}

void List::remove_head()
{
    if (head == NULL)       // If size of Linked List is 0
    {
        assert (size == 0);
        return;
    }

    ListNode *temp = head;
    head = head->next;
    delete (temp);

    size--;
}

void List::insert_beyond_tail(int n)
{
    ListNode *new_node = new ListNode(n);

    ListNode *temp = tail;          // Temp pointing to ORIGINAL TAIL of Linked List
    new_node->next = temp->next;    // New Node pointing to next of ORIGINAL TAIL
    temp->next = new_node;          // next of ORIGINAL TAIL now pointing to New Node
    tail = temp->next;              // Tail now switched to point to NEW TAIL

    size += 1;
}

void List::insert_body(int index, int n)
{
    // Traversing to Pre-Index of Insertion
    ListNode *pre = head;
    for (int i = 0; i < index - 1; i += 1)
    {
       pre = pre->next;
    }

    // Traversing to Post-Index of Insertion
    ListNode *post = pre->next;

    // Allocating memory for New Node
    ListNode *new_node = new ListNode(n);

    pre->next = new_node;    // Set Pre-Index to point to New Node
    new_node->next = post;   // Set New Node to point to Post-Index

    if (index == size)
    {
        tail = post;
    }

    size += 1;
}

void List::insert_head (int x)
{
    ListNode *new_node = new ListNode (x);
    new_node->next = head;
    head = new_node;

    if (size == 0)
    {
        tail = head;
    }

    size += 1;
}

void List::printing_values (void)
{
    ListNode *temp;
    temp = head;

    while (temp != NULL)
    {
        cout << temp->data << " ";
        temp = temp->next;
    }
}

void List::printing_size(void)
{
    cout << "Size of List is " << size << endl;
}

void List::printing_tail(void)
{
    int tail_value = tail->data;
    cout << "Value of Tail is " << tail_value << endl;
}

ListNode* List::get_node(int n)
{
    ListNode *traversal = head;   // Point Traversal to start of Linked List

    for (long i = 0; i < n; i += 1)
    {
        traversal = traversal->next;
    }

    return traversal;
}
