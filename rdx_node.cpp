#include <string>
#include <vector>

namespace RadixTree {
    struct rdx_node {
        std::string label;
        bool is_end_of_node;
        std::vector <rdx_node> sub_nodes;
        rdx_node(): label(""), is_end_of_node(false) {
            sub_nodes.clear();
        }
        rdx_node(std::string _label): label(_label), is_end_of_node(false) {
            sub_nodes.clear();
        }
    };
}