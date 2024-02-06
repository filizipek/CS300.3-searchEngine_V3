#include <iostream>
#include <vector>
#include "BST.h"
#include "HashClass.h"
#include "SortingClass.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

// Struct for storing information about a document
struct DocumentItem {
    string documentName;
    int count = 1;

    // Comparison operator for sorting
    bool operator<(const DocumentItem& other) const {
        return documentName < other.documentName;
    }
};

// Struct for storing information about a word and its occurrences in documents
struct vectorItem {
    string word;
    vector<DocumentItem> docList;

    // Comparison operator for sorting (<)
    bool operator<(const vectorItem &other) const {
        if (word != other.word) {
            return word < other.word;
        }

        // Compare docList lexicographically
        for (size_t i = 0; i < docList.size() && i < other.docList.size(); ++i) {
            if (docList[i] < other.docList[i]) {
                return true;
            } else if (other.docList[i] < docList[i]) {
                return false;
            }
            // If the current elements are equal, continue to the next element
        }

        // If all elements are equal up to the minimum size, the shorter vector comes first
        return docList.size() < other.docList.size();
    }

    // Comparison operator for sorting (>)
    bool operator>(const vectorItem &other) const {
        // This is the opposite logic of the '<' operator
        return other < *this;
    }
};




// Function to preprocess the input file
void preprocessed(ifstream &input, const string &openFile, vector<string> &temp) {
    input.open(openFile.c_str()); // Opening the file
    string word;

    while (input >> word) {
        string alphabeticChars;

        for (char c : word) {
            if (isalpha(c)) {
                alphabeticChars += tolower(c);
            } else {
                // Non-alphabetic character acts as a separator
                if (!alphabeticChars.empty()) {
                    temp.push_back(alphabeticChars);
                    alphabeticChars = ""; // Reset for the next word
                }
            }
        }

        // Add the remaining alphabetic characters
        if (!alphabeticChars.empty()) {
            temp.push_back(alphabeticChars);
        }
    }

    input.close(); // Close the file after reading
}

// Function to preprocess the query
void preprocessQuery(const string &inputLine, vector<string> &wordList) {
    istringstream iss(inputLine);
    string word;

    while (iss >> word) {
        string alphabeticChars;

        for (char c : word) {
            if (isalpha(c)) {
                alphabeticChars += tolower(c);
            } else {
                // Non-alphabetic character acts as a separator
                if (!alphabeticChars.empty()) {
                    wordList.push_back(alphabeticChars);
                    alphabeticChars = ""; // Reset for the next word
                }
            }
        }

        // Add the remaining alphabetic characters
        if (!alphabeticChars.empty()) {
            wordList.push_back(alphabeticChars);
        }
    }
}

// Function to store words in the BST and HashTable
void storing(vector<string> &fileName, vector<string> &temp, BST<string, WordItem<string, vector<DocumentItem>>*> &myTree, HashTable<string, WordItem<string, vector<DocumentItem>>*> &myHashTable) {
    for (int j = 0; j < fileName.size(); j++) {
        ifstream input;
        const string openFile = fileName[j];
        temp = {};
        preprocessed(input, openFile, temp);
        int vecSize = temp.size(), index = 0;
        while (index < vecSize) {
            string word = temp[index];
            //---Storing into the BST---
            if (myTree.findVal(word) != nullptr) {
                bool newDoc = true;
                for (int k = 0; k < myTree.findVal(word)->value->value.size(); k++) {
                    if (myTree.findVal(word)->value->value[k].documentName == fileName[j]) {
                        myTree.findVal(word)->value->value[k].count++;
                        newDoc = false;
                        break;
                    }
                }
                if (newDoc) {
                    DocumentItem doc;
                    doc.documentName = fileName[j];
                    myTree.findVal(word)->value->value.push_back(doc);
                }
            } else if (myTree.findVal(word) == nullptr) {
                // If the word is not in the BST, add it
                vector<DocumentItem> docVec;
                DocumentItem doc;
                doc.documentName = fileName[j];
                docVec.push_back(doc);
                auto *ptr = new WordItem<string, vector<DocumentItem>>(word, docVec, nullptr, nullptr, 0);
                myTree.insert(word, ptr);
            }

            //---Storing into the HashTable---
            if (auto *entry = myHashTable.find(word)) {
                // Word found in HashTable
                bool newDoc = true;
                for (DocumentItem &doc : entry->value) {
                    if (doc.documentName == fileName[j]) {
                        doc.count++;
                        newDoc = false;
                        break;
                    }
                }
                if (newDoc) {
                    DocumentItem doc;
                    doc.documentName = fileName[j];
                    entry->value.push_back(doc);
                }
            } else {
                // If the word is not in the HashTable, add it
                vector<DocumentItem> docVec{DocumentItem{fileName[j]}};
                auto *ptr = new WordItem<string, vector<DocumentItem>>(word, docVec, nullptr, nullptr, 0);
                myHashTable.insert(word, ptr);
            }
            index++;
        }
    }
}


