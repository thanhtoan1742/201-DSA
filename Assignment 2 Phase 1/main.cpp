#include "BKUTree.hpp"
#include <iostream>

using namespace std;

void printKey(int key, int value) {
    cout << key << '\n';
}

void test1() {
    BKUTree<int, int>* tree = new BKUTree<int, int>();
    int keys[] = {1, 3, 5, 7, 9, 2, 4};
    for (int i = 0; i < 7; i++) tree->add(keys[i], keys[i]);
    tree->traverseNLROnAVL(printKey);
    delete tree;
}

void test2() {
    BKUTree<int, int>* tree = new BKUTree<int, int>();
    int keys[] = {1, 3, 5, 7, 9, 2, 4};
    for (int i = 0; i < 7; i++) tree->add(keys[i], keys[i]);
    tree->traverseNLROnSplay(printKey);
    delete tree;
}

int main() {
    test1();

    cout << "----------------\n";

    test2();
}