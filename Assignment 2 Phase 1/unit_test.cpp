// include your BKUTree class here
// #include "BKUTree.hpp"
#include "BKUTree.cpp"
const char* ansFile = "ans.txt";
const char* outFile = "out.txt";

#include <iostream>
#include <sstream>
#include <functional>
#include <assert.h>
#include <cstdlib>
#include <unordered_map>
#include <fstream>
#include <string>

using namespace std;

template<class K, class V>
void print(K key, V value) {
    cout << key << ':' << value << ' ';
}

template<class K, class V>
void print_splay(typename BKUTree<K, V>::SplayTree* splay) {
    cout << "Splay: ";
    splay->traverseNLR(print);
    cout << "\n";
}

template<class K, class V>
void print_avl(typename BKUTree<K, V>::AVLTree* avl) {
    cout << "AVL: ";
    avl->traverseNLR(print);
    cout << "\n";
}

template<class K, class V>
void print_bku(BKUTree<K, V>* bku) {
    cout << "Splay: ";
    bku->traverseNLROnSplay(print);
    cout << "\n";
    cout << "AVL: ";
    bku->traverseNLROnAVL(print);
    cout << "\n";
}

string make_string_value(int n) {
    string value = to_string(n%1000);
    for (int i = 0; i < (int)value.size(); ++i)
        value[i] += 'a' - '0';
    return value;
}

void print_header(string header) {
    static bool first = 1;
    if (first) 
        first = 0;
    else 
        cout << "\n\n\n\n\n\n";
    cout << "____________________________________________";
    cout << header;
    cout << "__________________________________________________________________________________________\n";
}

void example_test() {
    print_header("EXAMPLE TEST");

    BKUTree<int, int>* tree = new BKUTree<int, int>();
    int keys[] = {1, 3, 5, 7, 9, 2, 4};
    for (int i = 0; i < 7; i++) tree->add(keys[i], keys[i]);
    print_bku(tree);
    delete tree;
}


void bku_add_test() {
    print_header("BKU ADD TEST");

    BKUTree<int, string>* bku = new BKUTree<int, string>(4);

    try {
        // Base case
        bku->add(10, "1st");
        print_bku(bku);
        bku->add(12, "2nd");
        print_bku(bku);
        bku->add(8, "3rd");
        print_bku(bku);
        bku->add(73, "4th");
        print_bku(bku);
        bku->add(42, "5th");
        print_bku(bku);
        bku->add(3, "6th");
        print_bku(bku);
        bku->add(39, "7th");
        print_bku(bku);
        bku->add(27, "8th");
        print_bku(bku);

        // Duplicate key
        bku->add(8, "Dup");
        print_bku(bku);
    }
    catch (const char* e) {
        cout << e << '\n';
    }

    // after exception, keep adding is fine.
    bku->add(38, "9th");
    print_bku(bku);
    bku->add(65, "10th");
    print_bku(bku);

    // Same value is fine.
    bku->add(7, "3rd");
    print_bku(bku);
    bku->add(29, "10th");
    print_bku(bku);

    // Duplicate again.
    try {
        bku->add(29, "11th");
        print_bku(bku);
    }
    catch (const char* e) {
        cout << e << '\n';
    }
    print_bku(bku);

    delete bku;
}

void random_test_bku() {
    print_header("RANDOM BKU TEST");

    srand(3301);    
    BKUTree<int, string>* bku = new BKUTree<int, string>();
    
    int n = 1e3;
    int clear_rate = 100;
    for (int i = 0; i < n; ++i) {
        int key = rand() % n;
        cerr << key << '\n';

        try {
            vector<int> tl;
            string value = bku->search(key, tl);

            cout << value << "\nTraversed List: ";
            for (int e: tl)
                cout << e << " ";
            cout << '\n';

            bku->remove(key);
        }
        catch (const char* e) {
            string value = make_string_value(rand());        
            bku->add(key, value);
        }

        print_bku(bku);
        if (rand() % clear_rate == 0) 
            bku->clear();
    }

    delete bku;
}

