#include <string>
#include <vector>

namespace RadixTree {
    struct rdx_node {
        std::string label;
        bool isEndofWord;
        std::vector <rdx_node> subNodes;
        rdx_node(): label(""), isEndofWord(false) {
            subNodes.clear();
        }
        rdx_node(std::string _label): label(_label), isEndofWord(false) {
            subNodes.clear();
        }
    };
}