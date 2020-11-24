#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;


#ifndef ILIST_H
#define ILIST_H

template <class T>
class IList
{
public:
    virtual void add(const T& element) = 0;
    virtual void add(int index, const T& element) = 0;
    virtual T removeAt(int index) = 0;
    virtual bool removeItem(const T& item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T get(int index) = 0;
    virtual void set(int index, const T& element) = 0;
    virtual int indexOf(const T& item) = 0;
    virtual bool contains(const T& item) = 0;
    virtual string toString() = 0;
};

#endif


template<class T>
class DynamicArray {
protected:
    static const int BLOCK_SIZE = 32;

    int nElements;
    int capcity;
    T *pData;

    void reallocate(int new_capacity) {
        capcity = new_capacity;
        nElements = min(nElements, capcity);

        T* p = new T[capcity];
        for (int i = 0; i < nElements; ++i)
            p[i] = move(pData[i]);

        delete[] pData;
        pData = p;
    }

public:
    DynamicArray() {
        capcity = nElements = 0;
        pData = nullptr;
    }

    ~DynamicArray() {
        delete[] pData;
    }

    bool isEmpty() {
        return !nElements;
    }

    int size() {
        return nElements;
    }

    void pushBack(const T& element) {
        if (capcity < nElements + 1)
            reallocate(capcity + BLOCK_SIZE);

        pData[nElements++] = element;
    }

    T& operator[](int idx) {
        return idx < 0 ? pData[nElements + idx] : pData[idx];
    }

    void clear() {
        nElements = 0;
    }
};


template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node
    {
    private:
        T data;
        Node *next;
        Node *prev;
        friend class FragmentLinkedList<T>;

    public:
        Node()
        {
            next = 0;
            prev = 0;
        }
        Node(Node *next, Node *prev)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next, Node *prev)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator
    {
    private:
        FragmentLinkedList<T> *pList;
        Node *pNode;
        int index;

    public:
        Iterator(FragmentLinkedList<T> *_pList = 0, bool begin = true) {
            pList = _pList;
            if (pList) {
                pNode = begin ? pList->head : pList->tail;
                index = begin ? 0 : pList->size();
            }
            else {
                pNode = nullptr;
                index = 0;
            }
        }

        Iterator(int fragmentIndex, FragmentLinkedList<T> *_pList = 0,  bool begin = true) {
            pList = _pList;

            // I added this by myself, could cause "not similar undified behavior".
            // if (fragmentIndex < 0 || fragmentIndex >= pList->fragmentHeads.size())
            //     throw out_of_range("The index is out of range!");

            if (begin) {
                pNode = pList->fragmentHeads[fragmentIndex];
                index = pList->fragmentSize * fragmentIndex;
            }
            else {
                if (fragmentIndex < pList->fragmentHeads.size() - 1) {
                    pNode = pList->fragmentHeads[fragmentIndex + 1];
                    index = pList->fragmentSize * (fragmentIndex + 1);
                } 
                else {
                    pNode = pList->tail;
                    index = pList->size();
                }
            }
        }

        Iterator &operator=(const Iterator &iterator) {
            pList = iterator.pList;
            pNode = iterator.Node;
            index = iterator.index;
            return *this;
        }

        T &operator*() {
            if (index < 0 || index >= pList->size()) {
                throw std::out_of_range("Segmentation fault!");
            }
            return pNode->data;
        }

        bool operator!=(const Iterator &iterator) {
            return pNode != iterator.pNode || index != iterator.index;
        }

        void remove() {
            pNode = pNode->prev;
            pList->removeAt(index);
            --index;
        }

        void set(const T& element) {
            pList->set(index, element);
        }

        Iterator &operator++() {
            if (index == -1) 
                pNode = pList->head;
            else 
                pNode = pNode->next;
            ++index;
            return *this;
        }

        Iterator operator++(int) {
            auto t = *this;
            if (index == -1) 
                pNode = pList->head;
            else 
                pNode = pNode->next;
            ++index;
            return t;
        }

        friend class FragmentLinkedList<T>;
    };

protected:
    DynamicArray<Node*> fragmentHeads;
    int fragmentSize;

    Node* head;
    Node* tail;
    int nElements;

    // this is stupid.
    DynamicArray<T> deletedElements;

