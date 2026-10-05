#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>
#include <initializer_list>
#include <cstddef>
using namespace std;

template <typename T>
struct Node
{
    T data;
    Node *next;
    explicit Node(const T &value) : data(value), next(nullptr) {}
};

struct Property
{
    string name;
    int cost;
    string owner; // Empty string "" means unowned

    // Overload == so search() and remove() work with Property
    bool operator==(const Property &other) const
    {
        return name == other.name;
    }
};

// Lets cout << Property work (used by print())
inline ostream &operator<<(ostream &os, const Property &p)
{
    return os << p.name << " ($" << p.cost << ", owner: " << (p.owner.empty() ? "none" : p.owner) << ")";
}

template <typename T>
class CircularLinkedList
{
private:
    Node<T> *tail; // Tail node; tail->next is the head
    size_t count;  // Number of nodes

public:
    CircularLinkedList() : tail(nullptr), count(0) {}

    CircularLinkedList(const initializer_list<T> &values) : tail(nullptr), count(0)
    {
        for (const T &value : values) append(value);
    }

    // Destructor: frees every node
    ~CircularLinkedList()
    {
        if (empty()) return;
        Node<T> *current = tail->next;
        tail->next = nullptr; // break the circle
        while (current)
        {
            Node<T> *temp = current;
            current = current->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    // Copying would double-delete nodes, so it is disabled
    CircularLinkedList(const CircularLinkedList &) = delete;
    CircularLinkedList &operator=(const CircularLinkedList &) = delete;

    bool empty() const { return tail == nullptr; }
    size_t size() const { return count; }

    // Add a node at the end (new tail)
    void append(const T &value)
    {
        Node<T> *node = new Node<T>(value);
        if (empty())
        {
            node->next = node; // points to itself
            tail = node;
        }
        else
        {
            node->next = tail->next; // new node -> head
            tail->next = node;       // old tail -> new node
            tail = node;             // new node becomes tail
        }
        ++count;
    }

    // Add a node at the front (new head)
    void prepend(const T &value)
    {
        Node<T> *node = new Node<T>(value);
        if (empty())
        {
            node->next = node;
            tail = node;
        }
        else
        {
            node->next = tail->next;
            tail->next = node;
        }
        ++count;
    }

    // Remove the first node matching value
    bool remove(const T &value)
    {
        if (empty()) return false;

        Node<T> *prev = tail;
        Node<T> *current = tail->next;

        do
        {
            if (current->data == value)
            {
                if (current == prev) // only node in the list
                {
                    tail = nullptr;
                }
                else
                {
                    prev->next = current->next;
                    if (current == tail) tail = prev;
                }
                delete current;
                --count;
                return true;
            }
            prev = current;
            current = current->next;
        } while (prev != tail);

        return false;
    }

    // Search for a value
    bool search(const T &value) const
    {
        if (empty()) return false;

        Node<T> *head = tail->next;
        Node<T> *current = head;
        do
        {
            if (current->data == value) return true;
            current = current->next;
        } while (current != head);

        return false;
    }

    // Print the whole list
    void print() const
    {
        if (empty())
        {
            cout << "(list empty)\n";
            return;
        }
        Node<T> *head = tail->next;
        Node<T> *current = head;
        do
        {
            cout << current->data << " -> ";
            current = current->next;
        } while (current != head);
        cout << "(back to head)\n";
    }

    // Head node (first space), where players start
    Node<T> *getHead() const
    {
        return tail ? tail->next : nullptr;
    }

    // Move a node pointer forward 'steps' spaces; passGoCount is how many
    // times it wrapped from the tail back to the head (GO)
    Node<T> *movePointer(Node<T> *current, int steps, int &passGoCount) const
    {
        passGoCount = 0;
        if (!current || empty()) return current;

        for (int i = 0; i < steps; ++i)
        {
            if (current == tail) passGoCount++;
            current = current->next;
        }
        return current;
    }
};

#endif
