#include <iostream>
using namespace std;

// Double Linked List Node
class doubleListNode
{
private:
    int data;
    doubleListNode* next;
    doubleListNode* prev;

public:
    doubleListNode(int n)
    {
        data = n;
        next = NULL;
        prev = NULL;
    }

    friend class List;

};

class List
{
private:
    int size;
    doubleListNode* head;
    doubleListNode* tail;

public:
    List()
    {
        size = 0;
        head = NULL;
        tail = NULL;
    }


    // Insert Function at Head
    void insert_head(int n);

};

int main (void)
{

}

void List::insert_head(int n)
{
    doubleListNode* new_node = new doubleListNode(n);
    doubleListNode* temp;

    // Prev can point to something ONLY IF adding to a non-empty list
    if (size > 0)
    {
        temp = head;
        temp->prev = new_node;
    }

    new_node->next = head;
    head = new_node;

    // After adding to EMPTY LIST, head and tail point to same node
    if (size == 0)
    {
        tail = head;
    }

    size ++;
}
