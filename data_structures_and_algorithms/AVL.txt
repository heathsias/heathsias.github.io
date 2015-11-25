// AVLSet.hpp
//
// Heath Sias

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include "Set.hpp"

#include <iostream>



template <typename T>
class AVLSet : public Set<T>
{
public:
    AVLSet();

    virtual ~AVLSet();

    AVLSet(const AVLSet& s);

    AVLSet& operator=(const AVLSet& s);

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
        int height;
    };

    Node* root;
    unsigned int numNodes;


private:
    int maxHeight(int first, int second);
    void addRec(const T& element, Node* rRoot);
    void balanceSubTree(Node* root);
    void fixHeights(Node* root);
    void copyAll(const Node* refN);
    void copyAllRec(const Node* refN, Node* thisN);
    void destroyAll(Node* node);
};


template <typename T>
AVLSet<T>::AVLSet()
    : root{nullptr}, numNodes{0}
{
}


template <typename T>
AVLSet<T>::~AVLSet()
{
    destroyAll(root);
}


template <typename T>
AVLSet<T>::AVLSet(const AVLSet& s)
    : root{nullptr}, numNodes{0}
{
    copyAll(s.root);
}


template <typename T>
AVLSet<T>& AVLSet<T>::operator=(const AVLSet& s)
{
    if (this != &s)
    {
        destroyAll(root);
        copyAll(s.root);
    }

    return *this;
}


template <typename T>
bool AVLSet<T>::isImplemented() const
{
    return true;
}


template <typename T>
void AVLSet<T>::add(const T& element)
{
    if (root == nullptr)
    {
        root = new Node{element, nullptr, nullptr, 0};
        numNodes++;
        return;
    }

    addRec(element, root);
}


template <typename T>
void AVLSet<T>::addRec(const T& element, Node* rRoot)
{
    if (rRoot->key == element)
    {
        return;
    }

    else if (element > rRoot->key)
    {

        if (rRoot->right != nullptr)
        {
            addRec(element, rRoot->right);
        }
        else if (rRoot->left != nullptr)
        {
            rRoot->right = new Node{element, nullptr, nullptr, 0};
            numNodes++;
        }
        else
        {
            rRoot->right = new Node{element, nullptr, nullptr, 0};
            numNodes++;
        }
    }

    else // element < rRoot->key
    {

        if (rRoot->left != nullptr)
        {
            addRec(element, rRoot->left);
        }
        else if (rRoot->right != nullptr)
        {
            rRoot->left = new Node{element, nullptr, nullptr, 0};
            numNodes++;
        }
        else
        {
            rRoot->left = new Node{element, nullptr, nullptr, 0};
            numNodes++;
        }
    }

    balanceSubTree(rRoot);

    if (rRoot->left != nullptr && rRoot->right != nullptr)
    {
        rRoot->height = maxHeight(rRoot->left->height, rRoot->right->height) + 1;
    }
    else if (rRoot->left == nullptr && rRoot->right != nullptr)
    {
        rRoot->height = rRoot->right->height + 1;
    }
    else if (rRoot->left != nullptr && rRoot->right == nullptr)
    {
        rRoot->height = rRoot->left->height + 1;
    }



}


template <typename T>
bool AVLSet<T>::contains(const T& element) const
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
unsigned int AVLSet<T>::size() const
{
    return numNodes;
}


