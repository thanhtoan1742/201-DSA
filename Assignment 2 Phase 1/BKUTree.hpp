#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

template <class K, class V>
class BKUTree {
#pragma region // BKU Tree
public:
    class AVLTree;
    class SplayTree;

    class Entry {
    protected:
        K key;
        V value;

        Entry(K key, V value) : key(key), value(value) {}
    };

private:
    class BaseNode {
        Entry* entry;
    };

    K& getKey(BaseNode* p) {
        return p->e->key;
    }
    V& getValue(BaseNode* p) {
        return p->e->value;
    }

private:
    AVLTree* avl;
    SplayTree* splay;
    list<K> hist;
    int maxHistSize;

    vector<K>* traversingList;

public:
    BKUTree(int _maxHistSize = 5) {
        maxHistSize = _maxHistSize;
        avl = new AVLTree();
        splay = new SplayTree();

        traversingList = nullptr;
    }

    ~BKUTree() { 
        delete avl;
        delete splay;
    }

    /*
    *   Create an entry with key and value then add the entry to both trees.
    *   Link the node on both tree to each other.
    *   Throw "Duplicate key" exception if the key is already in the trees.
    *   Add the key to history if the key is added to the trees successfully.
    *   If the history is full, remove the first in the history.
    */
    void add(K key, V value) {
        auto avlNode = avl->add(key, value);
        auto splayNode = splay->add(avlNode->entry);
        avlNode->corr = splayNode;
        splayNode->corr = avlNode;

        // TEST: hist overflow.
        // TEST: do not add to hist if not adding to the tree successfully.
        if (hist.size() == maxHistSize)
            hist.pop_front();
        hist.push_back(key);
    }

    /*
    *   remove the entry whose key is equivalent to key (parameter).
    *   if remove successfully and the key's in the history, remove the key from history
    *   then add the key of the root of splay tree if the root exists.
    */
    void remove(K key) {
        avl->remove(key);
        splay->remove(key);

        auto it = find(hist.begin(), hist.end(), key);
        if (it != hist.end()) {
            hist.erase(it);
            // TEST: when splay->root = nullptr
            // TEST: do not alter hist if not removing anything.
            if (splay->root)
                hist.push_back(splay->root->key);
        }
    }

    /*
    *   Fuck you whoever created this search function.
    *   If there is no entry in the trees with the equivalent key, throw
    *   "Not found" exception.
    *   If the entry we need in the root of splay tree, return the value of that entry.
    *   If the key in history, search on the splay tree then return the value of the entry we need,
    *   splay the node contains the entry we need once.
    *   If the key is not in the history:
    *       Search on the AVL subtree whose root is the node linked with the root of 
    *       Splay tree. If found return the value of the entry.
    *       If not found in the above search, search on the AVL tree from its root, ignore the subtree
    *       whose root is linked with the root of Splay tree. If found, splay the node contains the
    *       entry we need on the Splay tree once.
    * 
    *   If searching found, add the key to history. If the history is full, remove the 
    *   first on in history.
    * 
    *   traversedList contain the key of the node that this search function has searched
    *   on in the right order, including the key of the entry we need.
    */
    V search(K key, vector<K>& traversedList) {
        if (!splay->root)
            throw "Not found";

        traversingList = &traversedList;
        traversingList->clear();
        BaseNode* res = nullptr;

        if (getKey(splay->root) == key) {
            res = splay->root;
            traversedList.push_back(key);
        }
        else if (count(hist.begin(), hist.end(), key)) {
            res = splay.find(splay->root, key);
            // NOTE: splay exactly once here.
            splay->splay(res);
        }
        else {
            res = avl.find(splay->root->corr, key);
            if (!res) {
                res = avl.find(avl->root, key, splay->root->corr);
                // TODO: if res: splay exactly once at res->corr.
                if (res) 
                    splay->splay((typename AVLTree::Node*)(res)->corr);
            }
        }

        if (!res)
            throw "Not found";

        if (hist.size() == maxHistSize)
            hist.pop_front();
        hist.push_back(key);

        traversingList = nullptr;
        return getValue(res);
    }

    void traverseNLROnAVL(void (*func)(K key, V value)) {
        avl->traverseNLR(func);
    }

    void traverseNLROnSplay(void (*func)(K key, V value)) {
        splay->traverseNLR(func);
    }

    void clear() {
        hist.clear();
        avl->clear();
        splay->clear();
    }

#pragma endregion
    class SplayTree {
    public:
        class Node: public BaseNode {
            Node* parent;
            Node* left;
            Node* right;
            typename AVLTree::Node* corr;

