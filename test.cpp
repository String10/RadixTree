#include "rdx_tree.cpp"

#include <iostream>
using namespace std;

int main() {
    RadixTree::rdx_tree rdx_tree;
    
    rdx_tree.insert("abcd");
    rdx_tree.insert("ab");
    rdx_tree.insert("cd");
    rdx_tree.insert("cd");
    rdx_tree.insert("abcddd");
    rdx_tree.insert("celll");
    rdx_tree.insert("k");
}