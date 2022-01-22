#ifndef Vector_H
#define Vector_H

#include <iostream>
#include <cstdlib>
#include <new>
#define N 5

/*Dynamic array - resized when its half full*/
template <class T>
class Vector {
    T** arr;
    int size;

public:
    Vector() : arr(new T* [N]), size(N) {}

    explicit Vector(int new_size) {
        arr = new T*[new_size];
        size = new_size;
    };

    ~Vector() {
        delete[] arr;
    }

    Vector(const Vector& arr);
    Vector<T>& operator=(const Vector& arr);
    T& operator[](int i);
    T operator[](int i) const;
    void add(int idx, T* elm);
    void resize();
    int getSize();
    void print();
};

/***********************FUNCTION IMPLEMENTATIONS*******************/
template <class T>
inline Vector<T>::Vector(const Vector<T>& arr) {
    arr = new T * [arr.size];
    size = arr.size;
    for (int i = 0; i < size; i++) {
        arr[i] = arr.arr[i];
    }
}

template<class T>
inline Vector<T>& Vector<T>::operator=(const Vector& other)
{
    if (this == &other) {
        return *this;
    }
    delete[] arr;
    arr = new T[other.size];
    size = other.size;
    for (int i = 0; i < size; i++) {
        if (other != nullptr)
            arr[i] = other.arr[i];
    }
    return *this;
}

template<class T>
void Vector<T>::add(int idx, T* elm) {
    if (size = idx * 2) { //array half full
        resize();
    }

    arr[idx] = elm;
    size++;
}

template <class T>
inline T& Vector<T>::operator[](int i) {
    return *arr[i];
}


template <class T>
T Vector<T>::operator[](int i) const {
    return *arr[i];
}

template <class T>
void Vector<T>::resize() {
    if (size == 0) {
        size++;
    }
    auto new_size = 2 * size;
    T** newArr = new T * [new_size];
    //memcpy
    for (int i = 0; i < size; i++) {
        newArr[i] = arr[i];
    }

    size = new_size;
    delete[] arr;
    arr = newArr;
}

template<class T>
inline int Vector<T>::getSize()
{
    return size;
}

template<class T>
inline void Vector<T>::print()
{
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

#endif //Vector_H
