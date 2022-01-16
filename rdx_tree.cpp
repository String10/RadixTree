#include "rdx_node.cpp"

namespace RadixTree {
    class rdx_tree {
    public:
        rdx_tree();
        void insert(const std::string &word);
        bool lookup(const std::string &word);
        void remove(const std::string &word);
    private:
        rdx_node root;
    };
}