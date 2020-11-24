/*
    HCM University Of Science And Technology
    K19 - Computer Science
    DSA - Assignment 1 - Phase 1 - Fragment linked list
    Unit Test
    
    Note: 
    1. Include your FragmentLinkedList.cpp WITHOUT main function"
    2. Delete catch block in class method, just keep throw statement
*/
#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>

#include <cstring>
#include <assert.h>
/* ----------- Uncomment the following line ----------- */

#include "FragmentLinkedList.cpp"

using namespace std;

template <class T>
string fragmentHead(FragmentLinkedList<T> *pList, int fragmentMaxSize){
    stringstream ss;
    for(int i = 0; i < (pList->size() - 1) / fragmentMaxSize + 1; i++){
        ss << '|' << pList->get(i * fragmentMaxSize);
    }
    ss << '|';
    return ss.str();
}

#define expectedTest_1 "[9, 8, 7, 6, 5, 4, 3, 2, 1, 0]"
#define expectedTest_2 "[9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 0]"
#define expectedTest_3 "|9|5|1|"
#define expectedTest_4 "|9|11|2|"
#define expectedTest_5 "|12|6|3|0|"
#define expectedTest_6 "[8, 7, 6, 4, 3, 2, 0]"
#define expectedTest_7 "The index is out of range!"
#define expectedTest_8_0 "Dump"
#define expectedTest_8_1 "[8, 7, 6, 4, 3, 2, 0, -1]"
#define expectedTest_9 "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]"
#define expectedTest_10 "[0, 2, 4, 6, 8, 10, 12, 14, 16, 18]"
#define expectedTest_11 "[1, 5, 7, 11, 13, 17, 19]"
#define expectedTest_12 "[1, 5, 7, 11, 12, 16, 18]"
#define expectedTest_13 "[1, 5, 7, 11, 13, 17, 19, 21, 22]"
#define expectedTest_14 45

int main()
{
    std::out_of_range exOutBound("Dump");
    FragmentLinkedList<int> fList(4);
    // UnitTest 1
    for(int i = 0; i < 10; i++) fList.add(0, i);
    assert(expectedTest_1 == fList.toString());
    // UnitTest 2
    fList.add(9, 10);
    assert(expectedTest_2 == fList.toString());
    //Tescase 3
    assert(expectedTest_3 == fragmentHead(&fList, 4));  
    // Tescase 4
    fList.add(4, 11);
    assert(expectedTest_4 == fragmentHead(&fList, 4));
    // Tescase 5
    fList.add(0, 12);
    assert(expectedTest_5 == fragmentHead(&fList, 4));
    // UnitTest 6
    for(int i = 10; i <= 12; i++) fList.removeItem(i);
    for(int i = 0; i < 3; i++) fList.removeAt(i * 3);
    assert(expectedTest_6 == fList.toString());
    // UnitTest 7
    try{
        fList.add(8, -1);
    }
    catch(std::out_of_range ex){
        exOutBound = ex;
    }
    assert(!strcmp(expectedTest_7, exOutBound.what()));
    exOutBound = std::out_of_range("Dump");
    // UnitTest 8
    try{
        fList.add(7, -1);
    }
    catch(std::out_of_range ex){
        exOutBound = ex;
    }
    assert(!strcmp(expectedTest_8_0, exOutBound.what()));
    assert(expectedTest_8_1 == fList.toString());
    // UnitTest 9
    fList.clear();
    for(int i = 0;i < 10; i++) fList.add(i);
    for(int i = 0; i < 10; i++) fList.add(fList.indexOf(i) + 1,-1);
    while(fList.contains(-1)) fList.removeItem(-1);
    assert(expectedTest_9 == fList.toString());
    //UnitTest 10
    for(FragmentLinkedList<int>::Iterator v = fList.begin(); v != fList.end(); v++) 
        (*v)*=2;
    assert(expectedTest_10 == fList.toString());
    //Tescase 11
    for(FragmentLinkedList<int>::Iterator v = fList.begin(); v != fList.end(); v++) v.set(*v + 1);
    for(FragmentLinkedList<int>::Iterator v = fList.begin(); v != fList.end(); v++)
        if(*v % 3 == 0) v.remove();
    assert(expectedTest_11 == fList.toString());
    //Tescase 12
    for(FragmentLinkedList<int>::Iterator v = fList.begin(1); v != fList.end(); v++) (*v)--;
    assert(expectedTest_12 == fList.toString());
    //UnitTest 13
    fList.add(20);
    fList.add(22);
    for(FragmentLinkedList<int>::Iterator v = fList.begin(1); v != fList.end(1); v++) (*v)++;
    assert(expectedTest_13 == fList.toString());
    //UnitTest 14
    for(FragmentLinkedList<int>::Iterator v = (++fList.begin()); v != fList.end(); v++) v.remove();
    for(int i = 2; i < 10; i++) fList.add(i);
    int total = 0;
    while(!fList.empty()) total += fList.removeAt(0);
    assert(expectedTest_14 == total);
    cout << "Completed! Weldone!";
}