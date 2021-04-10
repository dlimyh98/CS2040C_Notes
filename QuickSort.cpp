#include <algorithm>
#include <iostream>

int partition(int arr[], int start_index, int end_index);
void QuickSort(int arr[], int low, int high);


int partition( int arr[], int start_index, int end_index)
{
    int pivot_index;
    int pivot_value;
    int seperate_index;

    // Random QuickSort Implementation
    pivot_index = (start_index + end_index) / 2;
    pivot_value = arr[pivot_index];
    std::swap(arr[pivot_index], arr[start_index]);      // Swap Random Index with First Element VALUE
    seperate_index = start_index;                              // Ensure that Seperate Index starts at 0th Index
    pivot_index = start_index;

    // Normal Quicksort Implementation
    //pivot_index = start_index;
    //pivot_value = arr[pivot_index];
    //seperate_index = start_index;

    for (int traversal = seperate_index + 1; traversal <= end_index; traversal++)
    {
        if (arr[traversal] < pivot_value)
        {
            seperate_index++;
            std::swap(arr[seperate_index], arr[traversal]);
        }
    }

    std::swap( arr[seperate_index], arr[start_index]);

    return seperate_index;
}

void QuickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivot_index = partition(arr, low, high);
        QuickSort(arr, low, pivot_index);
        QuickSort(arr, pivot_index + 1, high);
    }
}

