#include <iostream>
#include <algorithm>
using namespace std;

template <class T>
class List;

template <class T>
class ListNode
{
private:
    T _item;
    ListNode<T> *_next;

public:
    ListNode(T n) {_item = n; _next = nullptr;}
    T content() { return _item; };
    void print() { std::cout << _item; };
    friend class List<T>;
};

template <class T>
class List
{
private:
    int _size;
    ListNode<T> *_head;

public:
    // for the following functions, you cannot assume that the list is not empty or empty...
    List()	{	_size = 0;	_head = NULL;};
    ~List() {while (_head) removeHead();};

    int size() { return _size; };
    bool exist(T);
    bool empty() {return _size == 0;};
    void print(bool withNL = false); // print the items in one single row if false, otherwise, print each item in a new line
    T headItem();

    ListNode <T>* findMax();
    T extractMax();
    void insertHead(T n);
    void removeHead();
    void removeTail();
    void removeBody(T max_value, ListNode <T>* max_pointer);
    ListNode <T>* getTail();

    void reverseOp();
    void MergeSort(ListNode<T>** start, ListNode <T>* end);
    ListNode <T>* MergeSortedLists(ListNode <T>* head1, ListNode <T>* head2);
    void SplitLists(ListNode <T> *source, ListNode <T> **MidPoint);
    void bubbleSort(bool printAtEveryIteration);
    void partition(int pivot, List* smallerList, List* greaterList);
    void join(List* smaller, List* greater);
    void quickSort();
    void selectionSort();
};

int main ()
{
    List<int> list;
    list.insertHead(5);
    list.insertHead(3);
    //list.insertHead(8);
    list.insertHead(1);
    list.insertHead(4);
    list.print(false);
}


template <class T>
void List<T>::insertHead(T n)
{
    ListNode<T> *aNewNode = new ListNode<T>(n);
    aNewNode->_next = _head;
    _head = aNewNode;
    _size++;
};

template <class T>
void List<T>::removeHead()
{
    if (_size > 0) {
        ListNode<T> *temp = _head;
        _head = _head->_next;
        delete temp;
        _size--;
    }
}

template <class T>
void List<T>::print(bool withNL) {

    ListNode<T> *temp = _head;
    while (temp) {
        cout << temp->_item;
        if (withNL)
            cout << endl;
        else
            cout << " ";
        temp = temp->_next;
    }
    cout << endl;

}

template <class T>
bool List<T>::exist(T n) {

    ListNode <T>* current = _head;
    while (current != NULL)
    {
        if (current->_item == n)
            return true;

        current = current->_next;
    }

    return false;
}

template <class T>
T List<T>::headItem()
{
    if (_size)
        return *_head;
}

template <class T>
ListNode <T>* List<T> :: findMax()
{
    // Edge Case of only one item on List
    // Therefore that one item must be the Maximum
    if (_head -> _next == NULL)
        return _head;

    ListNode <T>* traversal = _head;
    ListNode <T> *pointer_to_maximum = NULL;
    T current_maximum = traversal -> _item;   // Initialize current maximum as first item on Linked List

    do
    {
        traversal = traversal -> _next;

        if ( (traversal -> _item) > current_maximum )   // If New Maximum found
        {
            current_maximum = traversal -> _item;       // Store the New Maximum
            pointer_to_maximum = traversal;
        }
    } while (traversal -> _next != NULL);

    if (pointer_to_maximum == NULL)   // Maximum must be First Element
        return _head;

    else
        return pointer_to_maximum;
}

template <class T>
T List<T>::extractMax()
{
    if (_head == NULL)
    {
        std::cout << "Cannot extract Max, list is Empty";
        return NULL;
    }
    ListNode <T>* max_pointer = findMax();
    T max_value = max_pointer -> _item;

    if ( _head == max_pointer)
    {
        //remove_head
        removeHead();
    }

    else if (max_pointer -> _next == NULL)
    {
        //remove_tail
        removeTail();
    }

    else
        removeBody(max_value, max_pointer);

    return max_value;
}

template <class T>
void List<T> :: removeTail()
{
    ListNode <T>* before_tail = _head;

    while (((before_tail -> _next) -> _next) != NULL)
    {
        before_tail = before_tail -> _next;
    }

    ListNode <T>* tail = before_tail -> _next;
    before_tail -> _next = NULL;
    delete(tail);
}

