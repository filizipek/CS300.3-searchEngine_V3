// SortingClass.h

#ifndef HOMEWORKS_SORTINGCLASS_H
#define HOMEWORKS_SORTINGCLASS_H

#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;

// Declaration of customSwap
template <class Comparable>
inline void customSwap(Comparable &obj1, Comparable &obj2);

// Declaration of customMedian3
template <class Comparable>
const Comparable &customMedian3(vector<Comparable> &a, int left, int right);

// Declaration of quicksort taking vector by value
template <class Comparable>
vector<Comparable> quicksort(vector<Comparable> a, int pivotMethod);

// Declaration of quicksort taking vector by reference
template <class Comparable>
void quicksortref(vector<Comparable> &a, int pivotMethod);

// Declaration of insertionSort
template <class Comparable>
void insertionSort(vector<Comparable> &a, int left, int right);

// Declaration of internal quicksort method
template <class Comparable>
void quicksort(vector<Comparable> &a, int left, int right, int pivotMethod);

// Declaration of heapsort
template <class Comparable>
void heapsort(vector<Comparable> &a);

// Declaration of mergeSort algorithm (driver)
template <class Comparable>
void mergeSort(vector<Comparable> &a);

// Declaration of internal method that makes recursive calls for mergeSort
template <class Comparable>
void mergeSort(vector<Comparable> &a, vector<Comparable> &tmpArray, int left, int right);

// Declaration of internal method that merges two sorted halves of a subarray
template <class Comparable>
void merge(vector<Comparable> &a, vector<Comparable> &tmpArray, int leftPos, int rightPos, int rightEnd);

template <class Comparable>
void insertionSort(vector<Comparable> &a);

#include "SortingClass.cpp"  // Include the implementations
#endif // HOMEWORKS_SORTINGCLASS_H


