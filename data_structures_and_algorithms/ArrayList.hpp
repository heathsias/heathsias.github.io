// ArrayList.hpp
// Heath Sias
// ICS 46 Spring 2015
//
#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

#include <iostream> //// FOR TESTING PURPOSES ONLY
#include <stdexcept>
#include <memory>



namespace
{
    const unsigned int initialCapacity = 10;

    class OutOfBoundsException{};
}



// DECLARATION

template <typename T>
class ArrayList
{
public:
    ArrayList();
    ArrayList(const ArrayList& a);

    ArrayList& operator=(const ArrayList& a);

    T& at(unsigned int index);
    const T& at(unsigned int index) const;

    void add(const T& s);

    unsigned int size() const;

    unsigned int capacity() const;

private:
    std::shared_ptr<T> items;
    unsigned int sz;
    unsigned int cap;

    void arrayCopy(std::shared_ptr<T> target, std::shared_ptr<T> source, unsigned int size);
};



// DEFINITIONS

template <typename T>
ArrayList<T>::ArrayList()
    : items{new T[initialCapacity], [](T* p) {delete[] p;}}, sz{0}, cap{initialCapacity}
{
}



template <typename T>
ArrayList<T>::ArrayList(const ArrayList& a)
    : items{new T[initialCapacity], [](T* p) {delete[] p;}}, sz{a.sz}, cap{a.cap}
{
    arrayCopy(items, a.items, sz);
}



template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList& a)
{
    if (this != &a)
    {
        std::shared_ptr<T> newItems(new T[a.cap]);
        arrayCopy(newItems, a.items, a.sz);
        sz = a.sz;
        cap = a.cap;

        items = newItems;
    }

    return *this;
}



template <typename T>
T& ArrayList<T>::at(unsigned int index)
{
    if ( (index < 0) || (index >= sz) )
    {
        throw OutOfBoundsException();
    }
    return items.get()[index];
}



template <typename T>
const T& ArrayList<T>::at(unsigned int index) const
{
    if ( (index < 0) || (index >= sz) )
    {
        throw OutOfBoundsException();
    }
    return items[index];
}



template <typename T>
void ArrayList<T>::add(const T& s)
{
    if (sz == cap)
    {

        int newCap = cap * 2 + 1;

        std::shared_ptr<T> newItems(new T[newCap]);
        arrayCopy(newItems, items, sz);
        cap = newCap;

        items = newItems;
    }

    items.get()[sz] = s;
    sz++;
}



template <typename T>
unsigned int ArrayList<T>::size() const
{
    return sz;
}



template <typename T>
unsigned int ArrayList<T>::capacity() const
{
    return cap;
}



// PRIVATE

template <typename T>
void ArrayList<T>::arrayCopy(std::shared_ptr<T> target, std::shared_ptr<T> source, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        target.get()[i] = source.get()[i];
    }
}



#endif // ARRAYLIST_HPP