template <typename T>
void AVLSet<T>::balanceSubTree(Node* rRoot)
{

    Node* child = nullptr;
    Node* grandchild = nullptr;

    if (rRoot->left != nullptr && rRoot->right != nullptr)
    {
        if (rRoot->left->height - rRoot->right->height > 1)
        {
            child = rRoot->left;

            // LL Rotation
            if (child->left->height > child->right->height)
            {
                grandchild = child->left;
                // create temp copy of rRoot
                // attach rRoot->right to temp->right
                // attach child->right to temp->left
                Node* temp = new Node{rRoot->key, child->right, rRoot->right, rRoot->height};
                // copy key from child to rRoot
                rRoot->key = child->key;
                // attach child to rRoot->right
                rRoot->right = child;
                // attach grandchild to rRoot->left
                rRoot->left = grandchild;
                // delete rRoot->right
                delete rRoot->right;
                // attach temp to rRoot->right
                rRoot->right = temp;

                //fix heights
                fixHeights(rRoot);



            }
            // LR Rotation
            else
            {
                grandchild = child->right;

                // R
                rRoot->left = grandchild;
                grandchild->left = child;
                child->right = nullptr;

                // L
                Node* temp = new Node{rRoot->key, rRoot->right, grandchild->right, rRoot->height};
                rRoot->key = grandchild->key;
                rRoot->right = grandchild;
                rRoot->left = child;
                delete rRoot->right;
                rRoot->right = temp;

                //fix heights
                fixHeights(rRoot);

            }
        }

        else if (rRoot->right->height - rRoot->left->height > 1)
        {

            child = rRoot->right;

            // RL Rotation
            if (child->left->height > child->right->height)
            {
                grandchild = child->left;

                // L
                root->right = grandchild;
                grandchild->right = child;
                child->left = nullptr;

                // R
                Node* temp = new Node{rRoot->key, rRoot->left, grandchild->left, rRoot->height};
                rRoot->key = grandchild->key;
                rRoot->left = grandchild;
                rRoot->right = child;
                delete rRoot->left;
                rRoot->left = temp;

                //fix heights
                fixHeights(rRoot);

            }


            // RR Rotation
            else
            {
                grandchild = child->right;
                // create temp copy of rRoot
                // attach rRoot->left to temp->left
                // attach child->left to temp->right
                Node* temp = new Node{rRoot->key, rRoot->left, child->left, rRoot->height};
                // copy key from child to rRoot
                rRoot->key = child->key;
                // attach child to rRoot->left
                rRoot->left = child;
                // attach grandchild to rRoot->right
                rRoot->right = grandchild;
                // delete rRoot->left
                delete rRoot->left;
                // attach temp to rRoot->left
                rRoot->left = temp;

                //fix heights
                fixHeights(rRoot);

            }
        }
    }

    else if (rRoot->left == nullptr && rRoot->right != nullptr)
    {
        if ((rRoot->right->height + 1) > 1)
        {
            child = rRoot->right;

            // RL Rotation
            if (child->right == nullptr)
            {
                rRoot->left = grandchild;
                grandchild->right = child;
                rRoot->right = grandchild;
                rRoot->left = nullptr;
                child->left = nullptr;

                child = rRoot->right;
            }

            // RR Rotation

            grandchild = child->right;
            // create temp copy of rRoot
            // set temp left and right to nullptr
            Node* temp = new Node{rRoot->key, nullptr, nullptr, rRoot->height};
            // copy key from child to rRoot
            rRoot->key = child->key;
            // attach child to rRoot->left
            rRoot->left = child;
            // attach grandchild to rRoot->right
            rRoot->right = grandchild;
            // delete rRoot->left
            delete rRoot->left;
            // attach temp to rRoot->left
            rRoot->left = temp;

            //fix heights
            fixHeights(rRoot);

        }
    }



    else if (rRoot->left != nullptr && rRoot->right == nullptr)
    {
        if ((rRoot->left->height + 1) > 1)
        {
            child = rRoot->left;

            // LR Rotation
            if (child->left == nullptr)
            {
                rRoot->right = grandchild;
                grandchild->left = child;
                rRoot->left = grandchild;
                rRoot->right = nullptr;
                child->right = nullptr;

                child = rRoot->left;
            }

            // LL Rotation

            grandchild = child->left;
            // create temp copy of rRoot
            // set temp left and right to nullptr
            Node* temp = new Node{rRoot->key, nullptr, nullptr, rRoot->height};
            // copy key from child to rRoot
            rRoot->key = child->key;
            // attach child to rRoot->right
            rRoot->right = child;
            // attach grandchild to rRoot->left
            rRoot->left = grandchild;
            // delete rRoot->right
            delete rRoot->right;
            // attach temp to rRoot->right
            rRoot->right = temp;

            //fix heights
            fixHeights(rRoot);

        }
    }
}


template <typename T>
void AVLSet<T>::fixHeights(Node* rRoot)
{
    if (rRoot->left == nullptr && rRoot->right == nullptr)
    {
        rRoot->height = 0;
        return;
    }

    else if (rRoot->left != nullptr && rRoot->right != nullptr)
    {
        fixHeights(rRoot->left);
        fixHeights(rRoot->right);
        rRoot->height = maxHeight(rRoot->left->height, rRoot->right->height) +1;
    }

    else if (rRoot->left == nullptr && rRoot->right != nullptr)
    {
        fixHeights(rRoot->right);
        rRoot->height = rRoot->right->height +1;
    }

    else if (rRoot->left != nullptr && rRoot->right == nullptr)
    {
        fixHeights(rRoot->left);
        rRoot->height = rRoot->left->height +1;
    }
}


template <typename T>
void AVLSet<T>::copyAll(const Node* refN)
{
    //Pre-DFS referenced tree, copying each node to this.

    if (refN == nullptr)
    {
        return;
    }

    root = new Node{refN->key, refN->left, refN->right, refN->height};
    numNodes++;

    if (refN->left != nullptr)
    {
        root->left = new Node{refN->left->key, nullptr, nullptr, refN->left->height};
        numNodes++;
        copyAllRec(refN->left, root->left);
    }

    if (refN->right != nullptr)
    {
        root->right = new Node{refN->right->key, nullptr, nullptr, refN->right->height};
        numNodes++;
        copyAllRec(refN->right, root->right);
    }
}



template <typename T>
void AVLSet<T>::copyAllRec(const Node* refN, Node* thisN)
{
    //Pre-DFS referenced tree, copying each node to this.

    if (refN->left != nullptr)
    {
        thisN->left = new Node{refN->left->key, nullptr, nullptr, refN->left->height};
        numNodes++;
        copyAllRec(refN->left, thisN->left);
    }

    if (refN->right != nullptr)
    {
        thisN->right = new Node{refN->right->key, nullptr, nullptr, refN->right->height};
        numNodes++;
        copyAllRec(refN->right, thisN->right);
    }

}



template <typename T>
void AVLSet<T>::destroyAll(Node* node)
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


template <typename T>
int AVLSet<T>::maxHeight(int first, int second)
{
    if (first > second)
    {
        return first;
    }

    return second;
}


#endif // AVLSET_HPP