template <class T>
void List<T> :: removeBody(T max_value, ListNode <T>* max_pointer)
{
    ListNode <T>* traversal = _head;

    ListNode <T>* pre = _head;
    while ( (traversal -> _next) -> _item != max_value )
    {
        pre = pre -> _next;
    }

    ListNode <T>* node_to_remove = pre -> _next;
    ListNode <T>* post = node_to_remove -> _next;

    pre -> _next = post;
    delete(max_pointer);
}

template <class T>
ListNode <T>* List<T> :: getTail()
{
    ListNode <T> *traversal = _head;

    while (traversal->_next != NULL)
    {
        traversal = traversal -> _next;
    }

    return traversal;
}


template <class T>
void List<T> :: reverseOp()
{
    ListNode <T> *current = _head;
    ListNode <T> *pre = NULL;
    ListNode <T>* next = NULL;

    while (current != NULL)
    {
        next = current -> _next;
        current -> _next = pre;
        pre = current;
        current = next;
    }
    _head = pre;
}

template <class T>
void List<T> :: MergeSort(ListNode<T> **start, ListNode <T>* end)
{
    // Base Case
    // If List contains one or fewer elements, it is considered sorted
    if (*start == end)
        return;

    // Split the List into half
    ListNode <T>* MidPoint = NULL;
    SplitLists(*start, &MidPoint);
    assert(MidPoint != NULL);

    ListNode <T> *head2 = MidPoint -> _next;
    ListNode <T> *tail2 = end;

    MidPoint -> _next = NULL;
    ListNode <T> *head1 = *start;
    ListNode <T> *tail1 = MidPoint;

    // Recursively Sort Left Half of List
    MergeSort(&head1, tail1);

    // Recursively Sort Right Half of List
    MergeSort(&head2, tail2);

    // Reconnecting Broken List
    //MidPoint -> _next = head2;

    // Merge Left and Right Sorted Halves
    ListNode <T> *newHead = MergeSortedLists(head1, head2);

    *start = newHead;
}

template <class T>
ListNode <T>* List <T> :: MergeSortedLists(ListNode <T>* head1, ListNode <T>* head2)
{
    // If when Combining Two Subarrays, one of the Subarrays is Empty,
    // Just need to return the other non-empty Sub-Array
    if (head1 == NULL)
        return head2;
    else if (head2 == NULL)
        return head1;

    ListNode <T> *newHead = NULL;

    if ( head1 ->_item < head2 -> _item )
    {
        newHead = head1;
        newHead->_next = MergeSortedLists(head1 -> _next, head2);
    }

    else
    {
        newHead = head2;
        newHead->_next = MergeSortedLists(head1, head2 -> _next);
    }

    return newHead;
}

template <class T>
void List <T> :: SplitLists(ListNode <T> *source, ListNode <T> **MidPoint)
{
    ListNode <T>* fast_pointer = source;
    ListNode <T>* slow_pointer = source;

    while (fast_pointer != NULL)
    {
        fast_pointer = fast_pointer -> _next;
        if (fast_pointer == NULL)
            break;

        fast_pointer = fast_pointer -> _next;
        if (fast_pointer == NULL)
            break;

        slow_pointer = slow_pointer -> _next;
    }

    // Slow Pointer is now at Middle of List
    *MidPoint = slow_pointer;
}