void storingVector(vector<vectorItem> &vectorX, vector<string> &temp, vector<string> &fileName) {
    for (int j = 0; j < fileName.size(); j++) {
        ifstream input;
        const string openFile = fileName[j];
        temp = {};
        preprocessed(input, openFile, temp);
        int vecSize = temp.size(), index = 0;
        string currentDocument = openFile;

        while (index < vecSize) {
            const string &word = temp[index];

            auto it = find_if(vectorX.begin(), vectorX.end(), [&](const vectorItem &item) {
                return item.word == word;
            });

            if (it != vectorX.end()) {
                auto index = distance(vectorX.begin(), it);
                auto &docList = vectorX[index].docList;

                auto docIt = find_if(docList.begin(), docList.end(), [&](const DocumentItem &docItem) {
                    return docItem.documentName == currentDocument;
                });

                if (docIt != docList.end()) {
                    docIt->count++;
                } else {
                    DocumentItem newDocItem;
                    newDocItem.documentName = currentDocument;
                    docList.push_back(newDocItem);
                }
            } else {
                vectorItem newItem;
                newItem.word = word;
                DocumentItem newDocItem;
                newDocItem.documentName = currentDocument;
                newItem.docList.push_back(newDocItem);
                vectorX.push_back(newItem);
            }

            index++;
        }
    }
}

bool binarySearch(const vector<vectorItem>& myVec, const vector<string>& queryWords, const string& document) {
    for (const string& query : queryWords) {
        bool found = false;

        // Binary search for the query word in the vector
        int left = 0, right = myVec.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (mid >= 0 && mid < myVec.size()) {
                if (myVec[mid].word == query) {
                    // Check if the document is in the docList
                    auto docIt = find_if(myVec[mid].docList.begin(), myVec[mid].docList.end(), [&](const DocumentItem &docItem) {
                        return docItem.documentName == document;
                    });

                    if (docIt != myVec[mid].docList.end()) {
                        found = true;
                    }

                    break;
                } else if (myVec[mid].word < query) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            } else {
                // Handle the case where mid is out of range
                break;
            }
        }

        // If the query word is not found, return false
        if (!found) {
            return false;
        }
    }

    // All query words found in the document
    return true;
}

    // All query word








