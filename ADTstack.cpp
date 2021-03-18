#include <iostream>
using namespace std;
template <class T> class StackNode;

// Template for Stack Engine
template <class T>
class Stack
{
private:
    int size;
    StackNode<T>* head;

public:
    Stack()      // Engine Constructor
    {
        size = 0;
        head = NULL;
    }
    void print_values();
    void push(T x);
    void pop();
    void peek();  // Peeks at Head
};

// Template for Stack Carriages
template <class T>
class StackNode
{
private:
    int data;
    StackNode* next;

public:
    StackNode(int n)    // Carriage Constructor
    {
        data = n;
        next = NULL;
    }

    friend class Stack<T>;
};

int main (void)
{
    Stack <int> Apple;

    Apple.push(5);
    Apple.push(10);

    Apple.print_values();

    Apple.peek();

    Apple.pop();

    Apple.peek();
}


// Defining Stack - Push Function
template <class T>
void Stack <T> :: push(T x)
{
   StackNode <T>* new_node = new StackNode <T> (x);
   new_node->next = head;
   head = new_node;

   size ++;
}

// Defining Stack = Pop Function
template <class T>
void Stack <T> :: pop()
{
    if (head == NULL)       // Cant remove Head if Stack is EMPTY
    {
        assert (size == 0);
        return;
    }

    StackNode <T>* temp = head;
    head = head->next;
    delete (temp);

    size --;
}

// Defining Print Value Function
template <class T>
void Stack <T> :: print_values()
{
    StackNode <T>* temp;
    temp = head;

    while (temp != NULL)
    {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

// Defining Peek Function (at Head)
template <class T>
void Stack <T> :: peek()
{
    if (head == NULL)
    {
        cout << "Stack is Empty!" << endl;
        return;
    }

    cout << "Peeking, Value is " << head->data << endl;
}