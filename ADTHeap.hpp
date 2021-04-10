#include <math.h>
#include <algorithm>

#include <iostream>
using namespace std;

template <class T>
class Heap {
protected:
    T* _heap;
    int _n;
    void _bubbleUp(int index);
    void _bubbleDown(int index);
    int _lookFor(T x); // return the index of the item x, return -1 if not found
    // it is not a good/usual implementation, so we hide it from public
public:
    Heap() {
        _heap = new T[DEFAULTHEAPSIZE];
        _n = 0;
    };

    void insert(T);
    bool empty() {return _n==0;};
    T extractMax();
    T peekMax() {return _heap[0];};
    void printHeapArray();
    void printTree();
    void increaseKey(T from, T to);
    void decreaseKey(T from, T to);
    void deleteItem(T);

    ~Heap() { delete[] _heap; };
};

template <class T>
void Heap<T>::_bubbleUp(int index) {
    int parent_index;

    // Starting from Node that needs to be Bubbled Up
    while (index >= 0)  // Node to be Bubbled Up can maximally reach the ROOT of Heap
    {
       parent_index = floor( (index-1) / 2) ;

       if (_heap[parent_index] > _heap[index])
           break;

        if (_heap[index] > _heap[parent_index])
        {
            std::swap(_heap[index], _heap[parent_index]);
            index = parent_index;
            continue;
        }

        else // Node cannot be Bubbled Up any further
            break;
    }
}

template <class T>
void Heap<T>::_bubbleDown(int index) {
    int left_child_index;
    int right_child_index;

    // Starting from Node that needs to be Bubbled Down
    while (  (index >= 0) && (index < _n) )
    {
        left_child_index = (2 * index) + 1;
        right_child_index = (2 * index) + 2;

        int scenario = BubbleWhichSide(is_LeftChildOk(index), is_RightChildOk(index) );

        // Both are fucked OR Node is larger than BOTH of it's children
        if ( scenario == 1 || (_heap[index] > _heap[left_child_index] && _heap[index] > _heap[right_child_index]) )
            break;

        // Only Left Child is OK
        else if (scenario == 2)
        {
            if (_heap[left_child_index] > _heap[index])
            {
                std::swap(_heap[left_child_index] , _heap[index]);
                index = left_child_index;
                continue;
            }
            break;
        }


        // Only Right Child is OK
        else if (scenario == 3)
        {
            if (_heap[right_child_index] > _heap[index])
            {
                std::swap(_heap[right_child_index] , _heap[index]);
                index = right_child_index;
                continue;
            }
            break;
        }

        else // Both are OK
        {
            if (_heap[right_child_index] > _heap[left_child_index])
            {
                std::swap(_heap[index], _heap[right_child_index]);
                index = right_child_index;
                continue;
            }

            else
            {
                std::swap(_heap[index], _heap[left_child_index]);
                index = left_child_index;
                continue;
            }
        }
    }
}

template <class T>
int Heap<T> :: BubbleWhichSide(bool LeftChildFlag, bool RightChildFlag)
{
    if (LeftChildFlag == false && RightChildFlag == false)
        return 1;

    else if (LeftChildFlag == true && RightChildFlag == false)
        return 2;

    else if (LeftChildFlag == false && RightChildFlag == true)
        return 3;

    else
        return 4;
}

template <class T>
bool Heap<T> :: is_LeftChildOk(int index)
{
    int LeftChildIndex = (2 * index) + 1;

    if (LeftChildIndex >= _n)
        return false;

    else
        return true;
}


template <class T>
bool Heap<T> :: is_RightChildOk(int index)
{
    int RightChildIndex = (2 * index) + 2;

    if (RightChildIndex >= _n)
        return false;

    else
        return true;
}

template <class T>
void Heap<T>::insert(T item) {
    int insert_index = _n;

    // If inserting into an Empty Heap
    if (_n == 0)
        _heap[0] = item;

    else
        _heap[insert_index] = item; // Heap allowed to have Duplicate Values

    _n++;
    _bubbleUp(insert_index);
}

template <class T>
void Heap<T>::deleteItem(T x)
{
    int delete_item_index = _lookFor(x);

    if (delete_item_index == -1)
        return;

    // Else, the item to be deleted exist in the Heap
    std::swap(_heap[delete_item_index], _heap[_n-1]);
    _n --;

    // If swapped Node is larger than it's Parent
    // If swapped Node has no parent above it, then naturally it cannot be bubbled up any further
    int parent_item = floor( _heap[delete_item_index - 1] / 2 );
    if ( parent_item >= 0 && (_heap[delete_item_index] > parent_item) )
        _bubbleUp(delete_item_index);

        // If swapped Node has children larger than it
        // If swapped Node has no Left or Right Child, then it naturally cannot be bubbled down any further
    else if ( (((2*delete_item_index + 1) <= _n) && (_heap[2*delete_item_index + 1] > _heap[delete_item_index]))
              || (((2*delete_item_index + 2) <= _n) && (_heap[2*delete_item_index + 2] > _heap[delete_item_index])) )
        _bubbleDown(delete_item_index);

    // Else, swapped Node fits just fine it's placement
}

template <class T>
T Heap<T>::extractMax() {
    T max_value = _heap[0];

    std::swap(_heap[0], _heap[_n-1]);
    _n--;

    _bubbleDown(0);
    return max_value;
}

template <class T>
void Heap<T>::decreaseKey(T from, T to)
{
    int index_of_key_decreased = _lookFor(from);
    _heap[index_of_key_decreased] = to;
    _bubbleDown(index_of_key_decreased);
}


template <class T>
void Heap<T>::increaseKey(T from, T to)
{
    int index_of_key_increased = _lookFor(from);
    _heap[index_of_key_increased] = to;
    _bubbleUp(index_of_key_increased);
}

template <class T>
int Heap<T>::_lookFor(T x){ // not a very good implementation, but just use this for now.
    int i;
    for(i=0;i<_n;i++)
        if (_heap[i] == x)
            return i;

    return -1;
}

template <class T>
void Heap<T>::printTree() {
    int parity = 0;
	if (_n == 0)
		return;
	int space = pow(2,1 + (int) log2f(_n)),i;
	int nLevel = (int) log2f(_n)+1;
	int index = 0,endIndex;
    int tempIndex;
	
	for (int l = 0; l < nLevel; l++)
	{
		index = 1;
        parity = 0;
		for (i = 0; i < l; i++)
			index *= 2;
		endIndex = index * 2 - 1;
		index--;
        tempIndex = index;
        while (index < _n && index < endIndex) {
            for (i = 0; i < space-1; i++)
                cout << " ";
            if(index==0)
                cout << "|";
            else if (parity)
                cout << "\\";
            else
                cout << "/";
            parity = !parity;
            for (i = 0; i < space; i++)
                cout << " ";
			index++;
		}
        cout << endl;
        index=tempIndex;
		while (index < _n && index < endIndex) {
			for (i = 0; i < (space-1-((int) log10(_heap[index]))); i++)
				cout << " ";
			cout << _heap[index];
			for (i = 0; i < space; i++)
				cout << " ";
			index++;
		}
		
		cout << endl;
		space /= 2;
	}

}


template <class T>
void Heap<T>::printHeapArray() {
    for (int i = 0; i < _n; i++)
        cout << _heap[i] << " ";
    cout << endl;
}

#endif
