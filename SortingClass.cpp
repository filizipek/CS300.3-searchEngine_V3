// SortingClass.cpp
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;

// --------------------------------Quick Sort--------------------------------------

// Implementation of customSwap
template <class Comparable>
inline void customSwap(Comparable &obj1, Comparable &obj2)
{
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}

// Implementation of customMedian3
template <class Comparable>
const Comparable &customMedian3(vector<Comparable> &a, int left, int right)
{
    int center = (left + right) / 2;
    if (a[center] < a[left])
        customSwap(a[left], a[center]);
    if (a[right] < a[left])
        customSwap(a[left], a[right]);
    if (a[right] < a[center])
        customSwap(a[center], a[right]);

    // Place pivot at position right - 1
    customSwap(a[center], a[right - 1]);
    return a[right - 1];
}

// Implementation of quicksort taking vector by value
template <class Comparable>
vector<Comparable> quicksort(vector<Comparable> a, int pivotMethod)
{
    if (!a.empty())
    {
        quicksort(a, 0, a.size() - 1, pivotMethod);
    }
    return a;
}



// Implementation of quicksort taking vector by reference
template <class Comparable>
void quicksortref(vector<Comparable> &a, int pivotMethod)
{
    quicksort(a, 0, a.size() - 1, pivotMethod);
}

// Implementation of insertionSort
template <class Comparable>
void insertionSort(vector<Comparable> &a, int left, int right)
{
    for (int p = left + 1; p <= right; p++)
    {
        Comparable tmp = a[p];
        int j;

        for (j = p; j > left && tmp < a[j - 1]; j--)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}

// Implementation of internal quicksort method
template <class Comparable>
void quicksort(vector<Comparable> &a, int left, int right, int pivotMethod)
{
    if (left + 10 <= right)
    {
        Comparable pivot;
        switch (pivotMethod)
        {
            case 1: // First pivot method
                pivot = a[left];
                break;
            case 2: // Random pivot method
                customSwap(a[left], a[left + rand() % (right - left + 1)]);
                pivot = a[left];
                break;
            case 3: // Median pivot method
                pivot = customMedian3(a, left, right);
                break;
            default:
                cerr << "Invalid pivot method" << endl;
                return;
        }

        int i = left, j = right - 1;
        for (;;)
        {
            while (a[++i] < pivot)
            {
                if (i >= right) // Check if i goes beyond right
                    break;
            }

            while (pivot < a[--j])
            {
            }

            if (i < j)
                customSwap(a[i], a[j]);
            else
                break;
        }
        customSwap(a[i], a[right - 1]);

        quicksort(a, left, i - 1, pivotMethod);
        quicksort(a, i + 1, right, pivotMethod);
    }
    else
        insertionSort(a, left, right);
}


// --------------------------------Heap Sort--------------------------------------
// Implementation of heapsort

// Implementation of leftChild
inline int leftChild(int i)
{
    return 2 * i + 1;
}

template <class Comparable>
void heapsort(vector<Comparable> &a)
{
    // buildHeap
    for (int i = a.size() / 2; i >= 0; i--)
        percDown(a, i, a.size());

    // sort
    for (int j = a.size() - 1; j > 0; j--)
    {
        swap(a[0], a[j]);    // swap max to the last pos.
        percDown(a, 0, j);    // re-form the heap
    }
}

// Implementation of percDown for deleteMax
template <class Comparable>
void percDown(vector<Comparable> &a, int i, int n)
{
    int child;
    Comparable tmp;

    for (tmp = a[i]; leftChild(i) < n; i = child)
    {
        child = leftChild(i);
        if (child != n - 1 && a[child] < a[child + 1])
            child++;
        if (a[child] > tmp)
            a[i] = a[child];
        else
            break;
    }
    a[i] = tmp;
}

// --------------------------------Merge Sort--------------------------------------

// Implementation of mergeSort algorithm (driver)
template <class Comparable>
void mergeSort(vector<Comparable> &a)
{
    vector<Comparable> tmpArray(a.size());
    mergeSort(a, tmpArray, 0, a.size() - 1);
}

// Internal method that makes recursive calls for mergeSort
template <class Comparable>
void mergeSort(vector<Comparable> &a, vector<Comparable> &tmpArray, int left, int right)
{
    if (left < right)
    {
        int center = (left + right) / 2;
        mergeSort(a, tmpArray, left, center);
        mergeSort(a, tmpArray, center + 1, right);
        merge(a, tmpArray, left, center + 1, right);
    }
}

template <class Comparable>
void merge(vector<Comparable> &a,
           vector<Comparable> &tmpArray, int leftPos, int rightPos, int rightEnd)
{
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    // Main loop
    while (leftPos <= leftEnd && rightPos <= rightEnd)
        if (a[leftPos] < a[rightPos])
            tmpArray[tmpPos++] = a[leftPos++];
        else
            tmpArray[tmpPos++] = a[rightPos++];

    while (leftPos <= leftEnd) // Copy rest of first half
        tmpArray[tmpPos++] = a[leftPos++];

    while (rightPos <= rightEnd) // Copy rest of right half
        tmpArray[tmpPos++] = a[rightPos++];

    // Copy tmpArray back
    for (int i = 0; i < numElements; i++, rightEnd--)
        a[rightEnd] = tmpArray[rightEnd];
}

// --------------------------------Insertion Sort--------------------------------------

template <class Comparable>
void insertionSort(vector<Comparable> &a)
{
    int j;
    for (int p = 1; p < a.size(); p++)
    {
        Comparable tmp = a[p];
        for (j = p; j > 0 && tmp < a[j - 1]; j--)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}
