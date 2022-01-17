#include "rdx_tree.cpp"

#define TEST_FINDSUCCESSOR
#define TEST_FINDSUCCESSOR_AUTO

#include <iostream>
#include <fstream>
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
    srand(time(NULL));
#ifdef TEST_INSERT_LOOKUP_REMOVE

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
    set <string> check_helper;

    string word;
#ifdef TEST_FINDSUCCESSOR_AUTO
    constexpr size_t checks = 1e3;

    for(size_t i = 0; i < checks; i++) {
        word = getRandomString();
        if(0 == rand() % 5) {
            // insert
            rdx_tree.insert(word);
            check_helper.insert(word);
        }
        else if(0 == rand() % 4) {
            // delete
            for(auto &_word: check_helper) {
                if(0 == rand() % 3) {
                    word = _word;
                }
            }
            rdx_tree.remove(word);
            check_helper.erase(word);
        }
        else {
            string checker;
            for(auto &_word: check_helper) {
                if(_word > word) {
                    checker = _word;
                    break;
                }
            }
            if(checker != rdx_tree.findSuccessor(word)) {
                cout << "Test Failed!" << endl;
            }
        }
    }
#else // TEST_FINDSUCCESSOR_INPUT
    while(cin >> word) {
        // cout << rdx_tree.findSuccessor(word) << endl;
        if(word[0] == '#') {
            rdx_tree.insert(word.substr(1, word.length()));
            check_helper.insert(word.substr(1, word.length()));
        }
        else if(word[0] == '!') {
            rdx_tree.remove(word.substr(1, word.length()));
            check_helper.erase(word.substr(1, word.length()));
        }
        else {
            cout << "Rdx: " << rdx_tree.findSuccessor(word) << endl;
            string checker;
            for(auto &_word: check_helper) {
                if(_word > word) {
                    // cout << "Ckr: " << _word << endl;
                    checker = _word;
                    break;
                }
            }
            cout << "Ckr: " << checker << endl;
        }
    }
    /*
    Real-time Content:
        #abcc
        #abccd
        #bcvsd 
        > #lask
        > #njnj
        #lasddfdf
        > !njnj
        > !lask
        #lassk
        #njn

        >> jn
    */
#endif // TEST_FINDSUCCESSOR_INPUT
#endif // TEST_FINDSUCCESSOR
#ifdef TEST_BENCHMARK
    srand(time(NULL));

    RadixTree::rdx_tree rdx_tree;
    set <string> check_helper;

    constexpr size_t inserts = 1e4, lookups = 1e4, removes = 5e3;
    string word;

#ifdef TEST_BENCHMARK_MAKEDATA
    ofstream of_data("data.txt");
    
    for(int i = 0; i < inserts; i++) {
        word = getRandomString();
        // rdx_tree.insert(word), check_helper.insert(word);
        check_helper.insert(word);
        of_data << word << endl;
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
        // if(rdx_tree.lookup(word) != check_helper.count(word)) {
        //     cout << "Insert Test Failed!" << endl;
        // }
        of_data << word << endl;
    }

    for(int i = 0; i < removes; i++) {
        word = getRandomString();
        if(0 != rand() % 4) {
            for(auto &str: check_helper) {
                if(0 == rand() % 4) {
                    word = str;
                    break;
                }
            }
        }
        // rdx_tree.remove(word), check_helper.erase(word);
        // if(rdx_tree.lookup(word) != check_helper.count(word)) {
        //     cout << "Remove Test Failed!" << endl;
        // }
        check_helper.erase(word);
        of_data << word << endl;
    }

    of_data.close();
#endif // TEST_BENCHMARK_MAKEDATA

    ifstream if_data("data.txt");

    check_helper.clear();

    time_t check_helper_start = clock();

    for(int i = 0; i < inserts; i++) {
        if_data >> word;
        check_helper.insert(word);
    }
    for(int i = 0; i < lookups; i++) {
        if_data >> word;
        check_helper.count(word);
    }

    for(int i = 0; i < removes; i++) {
        if_data >> word;
        check_helper.erase(word);
    }
    
    time_t check_helper_end = clock();

    if_data.open("data.txt");

    time_t rdx_tree_start = clock();

    for(int i = 0; i < inserts; i++) {
        if_data >> word;
        rdx_tree.insert(word);
    }
    for(int i = 0; i < lookups; i++) {
        if_data >> word;
        rdx_tree.lookup(word);
    }

    for(int i = 0; i < removes; i++) {
        if_data >> word;
        rdx_tree.remove(word);
    }

    time_t rdx_tree_end = clock();

    if_data.close();

    ofstream logger("log_3_v2.0.txt");

    cout << "Test Finished!" << endl;
    logger << "check_helper costs: " << check_helper_end - check_helper_start << "ms" << endl;
    logger << "rdx_tree costs: " << rdx_tree_end - rdx_tree_start << "ms" << endl;
#endif // TEST_BENCHMARK
}