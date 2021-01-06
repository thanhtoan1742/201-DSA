//*********** [LIBRARY] ************//
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
//*********************************//


//************************* [BKU TREE] ***********************//
template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;

//****************** [ENTRY] ******************//
    class Entry {
    public:
        K key;
        V value;

        Entry(K key, V value) : key(key), value(value) {}
    };
//*********************************************//

//***************** [PRIVATE OF BKU TREE] *****************//
private: //change this
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    int maxNumOfKeys;
//*********************************************************//

public:
    BKUTree(int maxNumOfKeys = 5) { 
        this -> maxNumOfKeys = maxNumOfKeys;
        splay = new SplayTree();
        avl = new AVLTree();
    }

    ~BKUTree() { this-> clear(); }
    


//--Help function------------------------------------------------------
    bool findInQueue(K key, queue<K> k){
        while (!(k.empty()))
        {
            if (key == k.front()) return true;
            else k.pop();
        }
        return false;
    }


    void addQueue(K key){
        if(keys.size() < (unsigned int)maxNumOfKeys) keys.push(key);
        else {
            keys.pop();
            keys.push(key);
        }
    }
//-----------------------------------------------------------------------


//--Main function----------------------------------------------------------
    void add(K key, V value){
        Entry *x = new Entry(key, value);
        Entry *y = new Entry(key,value);
        typename SplayTree::Node* a = new typename SplayTree::Node(x);
        typename AVLTree::Node* b = new typename AVLTree::Node(y);
        a->corr = b;
        b->corr = a;
        //???
        splay->add(a);
        avl->add(b);

        addQueue(key);
    }



    void remove(K key){
        queue<K> q;
        K temp;
        int flag = 0;
        while(!keys.empty()){
            temp = keys.front();
            keys.pop();
            if(temp == key) flag = 1;
            else q.push(temp);
        }

        while(!q.empty()){
            keys.push(q.front());
            q.pop();
        }

        avl->remove(key);
        splay->remove(key);

        if(flag == 1 && (splay->root)) keys.push(splay->root->entry->key);
    }




    V search(K key, vector<K>& traversedList){
        //key in root--------------------
        if(!(splay -> root)) throw "Not found";
        if(splay -> root -> entry-> key == key) return splay -> root -> entry -> value;
        //traversedList.push_back(splay -> root -> entry -> key);

        //key in queue-------------------
        if(findInQueue(key, keys)) {
            typename SplayTree::Node* temp = splay -> root;
            while (temp){
                if(temp -> entry -> key > key) {
                    traversedList.push_back(temp -> entry -> key);
                    temp = temp -> left;
                }
                else if(temp -> entry -> key < key) {
                    traversedList.push_back(temp -> entry -> key);
                    temp = temp -> right;
                }
                else {
                    splay -> splay(temp, 1);
                    addQueue(key);
                    return temp -> entry -> value;
                }
            }
        }

        //key not in queue------------------
        typename AVLTree::Node* temp = (splay -> root) -> corr;
        typename AVLTree::Node* mark = (splay -> root) -> corr;
        while (temp){
            if(temp -> entry -> key > key) {
                traversedList.push_back(temp -> entry -> key);
                temp = temp -> left;
            }
            else if(temp -> entry -> key < key) {
                traversedList.push_back(temp -> entry -> key);
                temp = temp -> right;
            }
            else {
                splay -> splay(temp -> corr, 1);
                addQueue(key);
                return temp -> entry -> value;
            }
        }

        temp = avl -> root;

        while (temp && (temp != mark)) {
            if(temp -> entry -> key > key) {
                traversedList.push_back(temp -> entry -> key);
                temp = temp -> left;
            }
            else if(temp -> entry -> key < key) {
                traversedList.push_back(temp -> entry -> key);
                temp = temp -> right;
            }
            else {
                splay -> splay(temp -> corr, 1);
                addQueue(key);
                return temp -> entry -> value;
            }
        }
        
        throw "Not found";        
    }



    void traverseNLROnAVL(void (*func)(K key, V value)){
        avl->traverseNLR(func);
    }



    void traverseNLROnSplay(void (*func)(K key, V value)){
        splay->traverseNLR(func);
    }



    void clear(){
        delete avl;
        delete splay;
        avl = NULL;
        splay = NULL;
    }
    
//********************************************[TEST]*****************************************//

//*******************************************************************************************//
    


//*********************************** [SPLAY TREE] ************************************//
    class SplayTree {
    public:
//******************* [NODE] *******************//
        class Node {
            public: //Public-------
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent;

            typename AVLTree::Node* corr;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->corr = NULL;
            }

            ~Node(){
                delete entry;
            }
        };
//***********************************************//


    public:
        Node* root;

        SplayTree(): root(NULL) {};
        ~SplayTree() { this->clear(); };