int main() {
    int fileNum = 0;
    vector<string> fileName;

    // Getting input of files
    cout << "Enter number of input files: ";
    cin >> fileNum;

    // Storing the filenames in a vector
    for (int i = 0; i < fileNum; i++) {
        string file;
        cout << "Enter " << i + 1 << ". file name: ";
        cin >> file;
        fileName.push_back(file);
    }

    // Creating the BST
    BST<string, WordItem<string, vector<DocumentItem>> *> myTree;
    HashTable<string, WordItem<string, vector<DocumentItem>> *> myHashTable(nullptr);

    vector<string> temp;
    storing(fileName, temp, myTree, myHashTable);

    //Creating the vectors for sorting
    vector<vectorItem> vector1;
    vector<vectorItem> vector2;
    vector<vectorItem> vector3;
    vector<vectorItem> vector4;


    storingVector(vector1, temp, fileName);
    storingVector(vector2, temp, fileName);
    storingVector(vector3, temp, fileName);
    storingVector(vector4, temp, fileName);

    //Check and Print the Quick Sorting:

    /*
        Quick Sort(median) Time: 7052375
        Quick Sort(random) Time: 7067562
        Quick Sort(first) Time:  7067666
     */

    /*
    //QuickSort: Pivot = First
    int pivotStrategy = 1; // 1 for first pivot method
    quicksort(vector1, pivotStrategy);

    //QuickSort: Pivot = Random
    pivotStrategy = 2; // 2 for random pivot method
    quicksort(vector1, pivotStrategy);

    //QuickSort: Pivot = Median
    pivotStrategy = 3; // 3 for median pivot method
    quicksortref(vector1, pivotStrategy);


    //Heap Sort
    heapsort(vector2);

    //Merge Sort
    mergeSort(vector3);

    //Insertion Sort
    insertionSort(vector4);
*/

    //QuickSort: Pivot = Median
    int pivotStrategy = 3; // 3 for median pivot method
    quicksortref(vector1, pivotStrategy);

    // Print the results after preprocessing
    double loadFac = double(myHashTable.getCurrSize()) / double(myHashTable.getArraySize());
    cout << "After preprocessing, the unique word count is " << myHashTable.getCurrSize()
         << ". Current load ratio is " << loadFac;

    string inputLine;
    bool queryExistsInAnyDocument = false;

    cout << "\nEnter queried words in one line: ";
    cin.ignore(); // Ignore the newline character left in the buffer
    getline(cin, inputLine);

    vector<string> queryWordList;
    preprocessQuery(inputLine, queryWordList);

    // Timing for Binary Search Tree
    int k = 100;
    auto beginTimeBST = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        // QueryDocuments (with BST);

        // For each file, check if all words in the query exist
        for (int h = 0; h < fileName.size(); h++) {
            bool allWordsFoundInDocument = true;

            for (int p = 0; p < queryWordList.size(); p++) {
                string temp = queryWordList[p];

                // Check if the word is in the AVL tree
                if (myTree.findVal(temp) != nullptr) {
                    bool wordFoundInDocument = false;

                    // Check if the word is in the document
                    for (int j = 0; j < myTree.findVal(temp)->value->value.size(); j++) {
                        if (myTree.findVal(temp)->value->value[j].documentName == fileName[h]) {
                            wordFoundInDocument = true;
                            break;
                        }
                    }

                    if (!wordFoundInDocument) {
                        allWordsFoundInDocument = false;
                        break;
                    }
                } else {
                    allWordsFoundInDocument = false;
                    break;
                }
            }

            if (allWordsFoundInDocument && i == 0) {
                queryExistsInAnyDocument = true;

                cout << "in Document " << fileName[h] << ", ";
                bool first = true;

                // Output the result for each word in the query using BST
                for (int y = 0; y < queryWordList.size(); y++) {
                    string newestWord = queryWordList[y];

                    // Check if the word is in the BST tree
                    if (myTree.findVal(newestWord) != nullptr) {
                        int count = myTree.findVal(newestWord)->value->value[h].count;

                        if (count > 0) {
                            if (!first) {
                                cout << ", ";
                            }

                            cout << newestWord << " found " << count << " times";
                            first = false;
                        }
                    }
                }
                cout << "." << endl;
            }

        }

        // If the query doesn't exist in any document
        if (!queryExistsInAnyDocument && i == 0) {
            cout << "No document contains the given query\n";
        }
    }


    auto endBST = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - beginTimeBST).count();


    // Timing for Hash Table
    auto beginTimeHT = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++) {
        // QueryDocuments (with hashtable);
        // For each file, check if all words in the query exist
        for (int h = 0; h < fileName.size(); h++) {
            bool allWordsFoundInDocument = true;

            for (int p = 0; p < queryWordList.size(); p++) {
                string temp = queryWordList[p];

                // Check if the word is in the hash table
                const auto &wordData = myHashTable.find(temp);
                if (wordData != myHashTable.getItemNotFound()) {
                    bool wordFoundInDocument = false;

                    // Check if the word is in the document
                    for (const auto &documentItem: wordData->value) {
                        if (documentItem.documentName == fileName[h]) {
                            wordFoundInDocument = true;
                            break;
                        }
                    }

                    if (!wordFoundInDocument) {
                        allWordsFoundInDocument = false;
                        break;
                    }
                } else {
                    allWordsFoundInDocument = false;
                    break;
                }
            }

            if (allWordsFoundInDocument && i == 0) {
                queryExistsInAnyDocument = true;

                cout << "in Document " << fileName[h] << ", ";
                bool first = true;

                // Output the result for each word in the query using HashTable
                for (int y = 0; y < queryWordList.size(); y++) {
                    string newestWord = queryWordList[y];

                    // Check if the word is in the HashTable
                    const auto &wordData = myHashTable.find(newestWord);
                    if (wordData != myHashTable.getItemNotFound()) {
                        int count = wordData->value[h].count;

                        if (count > 0) {
                            if (!first) {
                                cout << ", ";
                            }
                            cout << newestWord << " found " << count << " times";
                            first = false;
                        }
                    }
                }
                cout << "." << endl;
            }

        }

        // If the query doesn't exist in any document
        if (!queryExistsInAnyDocument && i == 0) {
            cout << "No document contains the given query\n";
        }
    }


    auto endHT = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - beginTimeHT).count();

    // Timing for Binary Search
    auto beginTimeBS = std::chrono::high_resolution_clock::now();
    bool queryFoundInAnyDocument = false;

    for (int i = 0; i < k; i++) {
        for (int h = 0; h < fileName.size(); h++) {
            // Call binarySearch and handle the result
            bool queryFound = binarySearch(vector1, queryWordList, fileName[h]);

            // Check if the query was found and it's the first iteration
            if (queryFound && i == 0) {
                bool first = true;

                // Output the result for each word in the query
                for (int y = 0; y < queryWordList.size(); y++) {
                    string newestWord = queryWordList[y];

                    auto it = find_if(vector1.begin(), vector1.end(), [&](const vectorItem &item) {
                        return item.word == newestWord;
                    });

                    if (it != vector1.end()) {
                        auto index = distance(vector1.begin(), it);
                        auto &docList = vector1[index].docList;

                        auto docIt = find_if(docList.begin(), docList.end(), [&](const DocumentItem &docItem) {
                            return docItem.documentName == fileName[h];
                        });

                        // Check if the word was found in the document
                        if (docIt != docList.end()) {
                            if (!first) {
                                cout << ", ";
                            }

                            // Print document header if it's the first word found in the document
                            if (first) {
                                cout << "in Document " << fileName[h] << ", ";
                            }

                            cout << newestWord << " found " << docIt->count << " times";
                            first = false;
                        }
                    }
                }

                // Print the period only if at least one word was found in the document
                if (!first) {
                    cout << "." << endl;
                }

                // Set the flag to true if the query was found in any document
                queryFoundInAnyDocument = true;
            }
        }


// Print the "No document contains the given query" message if no word in the query was found
    if (!queryFoundInAnyDocument && i == 0) {
        cout << "No document contains the given query" << endl;
    }
}
    auto endBS = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - beginTimeBS).count();



    cout << "\nBinary Search Tree Time: " << endBST/k << "\n";
    cout << "Hash Table Time: " << endHT/k  << "\n";
    cout << "Binary Search Time: " << endBS/k  << "\n";


    // Measure and print the time for QuickSort with different pivot strategies
    pivotStrategy = 1; // First pivot method
    auto startQuickFirst = chrono::high_resolution_clock::now();
    quicksort(vector2, pivotStrategy);
    auto endQuickFirst = chrono::high_resolution_clock::now();
    cout << "\nQuick Sort(first) Time: " << chrono::duration_cast<chrono::nanoseconds>(endQuickFirst - startQuickFirst).count()/k << endl;

    pivotStrategy = 2; // Random pivot method
    auto startQuickRandom = chrono::high_resolution_clock::now();
    quicksort(vector2, pivotStrategy);
    auto endQuickRandom = chrono::high_resolution_clock::now();
    cout << "Quick Sort(random) Time: " << chrono::duration_cast<chrono::nanoseconds>(endQuickRandom - startQuickRandom).count()/k << endl;

    pivotStrategy = 3; // Median pivot method
    auto startQuickMedian = chrono::high_resolution_clock::now();
    quicksort(vector2, pivotStrategy);
    auto endQuickMedian = chrono::high_resolution_clock::now();
    cout << "Quick Sort(median) Time: " << chrono::duration_cast<chrono::nanoseconds>(endQuickMedian - startQuickMedian).count()/k << endl;

    // Measure and print the time for Heap Sort
    auto startHeapSort = chrono::high_resolution_clock::now();
    heapsort(vector2);
    auto endHeapSort = chrono::high_resolution_clock::now();
    cout << "Heap Sort Time: " << chrono::duration_cast<chrono::nanoseconds>(endHeapSort - startHeapSort).count()/k << endl;

    // Measure and print the time for Merge Sort
    auto startMergeSort = chrono::high_resolution_clock::now();
    mergeSort(vector3);
    auto endMergeSort = chrono::high_resolution_clock::now();
    cout << "Merge Sort Time: " << chrono::duration_cast<chrono::nanoseconds>(endMergeSort - startMergeSort).count()/k << endl;

    // Measure and print the time for Insertion Sort
    auto startInsertionSort = chrono::high_resolution_clock::now();
    insertionSort(vector4);
    auto endInsertionSort = chrono::high_resolution_clock::now();
    cout << "Insertion Sort Time: " << chrono::duration_cast<chrono::nanoseconds>(endInsertionSort - startInsertionSort).count()/k << endl;

    cout << "\nSpeed Up BST/HST:" << double(endBST)/double(endHT);
    cout << "\nSpeed Up Merge Sort/Quick Sort(Median): " << double((chrono::duration_cast<chrono::nanoseconds>(endMergeSort - startMergeSort).count())/k)/double((chrono::duration_cast<chrono::nanoseconds>(endQuickMedian - startQuickMedian).count())/k);
    cout << "\nSpeed Up Heap Sort/Quick Sort(Median): " << double((chrono::duration_cast<chrono::nanoseconds>(endHeapSort - startHeapSort).count())/k)/double((chrono::duration_cast<chrono::nanoseconds>(endQuickMedian - startQuickMedian).count())/k);
    cout << "\nSpeed Up Insertion Sort/Quick Sort(Median): " << double((chrono::duration_cast<chrono::nanoseconds>(endInsertionSort - startInsertionSort).count())/k)/double((chrono::duration_cast<chrono::nanoseconds>(endQuickMedian - startQuickMedian).count())/k);

    cout << endl << "\nSpeed Up Binary Search Tree/ Binary Search Time: " << double(endBST/k)/double(endBS/k);
    cout << endl << "Speed Up Binary Search / Hash Table Time: " << double(endBS/k)/double(endHT/k);

    return 0;
}
