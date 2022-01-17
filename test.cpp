#include "rdx_tree.cpp"

#define TEST_FINDSUCCESSOR

#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
using namespace std;

constexpr size_t MAXLEN = 100;

string getRandomString() {
    const string alphabet = "qwertyuioplkjhgfdsazxcvbnm1234567890!@#$%^&*()_+-=/.,<>?;':[]{}|";
    size_t len = rand() % 100, ab_len = alphabet.length();

    string rand_str;
    do {
        rand_str.push_back(alphabet[rand() % ab_len]);
    } while(len--);
    return rand_str;
}

int main() {
#ifdef TEST_INSERT_LOOKUP_REMOVE
    srand(time(NULL));

    RadixTree::rdx_tree rdx_tree;
    set <string> check_helper;
    
    constexpr size_t inserts = 100, lookups = 100, removes = 50;
    string word;
    for(int i = 0; i < inserts; i++) {
        word = getRandomString();
        rdx_tree.insert(word), check_helper.insert(word);
    }
    for(int i = 0; i < lookups; i++) {
        word = getRandomString();
        if(0 == rand() % 2) {
            for(auto &str: check_helper) {
                if(0 == rand() % 3) {
                    word = str;
                }
            }
        }
        if(rdx_tree.lookup(word) != check_helper.count(word)) {
            cout << "Insert Test Failed!" << endl;
        }
    }

    for(int i = 0; i < lookups; i++) {
        word = getRandomString();
        if(0 != rand() % 4) {
            for(auto &str: check_helper) {
                if(0 == rand() % 4) {
                    word = str;
                    break;
                }
            }
        }
        rdx_tree.remove(word), check_helper.erase(word);
        if(rdx_tree.lookup(word) != check_helper.count(word)) {
            cout << "Remove Test Failed!" << endl;
        }
    }
    cout << "Test Finished!" << endl;
#endif // TEST_INSERT_LOOKUP_REMOVE
#ifdef TEST_FINDSUCCESSOR
    RadixTree::rdx_tree rdx_tree;

    rdx_tree.insert("aa");
    rdx_tree.insert("aaa");
    rdx_tree.insert("aac");
    rdx_tree.insert("aabbbbb");
    rdx_tree.insert("aad");
    rdx_tree.remove("aac");
    /*
        Now rdx_tree:
            aa
            aaa
            aabbbbb
            aad
    */
    string word;
    while(cin >> word) {
        cout << rdx_tree.findSuccessor(word) << endl;
    }
#endif // TEST_FINDSUCCESSOR
}