void random_test_splay() {
    print_header("RANDOM SPLAY TEST");

    srand(3301);    
    BKUTree<int, string>::SplayTree* splay = new BKUTree<int, string>::SplayTree();
    
    int n = 1e3;
    int clear_rate = 100;
    for (int i = 0; i < n; ++i) {
        int key = rand() % n;

        try {
            vector<int> tl;
            string value = splay->search(key);
            cout << value << "\n";
            splay->remove(key);
        }
        catch (const char* e) {
            string value = make_string_value(rand());        
            splay->add(key, value);
        }

        print_splay<int, string>(splay);
        if (rand() % clear_rate == 0) 
            splay->clear();
    }

    delete splay;
}

void random_test_avl() {
    print_header("RANDOM AVL TEST");

    srand(3301);    
    BKUTree<int, string>::AVLTree* avl = new BKUTree<int, string>::AVLTree();
    
    int n = 1e3;
    int clear_rate = 100;
    for (int i = 0; i < n; ++i) {
        int key = rand() % n;

        try {
            vector<int> tl;
            string value = avl->search(key);
            cout << value << "\n";
            avl->remove(key);
        }
        catch (const char* e) {
            string value = make_string_value(rand());        
            avl->add(key, value);
        }

        print_avl<int, string>(avl);
        if (rand() % clear_rate == 0) 
            avl->clear();
    }

    delete avl;
}

void avl_remove_test() {
    print_header("AVL REMOVE TEST");
    BKUTree<int, string>::AVLTree* avl = new BKUTree<int, string>::AVLTree();

    avl->add(10, "1st");
    avl->add(12, "2nd");
    avl->add(8, "3rd");
    avl->add(73, "4th");
    avl->add(42, "5th");
    avl->add(3, "6th");
    avl->add(39, "7th");
    avl->add(27, "8th");
    print_avl<int, string>(avl);

    avl->remove(8);
    print_avl<int, string>(avl);
    avl->remove(42);
    print_avl<int, string>(avl);
    avl->remove(27);
    print_avl<int, string>(avl);

    try {
        avl->remove(100);
        print_avl<int, string>(avl);
    }
    catch (const char* e) {
        cout << e << '\n';
    }

    try {
        avl->remove(42);
        print_avl<int, string>(avl);
    }
    catch (const char* e) {
        cout << e << '\n';
    }

    delete avl;
}

void run_test() {
    freopen(outFile, "w", stdout);

    example_test();

    bku_add_test();
    // bku_remove_test();
    // bku_search_test();
    
    // splay_add_test();
    // splay_remove_test();
    // splay_search_test();

    // avl_add_test();
    avl_remove_test();
    // avl_search_test();

    random_test_bku();
    random_test_splay();
    random_test_avl();
}

bool is_white_space(char c) {
    static char ws[] = {' ', '\n', '\r'};
    static int n = sizeof(ws);

    for (int i = 0; i < n; ++i)
        if (ws[i] == c)
            return 1;
    return 0;
}

void trim(string& s) {
    if (s.empty())
        return;

    while (is_white_space(s.back())) 
        s.pop_back();

    while (s.size() > 1 && is_white_space(s[0]))
        s = s.substr(1);
}

void compare() {
    ifstream ansf(ansFile);
    ifstream outf(outFile);

    int i = 1;
    while (!ansf.eof()) {
        string ans;
        getline(ansf, ans);
        trim(ans);

        if (outf.eof()) {
            cerr << outFile << " is shorter than " << ansFile;
            return;
        }

        string out;
        getline(outf, out);
        trim(out);

        if (out != ans) {
            cerr << "Different at line " << i << ":\n";
            cerr << ans << "\n" << out << '\n';
            return;
        }
        ++i;
    }

    if (!outf.eof()) {
        cerr << ansFile << " is shorter than " << outFile;
        return;
    }
}

int main() {
    run_test();
    // compare();
}
