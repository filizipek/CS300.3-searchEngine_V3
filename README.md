# Search Engine Project - Version 3

## Overview

Welcome to the third version of the Search Engine project! This C++ program has undergone significant enhancements, including the incorporation of various sorting algorithms (Quick Sort, Heap Sort, Merge Sort, and Insertion Sort) to improve the efficiency of indexing and querying documents. Additionally, it compares the performance of Binary Search Tree (BST) and Hash Table data structures for document retrieval.

## Features

- **Document Indexing:**
  - Efficiently indexes words using Binary Search Tree (BST) and Hash Table data structures.
  - Utilizes sorting algorithms to preprocess and organize word occurrences in documents.

- **Sorting Algorithms:**
  - Incorporates Quick Sort, Heap Sort, Merge Sort, and Insertion Sort for efficient word indexing.
  - Measures and compares the runtime of each sorting algorithm.

- **Data Structures:**
  - Utilizes BST for word indexing, supporting efficient search and retrieval.
  - Implements a Hash Table to store and query word occurrences across documents.

- **Query Processing:**
  - Handles user queries, searching for specific words and outputting occurrences in documents.
  - Utilizes Binary Search for document retrieval and compares its performance with other data structures.

## Main Components

### SortingClass

The `SortingClass` contains implementations of various sorting algorithms, including Quick Sort, Heap Sort, Merge Sort, and Insertion Sort. These algorithms are used to preprocess and organize word occurrences efficiently.

### BST (Binary Search Tree)

The `BST` class is responsible for indexing words using a Binary Search Tree. It supports efficient search, insertion, and removal operations.

### HashTable

The `HashTable` class is used to store words and their occurrences. It employs open addressing with quadratic probing to handle collisions efficiently. Dynamic resizing ensures a balanced load factor.

### Main Program (main.cpp)

The `main.cpp` file orchestrates the entire search engine. It handles user input, processes input files, builds the index using both BST and Hash Table, and performs queries. The program also measures and compares the runtime of sorting algorithms and the efficiency of document retrieval using different data structures.

## Usage

1. **Compile the Code:**
   - Compile the provided source code files using a C++ compiler.
   - Ensure all necessary header and implementation files are included in the compilation.

2. **Run the Program:**
   - Execute the compiled program.
   - Follow on-screen instructions to input the number of input files and their names.

3. **Query Documents:**
   - Enter queries to search for specific words or remove words from the index.
   - Receive output detailing word occurrences and statistics in the documents.

4. **Sorting Algorithm Comparison:**
   - Observe the runtime performance of Quick Sort, Heap Sort, Merge Sort, and Insertion Sort.

5. **Data Structure Performance:**
   - Compare the efficiency of Binary Search Tree (BST) and Hash Table for document retrieval.

## Example Usage

```cpp
#include <iostream>
#include "BST.h"
#include "HashClass.h"
#include "SortingClass.h"
// ... (other include statements)

int main() {
    // Example usage of the Search Engine
    // ...

    return 0;
}
```

## Additional Notes

- **Sorting Algorithm Performance:**
  - The program measures and compares the runtime of Quick Sort, Heap Sort, Merge Sort, and Insertion Sort on the same dataset.

- **Data Structure Efficiency:**
  - The program compares the performance of Binary Search Tree (BST) and Hash Table in terms of document retrieval speed.

- **User Queries:**
  - Users can enter queries to search for specific words or remove words from the index.

## Future Enhancements

This version of the Search Engine project provides enhanced performance through the inclusion of sorting algorithms and a comparative analysis of data structures. Future enhancements could include additional sorting algorithms, optimization of existing algorithms, and more advanced query functionalities.

Feel free to explore the source code, experiment with the Search Engine, and analyze the performance metrics provided. Happy searching!