template <class T>
void List<T> :: partition(int pivot, List<T>* smallerList, List<T>* greaterList) {

    // Finding Pivot Index && Pivot Value
    ListNode<T> *traversal = _head;
    ListNode<T> *pivot_index;
    while (traversal != nullptr)
    {
        if (traversal -> _item == pivot)
            pivot_index = traversal;

        traversal = traversal -> _next;
    }

    // Swapping Pivot Index, with 0th Index
    int temp = _head -> _item;
    _head -> _item = pivot_index -> _item;
    pivot_index -> _item = temp;


    ListNode<T>* start = _head;
    ListNode<T>* seperate = _head;           // Seperation Index begins at 0th Index (swapped Pivot)
    traversal = seperate -> _next;           // Traversal begins after Seperation

    // Explore Unknown Regions
    while (traversal != nullptr)
    {
        if (traversal -> _item < pivot)
        {
            seperate = seperate -> _next;

            int temp = seperate -> _item;
            seperate -> _item = traversal -> _item;
            traversal -> _item = temp;
        }

        traversal = traversal -> _next;
    }

    // Final swapping of Seperation and 0th Index (Start Point)
    temp = _head -> _item;
    _head -> _item = seperate -> _item;
    seperate -> _item = temp;

    // Assert that Original List is sorted now

    // Inserting for smallerList
    traversal = _head;
    while ( traversal -> _item != pivot )   // Will stop just before Pivot Point
    {
        int insertion_item = traversal -> _item;
        smallerList -> insertHead(insertion_item);
        traversal = traversal -> _next;
        _size--;
    }

    traversal = traversal -> _next;
    _size--;

    // Inserting for greaterList
    while (traversal != nullptr)
    {
        int insertion_item = traversal -> _item;
        greaterList -> insertHead(insertion_item);
        traversal = traversal -> _next;
        _size--;
    }

    // Destroying Original List
    while (_head)
        removeHead();

    // Implementation whereby we do not clear the Original List after partitioning
    /*
    smallerList -> _head = _head;
    traversal = _head;
    while ( (traversal -> _next) -> _item != pivot )
        traversal = traversal -> _next;
    traversal -> _next = nullptr;
    greaterList -> _head = seperate-> _next;
     */
}

template <class T>
void List<T> :: join(List<T>* smaller, List<T>* greater) {

    smaller -> reverse();
    greater -> reverse();
    ListNode<T>* smaller_traversal = smaller -> _head;
    ListNode<T>* greater_traversal = greater -> _head;

    while (greater_traversal != nullptr)
    {
        insertHead(greater_traversal -> _item);
        greater_traversal = greater_traversal -> _next;
        greater -> _size--;
        _size++;
    }

    while (smaller_traversal != nullptr)
    {
        insertHead(smaller_traversal -> _item);
        smaller_traversal = smaller_traversal -> _next;
        smaller -> _size--;
        _size++;
    }
}

template <class T>
void List<T>::quickSort() {

    // Edge Case
    if (_size <= 1)
        return;

    List smallerList;
    List greaterList;
    int pivot_value = _head -> _item;
    partition(_head -> _item, &smallerList, &greaterList);

    smallerList.quickSort();   // Recursive
    greaterList.quickSort();   // Recursive

    // Append Pivot to Head of Greater List
    greaterList.insertHead(pivot_value);

    // Join all the Sorted Parts
    join(&smallerList, &greaterList);
}

template <class T>
void List<T> :: selectionSort()
{
    ListNode<T>* unsorted_marker = _head;
    ListNode<T>* curr_min = _head;
    ListNode<T>* traversal = _head;

    while (unsorted_marker != nullptr)
    {
        traversal = unsorted_marker;
        curr_min = unsorted_marker;

        while (traversal != nullptr)
        {
            if (traversal -> _item < curr_min -> _item)
            {
                int temp = curr_min -> _item;
                curr_min -> _item = traversal -> _item;
                traversal -> _item = temp;
            }

            traversal = traversal -> _next;
        }

        // Swap current minimum with Unsorted Marker
        int temp = unsorted_marker -> _item;
        unsorted_marker -> _item  = curr_min -> _item;
        curr_min -> _item = temp;

        // Increment Left Boundary
        unsorted_marker = unsorted_marker -> _next;
    }
}

template <class T>
void List<T> ::bubbleSort( bool printAtEveryIteration ) {

    // ENTER YOUR ANSWER BELOW.

    ListNode<T>* original = _head;
    ListNode<T>* curr = _head;

    ListNode<T>* traversal = curr -> _next;
    ListNode<T>* end = nullptr;

    while (curr -> _next != end)
    {
        while (traversal != end)
        {
            // Swapping if needed
            if (curr -> _item > traversal -> _item)
            {
                int temp = traversal -> _item;
                traversal -> _item = curr -> _item;
                curr -> _item = temp;
            }

            curr = curr -> _next;
            traversal = curr -> _next;

        }

        end = curr;
        curr = original;
        traversal = curr -> _next;
    }
}