            Node(Entry* entry = nullptr, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr) {
                this->entry = entry;
                this->parent = parent;
                this->left = left;
                this->right = right;
                this->corr = nullptr;
            }

            friend class SplayTree;
            friend class BKUTree;
        };
   
    public:
        Node* root;

    private:
        // return pointer to the node with the equivalent key. If not found, return nullptr.
        void splay(Node* node);
        
        /*
        *   Return the node whose key is equivalent to key(parameter).
        *   If no node found, return nullptr.
        *   If  BKU::traversingList is not null, record the finding process 
        *   to BKU::traversingList.
        */
        Node* find(Node* root, K key) {
            if (!root) 
                return nullptr;

            if (BKUTree::traversingList)
                BKUTree::traversingList->push_back(getKey(root));

            if (getKey(root) == key)    
                return root;
            
            if (key < getKey(root))
                return find(root->left, key);
            else    
                return find(root->right, key);
        }
        
        /*
        *   Add entry to the Splay tree.
        *   Return the newly added node.
        *   If the key is duplicate, throw "Duplicate key".
        */
        Node* add(Node*& root, Entry* entry, Node* par = nullptr) {
            if (!root) {
                root = new Node(entry, par);
                return root;
            }

            if (getKey(root) == entry->key)
                throw "Duplicate key";

            Node* p = nullptr;
            if (entry->key < getKey(root))
                p = add(root->left, entry, root);
            else
                p = add(root->right, entry, root);

            // Splay here if necessary.
            return p;
        }

        void remove(Node*& root, K key);

        void clear(Node*& root) {
            if (!root)
                return;
            clear(root->left);
            clear(root->right);
            delete root;
        }

        void traverseNLR(Node* root, void (*func)(K key, V value)) {
            if (!root)
                return;

            func(getKey(root), getValue(root));
            traverseNLR(root->left, func);
            traverseNLR(root->right, func);
        }

    public:
    #pragma region // Splay Tree public method
        SplayTree(): root(nullptr) {};
        ~SplayTree() { this->clear(); };
        
        Node* add(K key, V value) {
            return add(new Entry(key, value));
        }
        Node* add(Entry* entry) {
            return add(root, entry);
        }
        void remove(K key) {
            remove(root, key);
        }

        void clear() {
            clear(root);
        }

        V search(K key) {
            auto node = find(root, key);
            if (!node)
                throw "Not found";
            return getValue(node);
        }

        void traverseNLR(void (*func)(K key, V value)) {
            traverseNLR(root, func);
        }
    #pragma endregion
    };

    class AVLTree {
    public:
        class Node : public BaseNode {
            Node* left;
            Node* right;
            int balance;
            typename SplayTree::Node* corr;

            Node(Entry* entry = nullptr, Node* left = nullptr, Node* right = nullptr) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = nullptr;
            }

            friend class AVLTree;
            friend class BKUTree;
        };

    public:
        Node* root;

    private:

        /*
        *   Return the node whose key is equivalent to key(parameter).
        *   If no node found, return nullptr.
        *   Ignore the node == ignored.
        *   If  BKU::traversingList is not null, record the finding process 
        *   to BKU::traversingList.
        */
        Node* find(Node* root, K key, Node* ignored = nullptr) {
            if (!root)
                return nullptr;
            if (root == ignored)
                return nullptr;

            if (BKUTree::traversingList)
                BKUTree::traversingList->push_back(getKey(root));
            
            if (getKey(root) == key)
                return root;

            if (key < getKey(root))
                return find(root->left, key, ignored);
            else 
                return find(root->right, key, ignored);
        }

        Node* add(Node*& root, Entry* entry);
        void remove(Node*& root, K key);

        void clear(Node*& root) {
            if (!root)
                return;
            clear(root->left);
            clear(root->right);
            delete root;
        }

        void traverseNLR(Node* root, void (*func)(K key, V value)) {
            if (!root)
                return;

            func(getKey(root), getValue(root));
            traverseNLR(root->left, func);
            traverseNLR(root->right, func);
        }

    public:
    #pragma region // AVL Tree public method
        AVLTree(): root(nullptr) {};
        ~AVLTree() { this->clear(); };

        Node* add(K key, V value) {
            return add(new Entry(key, value));
        }
        Node* add(Entry* entry) {
            return add(root, entry);
        }
        void remove(K key) {
            remove(root, key);
        }

        void clear() {
            clear(root);
        }

        V search(K key) {
            auto node = find(root, key);
            if (!node)
                throw "Not found";
            return getValue(node);
        }

        void traverseNLR(void (*func)(K key, V value)) {
            traverseNLR(root, func);
        }
    #pragma endregion
    };
};