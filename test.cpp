#include "rdx_tree.cpp"

#include <iostream>
using namespace std;

int main() {
    RadixTree::rdx_tree rdx_tree;
    
    string insert_strs[] = {
        "sasdkj",
        "aasuab",
        "IasNJCjn",
        "3ja8sdh",
        "3jass",
        "aaasdsd"
    };
    string lookup_strs[] = {
        "sasdkj", // True
        "aasuab", // True
        "IasNJCjn", // True
        "3ja**dh", // False
        "aiss", // False
        "aaasdsd" // True
    };
    int size = 6;
    for(int i = 0; i < size; i++) {
        rdx_tree.insert(insert_strs[i]);
    }
    for(int i = 0; i < size; i++) {
        cout << (rdx_tree.lookup(lookup_strs[i]) ? "True" : "False") << endl;
    }
}