//--Help method-------------------------------------------------------------
        void Lrotate(Node* x){
            Node* y = x->right;

            if (y) {
                x->right = y->left;
                if (y->left) y->left->parent = x;
                y->parent = x->parent;
            }

            if (!x->parent) root = y;
            else if (x == x->parent->left) x->parent->left = y;
            else x->parent->right = y;

            if (y) y->left = x;
            x->parent = y;
        }




        void Rrotate(Node* x){
            Node* y = x->left;

            if (y) {
                x->left = y->right;
                if (y->right) y->right->parent = x;
                y->parent = x->parent;
            }

            if (!x->parent) root = y;
            else if (x == x->parent->left) x->parent->left = y;
            else x->parent->right = y;

            if (y) y->right = x;
            x->parent = y;
        }




        void replace(Node *x, Node *y) {
            if (!x->parent) root = y;
            else if (x == x->parent->left) x->parent->left = y;
            else x->parent->right = y;

            if (y) y->parent = x->parent;
        }




        Node* maxLeftSubTree(Node *u){
            while (u->right) u = u->right;
            return u;
        }
        



        void splay(Node *x, int mode = 0) {
            while (x->parent) {
                if (!x->parent->parent) {
                    if (x->parent->left == x) Rrotate(x->parent);
                    else Lrotate(x->parent);
                } 
                //LL-----------------------
                else if (x->parent->left == x && x->parent->parent->left == x->parent) {
                    Rrotate(x->parent->parent);
                    Rrotate(x->parent);
                } 
                //RR----------------------
                else if (x->parent->right == x && x->parent->parent->right == x->parent) {
                    Lrotate(x->parent->parent);
                    Lrotate(x->parent);
                } 
                //RL----------------------
                else if (x->parent->left == x && x->parent->parent->right == x->parent) {
                    Rrotate(x->parent);
                    Lrotate(x->parent);
                } 
                //RR----------------------
                else {
                    Lrotate(x->parent);
                    Rrotate(x->parent);
                }
                //If mode != 0, splay one time
                if(mode != 0) break;
            }
        }





        Node* findKey(const K& key){
            Node* temp = root;
            while (temp){
                if(temp -> entry -> key > key) temp = temp -> left;
                else if(temp -> entry -> key < key) temp = temp -> right;
                else return temp;
            }
            return NULL; 
        }




        void deleteTree(Node *x){
            if(!x) return;
            deleteTree(x-> left);
            deleteTree(x-> right);
            delete x;
        }




        void travel(Node* curr, void (* func) (K key, V value)){
            if(!curr) return;
            func(curr->entry->key,curr->entry->value);
            travel(curr->left, func);
            travel(curr->right,func);
        }



//Main method ---------------------------------------------------------------------
        void add(Node* n){
            Node* temp = root;
            Node* p = NULL;

            while (temp){
                p = temp;
                if(temp -> entry -> key > n -> entry -> key) temp = temp -> left;
                else if(temp -> entry -> key < n->entry -> key) temp = temp -> right;
                else {
                    delete n;
                    throw "Duplicate key";
                    return;
                }
            } 

            temp = n;
            temp -> parent = p;

            if(!p) root = temp;
            else if(p -> entry -> key > temp -> entry -> key) p -> left = temp;
            else p -> right = temp;

            splay(temp);
        }




        void add(Entry* entry){
            Node* n = new Node(entry);
            add(n);
        }




        void add(K key, V value){
            Entry *e = new Entry(key, value);
            add(e); 
        }



        
        void remove(K key){
            Node* temp = findKey(key);

            if(!temp) {
                throw "Not found";
                return;
            }

            splay(temp);

            if(!temp -> left) replace(temp, temp -> right);
            else if(!temp -> right) replace(temp, temp -> left);
            else{
                Node* z = maxLeftSubTree(temp -> left);
                Node* rightSubTree = root -> right;
                replace(root, root -> left);
                splay(z);
                root -> right = rightSubTree;
                if(rightSubTree) rightSubTree -> parent = root;
            }

            delete temp;
        }





        V search(K key){
            Node* z = findKey(key);
            if(!z) throw "Not found";
            else {
                splay(z);
                return z->entry->value;
            }
        }




        void traverseNLR(void (*func)(K key, V value)){
            travel(root,func);
        }


        void clear(){
            deleteTree(root);
            root = NULL;
        }

//**************************************** [CHECK] **************************************//

//*****************************************************************************************//

    };
    
//***************************************** [AVL TREE] **********************************//
    class AVLTree {
    public:
//*************************** [NODE] ****************************//
        class Node {
            public: //public-------------------
            Entry* entry;
            Node* left;
            Node* right;
            int height;
            typename SplayTree::Node* corr;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->height = 1;
                this->corr = NULL;
            }

            ~Node(){
                delete entry;
            }
        };
//**************************************************************//

    public:
        Node* root;

        AVLTree(): root(NULL) {};
        ~AVLTree() { this->clear(); };


