#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include "list.h"
#define N 5


/*NOTE:  template class must have operator() which returns the insertion key which will be used in hash function*/

template<class T>
class Chain {
public:
    List<T>* chain;
    Chain() : chain(new List<T>()) {}
    ~Chain() {
        delete chain;
    }

    Node<T>* insertElement(T* element);//created new node and inserts to the chain
    int removeElement(T* element); //deletes element
    Node<T>* popElement(); //pops without deleting
    void pushElement(Node<T>* node); //pushes already existing node to chain
    Node<T>* findElement(T* element);
    int getChainSize();
   
};


/* dynamic HashTable - uses chain hashing (modulo function)
* dynamic_arr: a dynamic array where each cell holds a chain of elements of type T
* size: the size of the hash table, aka the current size of the dynamic array
* count: keeps track of the number of elements in the table for the purpose of rehashing*/
template<class T>
class HashTable {
    Chain<T>** dynamic_arr;
    int size;
    int count;

public:
    HashTable();
    HashTable(int _size);
    ~HashTable();
    int hash(int key);
    void rehash();
    Node<T>* insert(T* element, int key);
    void remove(T* element, int key);
    Node<T>* find(T* element, int key);
    int getSize();
    int getCount();
    Chain<T>& operator[](int i) {
        return *dynamic_arr[i];
    }
};

/*returns null if the element already exists in the chain else returns pointer to
  recently inserted element
  *note: the new element is inserted right after the list head (which is a dummy node)*/
template<class T>
Node<T>* Chain<T>::insertElement(T* element) {
    if (chain->find(*element) != nullptr)
        return nullptr;

    chain->insestAfterNode(*element, chain->getHead());
    return chain->getHead()->next;
}

/*returns 0 on success, -1 if element not found*/
template<class T>
int Chain<T>::removeElement(T* element) {
    Node<T>* temp = chain->find(*element);
    if (temp != nullptr) {
        chain->remove(temp);
        return 0;
    }
    return -1;
}

/*returns (head->next) and doesnt delete the node*/
template<class T>
Node<T>* Chain<T>::popElement() {
    return chain->pop_front();
}

template<class T>
void Chain<T>::pushElement(Node<T>* node) {
    return chain->push_front(node);
}

template<class T>
Node<T>* Chain<T>::findElement(T* element)
{
    return chain->find(*element);
}

template<class T>
int Chain<T>::getChainSize() {
    return chain->getSize();
}

/******************************************************************/
template<class T>
HashTable<T>::HashTable() {
    size = N;
    count = 0;
    dynamic_arr = new Chain<T>*[N];
    for (int i = 0; i < size; i++) {
        dynamic_arr[i] = nullptr;
    }
}

template<class T>
HashTable<T>::HashTable(int _size) {
    size = _size;
    count = 0;
    dynamic_arr = new Chain<T>*[size];
    for (int i = 0; i < size; i++) {
        dynamic_arr[i] = nullptr;
    }
}

template<class T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < size; i++) {
        if (dynamic_arr[i] != nullptr)
            delete dynamic_arr[i];
    }
    delete[] dynamic_arr;
}

template<class T>
int HashTable<T>::hash(int key)
{
    return key % size;
}

template<class T>
void HashTable<T>::rehash() {
    //check if theres a need for rehashing
    int old_size = size;
    if (old_size == count) { //must increase size
        size = (size * 2) + 1;
    }
    else if (old_size >= count * 4) { //must decrease size
        old_size = (size - 1) / 2;
    }
    else {
        return; //no need for rehash
    }

    //create new hashtable and initialize it
    Chain<T>** new_arr = new Chain<T>*[size]; //new arr with updated size
    for (int i = 0; i < size; i++) {
        new_arr[i] = nullptr;
    }

    //copy nodes from old table to new one
    for (int i = 0; i < old_size; i++) {
        Chain<T>* current_chain = dynamic_arr[i];
        if (dynamic_arr[i] != nullptr) { //cell contains a chain
            Node<T>* chain_node;
            while (current_chain->getChainSize() > 0) {
                chain_node = current_chain->popElement();
                if (new_arr[hash(chain_node->data())] == nullptr) { //if cell in new arr is empty
                    new_arr[hash(chain_node->data())] = new Chain<T>();
                }

                new_arr[hash(chain_node->data())]->pushElement(chain_node); //push node to chain
            }
        }
    }

    //delete old table
    Chain<T>** del_arr = dynamic_arr;
    dynamic_arr = new_arr; //switch ptrs
    for (int i = 0; i < old_size; i++) {
        if (del_arr[i] != nullptr)
            delete del_arr[i];
    }
    delete del_arr;
}

/*key will be used in hash function to determine which index of insertion in the arr
 * returns null if insertion failed, else returns pointer to the element node*/
template<class T>
Node<T>* HashTable<T>::insert(T* element, int key)
{
    int index = hash(key);
    if (dynamic_arr[index] == nullptr) { //cell is empty
        dynamic_arr[index] = new Chain<T>();
    }
    //add element
    Node<T>* element_node = (dynamic_arr[index])->insertElement(element);
    count++;
    rehash(); //checks if theres a need to rehash, and does rehash accordingly
    return element_node;
}

/*key will be used in hash function to determine which index of insertion in the arr */
template<class T>
void HashTable<T>::remove(T* element, int key) {
    int index = hash(key);
    if (dynamic_arr[index] == nullptr)
        return;

    int returned = (dynamic_arr[index])->removeElement(element);
    if (returned == 0) {
        count--;
        rehash();
    }
    return;
}

/*returns nullptr if not found*/
template<class T>
Node<T>* HashTable<T>::find(T* element, int key) {
    int index = hash(key);
    if (dynamic_arr[index] == nullptr)
        return nullptr;

    return dynamic_arr[index]->findElement(element);
}

template<class T>
int HashTable<T>::getSize()
{
    return size;
}

template<class T>
int HashTable<T>::getCount()
{
    return count;
}


#endif // !HASHTABLE_H
