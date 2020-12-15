#include "BKUTree.hpp"
#include <iostream>

using namespace std;

void printKey(int k, int v) {
    cout << k << '\n';
}

int main() {
try
    {
        BKUTree<int, int> bku;
        BKUTree<int, int>::AVLTree avl;
        BKUTree<int, int>::SplayTree splay;
        vector<int> traverse_list;
       
            int arr[] = {1,6,7,4,3,11,5,8,10,9};
            //int arr[] = { 5, 10, 15, 1, 2, 3, 6, 100, 50, 20, 40, 16, 17, 1000, 0, 500, 21, 400, 22, 23, 24, 5000, 9 };
            for (int i = 0; i < (int)sizeof(arr) / 4; i++)
            {
                avl.add(arr[i], arr[i]);
                bku.add(arr[i], arr[i]);
                splay.add(arr[i], arr[i]);
            }
            bku.remove(4);
         //   bku.traverseNLROnAVL(printKey);
         //   cout << endl;
            bku.traverseNLROnSplay(printKey);
            cout << endl;
        
       
    }
    catch (...)
    {
    }
}
