#include <iostream>

void merge(int a[], int low, int mid, int high)
{
    int N = high - low + 1;
    int b[N];
    int left = low;
    int right = mid + 1;
    int bIdx = 0;


    // Main Merging
    while (left <= mid && right <= high)
        b[bIdx++] = (a[left] <= a[right]) ? a[left++] : a[right++];

    // Leftover if any
    while (left <= mid)
        b[bIdx++] = a[left++];

    // Leftover if any
    while (right <= high)
        b[bIdx++] = a[right++];

    // Copy back to original Array
    for (int k = 0; k < N; k ++)
        a[low+k] = b[k];
}

void MergeSort(int a[], int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        MergeSort(a, low, mid);
        MergeSort(a, mid+1, high);
        merge(a, low, mid, high);
    }
}

