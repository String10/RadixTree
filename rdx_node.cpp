#include <string>
// #include <vector>
#include <map>

namespace RadixTree {
    struct rdx_node {
        std::string label;
        bool is_end_of_node;
        /* DONE: Use rdx_node* instead of rdx_node to avoid recusive copy. */
        /* DONE: Use smarter data structrue to find next node faster. */
        // std::vector <rdx_node> sub_nodes;
        std::map <char, rdx_node*> sub_nodes;
        rdx_node(): label(""), is_end_of_node(false) {
            sub_nodes.clear();
        }
        rdx_node(std::string _label): label(_label), is_end_of_node(false) {
            sub_nodes.clear();
        }
        ~rdx_node() {
            for(auto &sub_node: sub_nodes) {
                if(sub_node.second != nullptr) {
                    delete sub_node.second;
                    sub_node.second = nullptr;
                }
            }
        }
    };
}