//--Help function--------------------------------------------------
        int max(int x, int y){
            return (x > y)? x : y;
        }



        int height(Node *x){
            if(!x) return 0;
            return x -> height;
        }



        int balance(Node *x){
            if(!x) return 0;
            return height(x->left) - height(x->right);
        }

        

        Node* Rrotate(Node* x){
            Node* temp = x -> left;
            if(temp) {
                x -> left = temp -> right;
                temp -> right = x;
                x->height = 1 + max(height(x-> left), height(x-> right));
                temp->height = 1 +max(height(temp -> left), height(temp-> right));
            }
            return temp;
        }




        Node* Lrotate(Node* x){
            Node* temp = x -> right;
            if(temp) {
                x -> right = temp -> left;
                temp -> left = x;
                x->height = 1 + max(height(x-> left), height(x-> right));
                temp->height = 1 +max(height(temp -> left), height(temp-> right));
            }
            return temp;
        }




        Node* maxLeftSubTree(Node *u){
            while (u->right) u = u->right;
            return u;
        }




        Node* insert(Node* n, Node* root){
            if(!root) return n;
            if(n->entry->key < root -> entry -> key){
                root -> left = insert(n, root->left);
            }
            else if(n->entry->key > root -> entry -> key){
                root -> right = insert(n, root->right);
            }
            else{
                delete n;
                throw "Duplicate key";
                return root;
            }

            root->height = 1 + max(height(root->left),height(root->right));
            int balan = balance(root);
            if(balan > 1){
                //LL-------------------
                if(n->entry-> key < root -> left -> entry -> key) return Rrotate(root);
                //LR-------------------
                else if(n->entry-> key > root -> left -> entry -> key) {
                    root->left = Lrotate(root->left);
                    return Rrotate(root);
                }
            }
            if(balan < -1){
                //RR------------------
                if(n->entry-> key > root -> right -> entry -> key) return Lrotate(root);
                //RL------------------
                if(n->entry-> key < root -> right -> entry -> key){
                    root->right = Rrotate(root->right);
                    return Lrotate(root);
                }
            }
            return root;
        }




        Node* deleteNode(Node* root, K key){
            if(!root) return root;
            if(key < root -> entry -> key) root -> left = deleteNode(root -> left, key);
            else if(key > root -> entry -> key) root -> right = deleteNode(root -> right, key);
            else{
                //One or No Child------------------------------------
                if(!(root -> left) || !(root -> right)){
                    Node* temp = (root->left)? root->left : root->right;
                    //No child------------------------------------
                    if(!temp){
                        temp = root;
                        root = NULL;
                    }
                    //One child-----------------------------------
                    else{
                        root -> left = temp -> left;
                        root -> right = temp -> right;
                        root -> entry -> key = temp -> entry -> key;
                        root -> entry -> value = temp -> entry -> value;
                        root -> corr = temp -> corr;
                        if(temp -> corr) (temp -> corr) -> corr = root;
                    }
                    delete temp;
                }
                //Two children-----------------------------------------
                else{
                    Node* temp = maxLeftSubTree(root -> left);
                    root -> entry -> key = temp -> entry -> key;
                    root -> entry -> value = temp -> entry -> value;
                    root -> corr = temp -> corr;
                    if(temp -> corr) (temp -> corr) -> corr = root;
                    root -> left = deleteNode(root -> left, temp -> entry -> key);
                }
            }

            if (!root) return root;

            root -> height = 1 + max(height(root -> left), height(root -> right));
            int balan = balance(root);
            if(balan > 1){
                //LL-------------------
                if(balance(root -> left) >= 0) return Rrotate(root);
                //LR-------------------
                else if(balance(root -> left) < 0) {
                    root->left = Lrotate(root->left);
                    return Rrotate(root);
                }
            }
            if(balan < -1){
                //RR------------------
                if(balance(root -> right) <= 0) return Lrotate(root);
                //RL------------------
                if(balance(root -> right) > 0){
                    root->right = Rrotate(root->right);
                    return Lrotate(root);
                }
            }
            return root;
        }




        Node* findKey(const K& key){
            Node* temp = root;
            while (temp){
                if(temp -> entry -> key > key) temp = temp -> left;
                else if(temp -> entry -> key < key) temp = temp -> right;
                else return temp;
            }
            return NULL; 
        }




        void deleteTree(Node *x){
            if(!x) return;
            deleteTree(x-> left);
            deleteTree(x-> right);
            delete x;
        }




        void travel(Node* curr, void (* func) (K key, V value)){
            if(!curr) return;
            func(curr->entry->key,curr->entry->value);
            travel(curr->left, func);
            travel(curr->right,func);
        }

        
//--Main function-------------------------------------------------------------------
        void add(Node* n){
            root = insert(n, root);
        }



        void add(Entry* entry){
            Node* n = new Node(entry);
            add(n);
        }




        void add(K key, V value){
            Entry* e = new Entry(key, value);
            add(e);
        }




        void remove(K key){
            Node* z = findKey(key);
            if(z) {
                root = deleteNode(root, key);
                return;
            }
            else throw "Not found";
        }




        V search(K key){
            Node* z = findKey(key);
            if(!z) throw "Not found";
            else return z->entry->value;
        }



        void traverseNLR(void (*func)(K key, V value)){
            travel(root,func);
        }



        void clear(){
            deleteTree(root);
            root = NULL;
        }

//*****************************************************[CHECK] *************************************//

//***************************************************************************************************//
    };
};

//*******************************************************************************//
