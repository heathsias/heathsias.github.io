// HashSet.hpp
//
// Heath Sias
// ICS 46 Spring 2015

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename T>
class HashSet : public Set<T>
{
public:
    // The default capacity of an empty HashSet
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // The percent-full at which the HashSet be increased in size
    static constexpr double MAX_LOAD_FACTOR = 0.8;

    // A HashFunction
    typedef std::function<unsigned int(const T&)> HashFunction;

public:
    // Initializes a HashSet to be empty. Requires a hash function to be passed in.
    HashSet(HashFunction hashFunction);

    virtual ~HashSet();

    HashSet(const HashSet& s);

    HashSet& operator=(const HashSet& s);

    // Adds an element to the set.  If the element is already in the set,
    // function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity exceeds MAX_LOAD_FACTOR.
    virtual void add(const T& element);


    // Returns true if the given element is already in the set, false otherwise.
    virtual bool contains(const T& element) const;


    // Returns the number of elements in the set.
    virtual unsigned int size() const;


private:
    HashFunction hashFunction;

    struct Node
    {
        T key;
        Node* next;
    };

    Node** bucketList;
    unsigned int hsize;
    unsigned int hcapacity;


private:
    // clear the array
    void clear();

    // initialize bucket list to nullptrs
    void bucketListInit(Node** bucketList, unsigned int bucketCount);

    // free memory of bucketList
    void bucketListFree(Node** bucketList, unsigned int bucketCount);

    // copy contents of one bucketlist into another
    void bucketListCopy(Node** targetBL, Node** sourceBL, unsigned int bucketCount);

    double loadFactor() const;
};



template <typename T>
HashSet<T>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, bucketList{new Node*[DEFAULT_CAPACITY]}, hsize{0},
     hcapacity{DEFAULT_CAPACITY}
{
    //init array
    bucketListInit(bucketList, hcapacity);
}


template <typename T>
HashSet<T>::~HashSet()
{
    //delete the contents of each bucket.
    bucketListFree(bucketList, hcapacity);
}


template <typename T>
HashSet<T>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction}, bucketList{new Node*[s.hcapacity]}, hsize{s.hsize},
     hcapacity{s.hcapacity}
{
    // init array
    bucketListInit(bucketList, hcapacity);

    //copy array
    bucketListCopy(bucketList, s.bucketList, s.hcapacity);
}


template <typename T>
HashSet<T>& HashSet<T>::operator=(const HashSet& s)
{
    if (this != &s)
    {
        hashFunction = s.hashFunction;

        //delete the contents of each bucket.
        bucketListFree(bucketList, hcapacity);

        //create new array
        bucketList = new Node*[s.hcapacity];

        // init array
        bucketListInit(bucketList, s.hcapacity);

        //copy array
        bucketListCopy(bucketList, s.bucketList, s.hcapacity);
    }

    return *this;
}


template <typename T>
bool HashSet<T>::isImplemented() const
{
    return true;
}


template <typename T>
void HashSet<T>::add(const T& element)
{
    //add it
    if (!contains(element))
    {
        hsize += 1;
        unsigned int hash = hashFunction(element);
        unsigned int bucket = hash % hcapacity;

        Node* current = bucketList[bucket];

        if (bucketList[bucket] == nullptr)
        {
            bucketList[bucket] = new Node;
            bucketList[bucket]->key = element;
            bucketList[bucket]->next = nullptr;
        }
        else
        {
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = new Node;
            current->next->key = element;
            current->next->next = nullptr;
        }

        // check load factor - rehash all into new set if necessary
        if (loadFactor() > MAX_LOAD_FACTOR)
        {
            unsigned int oldBucketCount = hcapacity;
            hcapacity = (oldBucketCount * 2) + 1;
            Node** newBucketList = new Node*[hcapacity];
            bucketListInit(newBucketList, hcapacity);

            //attach new list to HashSet, keep old for copying info out of.
            Node** oldList = bucketList;
            bucketList = newBucketList;
            Node* oldCurrent = nullptr;

            //reset size before adding entries
            hsize = 0;

            //iterate through old, add each entry to new
            for (unsigned int i = 0; i < oldBucketCount; ++i)
            {
                oldCurrent = oldList[i];

                if (oldCurrent == nullptr)
                {
                    continue;
                }
                else
                {
                    while (oldCurrent != nullptr)
                    {
                        add(oldCurrent->key);
                        oldCurrent = oldCurrent->next;
                    }
                }
            }

            //delete old array
            bucketListFree(oldList, oldBucketCount);
        }
    }
}



template <typename T>
bool HashSet<T>::contains(const T& element) const
{
    //hash element
    unsigned int hash = hashFunction(element);
    unsigned int bucket = hash % hcapacity;

    //find node
    Node* current = bucketList[bucket];

    while (current != nullptr)
    {
        if (current->key == element)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}


template <typename T>
unsigned int HashSet<T>::size() const
{
    return hsize;
}


template <typename T>
void HashSet<T>::clear()
{
    HashSet<T>::~HashSet();
    hsize = 0;
    hcapacity = DEFAULT_CAPACITY;
    bucketList = new Node*[DEFAULT_CAPACITY];
    bucketListInit(bucketList, hcapacity);
}


template <typename T>
void HashSet<T>::bucketListInit(Node** bucketList, unsigned int bucketCount)
{
    for (unsigned int i = 0; i < bucketCount; ++i)
    {
        bucketList[i] = nullptr;
    }
}


template <typename T>
void HashSet<T>::bucketListFree(Node** bucketList, unsigned int bucketCount)
{
    //delete the contents of each bucket.
    Node* current = nullptr;
    Node* next = nullptr;
    for (unsigned int i = 0; i < bucketCount; ++i)
    {
        if (bucketList[i] == nullptr)
        {
            continue;
        }
        else
        {
            current = bucketList[i];
            next = current->next;
            delete current;
            current = next;

            while (current != nullptr)
            {
                next = current->next;
                delete current;
                current = next;
            }
        }
    }
    delete[] bucketList;
}


template <typename T>
void HashSet<T>::bucketListCopy(Node** targetBL, Node** sourceBL, unsigned int bucketCount)
{
    Node* current = nullptr;
    Node* copy = nullptr;

    for (unsigned int i = 0; i < bucketCount; ++i)
    {
        if (sourceBL[i] == nullptr)
        {
            targetBL[i] = nullptr;
            continue;
        }
        else
        {
            current = sourceBL[i];

            copy = new Node;
            copy->key = current->key;
            copy->next = nullptr;

            targetBL[i] = copy;
            current = current->next;

            while (current != nullptr)
            {
                copy->next = new Node;
                copy = copy->next;

                copy->key = current->key;
                copy->next = nullptr;

                current = current->next;
            }
        }
    }
}


template <typename T>
double HashSet<T>::loadFactor() const
{
    return (double)hsize/(double)hcapacity;
}



#endif // HASHSET_HPP
