#ifndef HCMUT_SPLAY_TREE
#define HCMUT_SPLAY_TREE

#include <algorithm>
#include <functional>

using namespace std;

template <class K, class V>
class splay_tree {
public:
    class Node {
    protected:
        Node* parent;
        Node* left;
        Node* right;
    public:
        const K key;
        V value;

    private:
        Node(K _key, V _value, Node* _parent = nullptr, Node* _left = nullptr, Node* _right = nullptr)
        : parent(_parent), left(_left), right(_right), key(_key), value(_value) {
        }

    protected:
        void update() {
            if (left) 
                left->parent = this;
            if (right)
                right->parent = this;
        }

        friend class splay_tree;
    };

protected:
    Node* root;
    int nE;

protected:
    void rotate_left(Node*& root) {
        auto p = root->right;
        root->right = p->left;
        p->left = root;
        root = p;

        root->parent = root->right->parent;
        root->right->update();
        root->update();
    }

    void rotate_right(Node*& root) {
        auto p = root->left;
        root->left = p->right;
        p->right = root;
        root = p;

        root->parent = root->left->parent;
        root->left->update();
        root->update();
    }

    void splay(Node*& child) {
        Node* parent = child->parent;
        if (parent->parent) {
            Node* grand = parent->parent;
            if (parent == grand->left)
                rotate_right(grand);
            else 
                rotate_left(grand);
        }

        if (child == parent->left)
            rotate_right(parent);
        else
            rotate_left(parent);
    }

    Node* find(Node* root, const K& key) {
        if (!root)
            return nullptr;

        if (key == root->key)
            return root;

        if (key < root->key)
            return find(root->left, key);
        else
            return find(root->right, key);

    }

    Node* get_right_most(Node* root) {
        while (root->right)
            root = root->right;
        return root;
    }

    void inorder_traverse(Node* root, function<void(const K&, V&)> op) {
        if (!root)
            return;

        inorder_traverse(root->left, op);
        op(root->key, root->value);
        inorder_traverse(root->right, op);
    }   

    Node* add(Node*& root, const K& key, const V& value) {
        if (!root)  {
            root = new Node(key, value);
            return root;
        }

        Node* p = nullptr;
        if (key < root->key)
            p = add(root->left, key, value);
        else
            p = add(root->right, key, value);

        root->update();
        return p;
    }

    void clear(Node* root) {
        if (!root)
            return;
        
        clear(root->left);
        clear(root->right);
        delete root;
    }

public:
    splay_tree()
    : root(nullptr), nE(0) {
    }

    Node* find(const K& key) {
        return find(root, key);
    }

    int count(const K& key) {
        return find(key) != nullptr;
    }

    int size() {
        return nE;
    }

    bool empty() {
        return !nE;
    }

    void inorder_traverse(function<void(const K&, V&)> op) {
        inorder_traverse(root, op);
    }   

    Node* add(const K& key, const V& value) {
        Node* p = add(root, key, value);
        ++nE;
        return p;
    }

    void remove(const K& key) {
        Node* p = find(key);
        if (!p)
            throw "not Found";

        while (p->parent)
            splay(p);

        Node* left = p->left;
        Node* right = p->right;
        if (left)
            left->parent = nullptr;
        if (right)
            right->parent = nullptr;
        delete p;

        if (!left)
            root = right;
        else if (!right)
            root = left;
        else {
            left = get_right_most(left);
            while (left->parent)
                splay(left);
            left->right = right;
        }

        --nE;
    }

    void clear() {
        clear(root);
        nE = 0;
        root = nullptr;
    }
};

#endif // HCMUT_SPLAY_TREE