    void hardUpdateFragmentHeads() {
        fragmentHeads.clear();

        auto p = head;
        for (int i = 0; i < nElements; ++i) {
            if (i % fragmentSize == 0)
                fragmentHeads.pushBack(p);
            p = p->next;
        }
    }

    T& operator[](int index) {
        if (index < 0 || index >= nElements) 
            throw out_of_range("The index is out of range!");

        auto p = head;
        while (index--) p = p->next;
        return p->data;   
    }

public:
    FragmentLinkedList(int _fragmentSize = 5) {
        head = tail = new Node(T(), nullptr, nullptr);
        nElements = 0;
        fragmentSize = _fragmentSize;
    }

    virtual ~FragmentLinkedList() {
        clear();
        delete tail;
    }

    virtual bool empty() {
        return !nElements;
    }

    virtual int size() {
        return nElements;
    }

    // return -1 if none was found.
    virtual int indexOf(const T& item) {
        auto p = head;
        for (int i = 0; i < nElements; ++i) 
            if (item == p->data) return i;
            else p = p->next;
        return -1;
    }

    virtual T get(int index) {
        return (*this)[index];
    }

    virtual void set(int index, const T& element) {
        if (index < 0 || index >= nElements) 
            throw out_of_range("The index is out of range!");
        
        (*this)[index] = element;
    }

    virtual bool contains(const T& item) {
        return indexOf(item) != -1;
    }

    virtual void add(const T& element) {
        if (!nElements) {
            head = new Node(element, nullptr, nullptr);
            tail->prev = head;
        }
        else {
            auto p = new Node(element, tail, tail->prev);
            tail->prev->next = p;
            tail->prev = p;
        }

        ++nElements;
        hardUpdateFragmentHeads();
    }

    virtual void add(int index, const T& element) {
        // I added this by myself, could cause "not similar undified behavior".
        if (index < 0 || index > nElements) 
            throw out_of_range("The index is out of range!");

        if (!nElements) {
            add(element);
            return;
        }

        if (!index) {
            head = new Node(element, head, nullptr);
            head->next->prev = head;
        }
        else {
            auto p = head;
            for (int i = 1; i < index; ++i) p = p->next;

            p->next = new Node(element, p->next, p);
            if (p->next->next)
                p->next->next->prev = p->next;
        }

        ++nElements;
        hardUpdateFragmentHeads();
    }

    virtual T removeAt(int index) {
        if (index < 0 || index >= nElements) 
            throw out_of_range("The index is out of range!");

        T deleted;
        if (!index) {
            deleted = head->data;

            auto p = head;
            head = head->next;
            head->prev = nullptr;
            delete p;
        }
        else {
            auto p = head;
            while (index--) p = p->next;

            deleted = p->data;
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
        }


        --nElements;
        hardUpdateFragmentHeads();
        deletedElements.pushBack(deleted);
        return deletedElements[-1];
    }

    virtual bool removeItem(const T& item) {
        int index = indexOf(item);
        if (index == -1) return false;
        removeAt(index);
        return true;
    }

    virtual void clear() {
        for (int i = 0; i < nElements; ++i) {
            auto p = head->next;
            delete head;
            head = p;
        }

        nElements = 0;
        hardUpdateFragmentHeads();
    }

    virtual string toString() {
        stringstream ss;
        ss << "[";
        auto p = head;
        for (int i = 0; i < nElements; ++i) {
            if (i) ss << ", " << p->data;
            else ss << p->data; 
            p = p->next;

        }
        ss << "]";
        return ss.str();
    }

    Iterator begin(int index = 0) {
        return Iterator(index, this, true);
    }

    // return end() as [begin, end)
    // the behavior is similar to that of STL.
    Iterator end(int index = -1) {
        if (index == -1) return Iterator(this, false);
        return Iterator(index, this, false);
    }

    // for debugging purpose, remove when submit the code.
    // void log() {
    //     cout << "size: " << size() << '\n';
        

    //     cout << "elements: ";
    //     auto p = head;
    //     for (int i = 0; i < nElements; ++i) {
    //         cout << p->data << ' ';
    //         p = p->next;
    //     }
    //     cout << '\n';

    //     cout << "fragment heads: ";
    //     for (int i = 0; i < fragmentHeads.size(); ++i)
    //         cout << fragmentHeads[i]->data << ' ';
    //     cout << '\n';
    // }
};  