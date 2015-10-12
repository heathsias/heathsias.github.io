// BSTSet.hpp
//
// Heath Sias
// ICS 46 Spring 2015

#ifndef BSTSET_HPP
#define BSTSET_HPP

#include "Set.hpp"



template <typename T>
class BSTSet : public Set<T>
{
public:
    BSTSet();

    virtual ~BSTSet();

    BSTSet(const BSTSet& s);

    BSTSet& operator=(const BSTSet& s);

    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.
    virtual void add(const T& element);

    // contains() returns true if the given element is already in the set,
    // false otherwise.
    virtual bool contains(const T& element) const;

    // size() returns the number of elements in the set.
    virtual unsigned int size() const;


private:
    struct Node
    {
        T key;
        Node* left;
        Node* right;
    };

    Node* root;
    unsigned int numNodes;


private:
    void copyAll(const Node* refN);
    void copyAllRec(const Node* refN, Node* thisN);
    void destroyAll(Node* node);
};



template <typename T>
BSTSet<T>::BSTSet()
    : root{nullptr}, numNodes{0}
{
}


template <typename T>
BSTSet<T>::~BSTSet()
{
    destroyAll(root);
}


template <typename T>
BSTSet<T>::BSTSet(const BSTSet& s)
    : root{nullptr}, numNodes{0}
{
    copyAll(s.root);
}


template <typename T>
BSTSet<T>& BSTSet<T>::operator=(const BSTSet& s)
{
    if (this != &s)
    {
        destroyAll(root);
        copyAll(s.root);
    }

    return *this;
}


template <typename T>
bool BSTSet<T>::isImplemented() const
{
    return true;
}


template <typename T>
void BSTSet<T>::add(const T& element)
{
    if (root == nullptr)
    {
        root = new Node{element, nullptr, nullptr};
        numNodes++;
        return;
    }

    Node* curr = root;
    Node* parent = nullptr;

    while (true)
    {
        if (curr->key == element)
        {
            return;
        }

        parent = curr;

        if (element > curr->key)
        {
            curr = curr->right;
            if (curr == nullptr)
            {
                parent->right = new Node{element, nullptr, nullptr};
                numNodes++;
                return;
            }
        }
        else //element < curr->key
        {
            curr = curr->left;
            if (curr == nullptr)
            {
                parent->left = new Node{element, nullptr, nullptr};
                numNodes++;
                return;
            }
        }
    }
}


template <typename T>
bool BSTSet<T>::contains(const T& element) const
{
    if (root == nullptr)
    {
        return false;
    }

    Node* curr = root;

    while (curr != nullptr)
    {
        if (curr->key == element)
        {
            return true;
        }

        if (element > curr->key)
        {
            curr = curr->right;
        }
        else //element < curr->key
        {
            curr = curr->left;
        }
    }
    return false;
}


template <typename T>
unsigned int BSTSet<T>::size() const
{
    return numNodes;
}


template <typename T>
void BSTSet<T>::copyAll(const Node* refN)
{
    //Pre-DFS referenced tree, copying each node to this.

    if (refN == nullptr)
    {
        return;
    }

    root = new Node{refN->key, refN->left, refN->right};
    numNodes++;

    if (refN->left != nullptr)
    {
        root->left = new Node{refN->left->key, nullptr, nullptr};
        numNodes++;
        copyAllRec(refN->left, root->left);
    }

    if (refN->right != nullptr)
    {
        root->right = new Node{refN->right->key, nullptr, nullptr};
        numNodes++;
        copyAllRec(refN->right, root->right);
    }

}


template <typename T>
void BSTSet<T>::copyAllRec(const Node* refN, Node* thisN)
{
    //Pre-DFS referenced tree, copying each node to this.

    if (refN->left != nullptr)
    {
        thisN->left = new Node{refN->left->key, nullptr, nullptr};
        numNodes++;
        copyAllRec(refN->left, thisN->left);
    }

    if (refN->right != nullptr)
    {
        thisN->right = new Node{refN->right->key, nullptr, nullptr};
        numNodes++;
        copyAllRec(refN->right, thisN->right);
    }

}


template <typename T>
void BSTSet<T>::destroyAll(Node* node)
{
    //Post-DFS this, removing each node.

    if (node == nullptr)
    {
        return;
    }

    destroyAll(node->left);
    destroyAll(node->right);
    delete node;
    numNodes--;
}


#endif // BSTSET_HPP
