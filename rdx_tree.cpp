#include "rdx_node.cpp"

namespace RadixTree {
    class rdx_tree {
    public:
        rdx_tree();
        void insert(const std::string &word);
        bool lookup(const std::string &word);
        void remove(const std::string &word);
        std::string findSuccessor(const std::string &word);
        std::string findPredecessor(const std::string &word);
    private:
        rdx_node root;

        size_t matchingConsecutiveCharacter(const std::string &word, size_t offset, const rdx_node &node);
        void insertRec(const std::string &word, size_t offset, rdx_node &node);
        bool lookupRec(const std::string &word, size_t offset, const rdx_node &node);
        void removeRec(const std::string &word, size_t offset, rdx_node &node);
        std::string findSuccessorRec(const std::string &word, size_t offset, const rdx_node &node);
        std::string findPredecessorRec(const std::string &word, size_t offset, const rdx_node &node);
        std::string getMinSuffix(const rdx_node &node);
        std::string getMaxSuffix(const rdx_node &node);
    };

    rdx_tree::rdx_tree(): root() {}
    void rdx_tree::insert(const std::string &word) {
        insertRec(word, 0, root);
    }
    void rdx_tree::insertRec(const std::string &word, size_t offset, rdx_node &node) {
        static size_t matches;
        matches = matchingConsecutiveCharacter(word, offset, node);
        if((matches == 0) || (matches > 0 && offset + matches < word.length() && matches >= node.label.length())) {
            /**
             * @brief case 0:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|
             * 
             */
            offset += matches;
            bool inserted = false;
            for(auto &sub_node: node.sub_nodes) {
                if(sub_node.label[0] == word[offset]) {
                    insertRec(word, offset, sub_node);
                    inserted = true;
                }
            }
            if(!inserted) {
                node.sub_nodes.push_back(rdx_node(word.substr(offset, word.length())));
                node.sub_nodes.back().is_end_of_node = true;
            }
        }
        else if(offset + matches >= word.length() && matches < node.label.length()) {
            /**
             * @brief case 1:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|-------------|
             * 
             */
            rdx_node new_node(word.substr(offset, matches));
            new_node.is_end_of_node = true;
            node.label = node.label.substr(matches, node.label.length());
            new_node.sub_nodes.push_back(node);
            node = new_node;
        }
        else if(offset + matches >= word.length() && matches == node.label.length()) {
            /**
             * @brief case 2:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|
             * 
             */
            node.is_end_of_node = true;
        }
        else {
            /**
             * @brief case 3:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|-------------|
             * 
             */
            rdx_node new_node(word.substr(offset, matches));
            new_node.sub_nodes.push_back(rdx_node(word.substr(offset + matches, word.length())));
            new_node.sub_nodes.back().is_end_of_node = true;
            node.label = node.label.substr(matches, node.label.length());
            new_node.sub_nodes.push_back(node);
            node = new_node;
        }
    }
    size_t rdx_tree::matchingConsecutiveCharacter(const std::string &word, size_t offset, const rdx_node &node) {
        static size_t matches;
        for(matches = 0; matches < node.label.length() && offset < word.length(); offset++, matches++) {
            if(word[offset] != node.label[matches]) {
                return matches;
            }
        }
        return matches;
    }
    bool rdx_tree::lookup(const std::string &word) {
        return lookupRec(word, 0, root);
    }
    bool rdx_tree::lookupRec(const std::string &word, size_t offset, const rdx_node &node) {
        static size_t matches;
        matches = matchingConsecutiveCharacter(word, offset, node);
        if((matches == 0) || (matches > 0 && offset + matches < word.length() && matches >= node.label.length())) {
            /**
             * @brief case 0:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|
             * 
             */
            offset += matches;
            for(auto &sub_node: node.sub_nodes) {
                if(sub_node.label[0] == word[offset]) {
                    return lookupRec(word, offset, sub_node);
                }
            }
            return false;
        }
        else if(offset + matches >= word.length() && matches < node.label.length()) {
            /**
             * @brief case 1:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|-------------|
             * 
             */
            return false;
        }
        else if(offset + matches >= word.length() && matches == node.label.length()) {
            /**
             * @brief case 2:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|
             * 
             */
            return node.is_end_of_node;
        }
        else {
            /**
             * @brief case 3:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|-------------|
             * 
             */
            return false;
        }
    }
    void rdx_tree::remove(const std::string &word) {
        removeRec(word, 0, root);
    }
    void rdx_tree::removeRec(const std::string &word, size_t offset, rdx_node &node) {
        static size_t matches;
        matches = matchingConsecutiveCharacter(word, offset, node);
        if((matches == 0) || (matches > 0 && offset + matches < word.length() && matches >= node.label.length())) {
            /**
             * @brief case 0:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|
             * 
             */
            offset += matches;
            for(auto &sub_node: node.sub_nodes) {
                if(sub_node.label[0] == word[offset]) {
                    removeRec(word, offset, sub_node);
                }
            }
        }
        else if(offset + matches >= word.length() && matches < node.label.length()) {
            /**
             * @brief case 1:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|-------------|
             * 
             */
        }
        else if(offset + matches >= word.length() && matches == node.label.length()) {
            /**
             * @brief case 2:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|
             * 
             */
            node.is_end_of_node = false;
        }
        else {
            /**
             * @brief case 3:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|-------------|
             * 
             */
        }
    }
    std::string rdx_tree::findSuccessor(const std::string &word) {
        return findSuccessorRec(word, 0, root);
    }
    std::string rdx_tree::findSuccessorRec(const std::string &word, size_t offset, const rdx_node &node) {
        // FIXME: Input "aabbbbb", should return "aad", but gets "".
        // The bug raises when finding the min-sub-node of the node, if the one didn't got a valid word,
        // the program won't continue to find the second-min-sub-node, but return "".
        static size_t matches;
        matches = matchingConsecutiveCharacter(word, offset, node);
        if((matches == 0) || (matches > 0 && offset + matches < word.length() && matches >= node.label.length())) {
            /**
             * @brief case 0:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|
             * 
             */
            offset += matches;
            const rdx_node *p_min_node = nullptr;
            for(auto &sub_node: node.sub_nodes) {
                if(sub_node.label[0] == word[offset]) {
                    std::string ret = findSuccessorRec(word, offset, sub_node);
                    if(ret != "") {
                        return ret;
                    }
                }
                if(sub_node.label[0] > word[offset]) {
                    if(p_min_node == nullptr || p_min_node->label[0] > sub_node.label[0]) {
                        p_min_node = &sub_node;
                    }
                }
            }
            if(p_min_node == nullptr) {
                return "";
            }
            else {
                std::string successor = word.substr(0, offset);
                while(p_min_node != nullptr) {
                    successor.append(p_min_node->label);
                    if(p_min_node->is_end_of_node) {
                        return successor;
                    }
                    const rdx_node *p_min_sub_node = nullptr;
                    for(auto &sub_node: p_min_node->sub_nodes) {
                        if(p_min_sub_node == nullptr || p_min_sub_node->label[0] > sub_node.label[0]) {
                            p_min_sub_node = &sub_node;
                        }
                    }
                    p_min_node = p_min_sub_node;
                }
                return "";
            }
        }
        else if(offset + matches >= word.length() && matches < node.label.length()) {
            /**
             * @brief case 1:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|-------------|
             * 
             */
            // return word.substr(0, offset) + node.label;
            const rdx_node *p_min_node = &node;
            std::string successor = word.substr(0, offset);
            while(p_min_node != nullptr) {
                successor.append(p_min_node->label);
                if(p_min_node->is_end_of_node) {
                    return successor;
                }
                const rdx_node *p_min_sub_node = nullptr;
                for(auto &sub_node: p_min_node->sub_nodes) {
                    if(p_min_sub_node == nullptr || p_min_sub_node->label[0] > sub_node.label[0]) {
                        p_min_sub_node = &sub_node;
                    }
                }
                p_min_node = p_min_sub_node;
            }
            return "";
        }
        else if(offset + matches >= word.length() && matches == node.label.length()) {
            /**
             * @brief case 2:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|
             * 
             */
            const rdx_node *p_min_node = nullptr;
            for(auto &sub_node: node.sub_nodes) {
                if(p_min_node == nullptr || p_min_node->label[0] > sub_node.label[0]) {
                    p_min_node = &sub_node;
                }
            }
            if(p_min_node == nullptr) {
                return "";
            }
            else {
                std::string successor = word;
                while(p_min_node != nullptr) {
                    successor.append(p_min_node->label);
                    if(p_min_node->is_end_of_node) {
                        return successor;
                    }
                    const rdx_node *p_min_sub_node = nullptr;
                    for(auto &sub_node: p_min_node->sub_nodes) {
                        if(p_min_sub_node == nullptr || p_min_sub_node->label[0] > sub_node.label[0]) {
                            p_min_sub_node = &sub_node;
                        }
                    }
                    p_min_node = p_min_sub_node;
                }
                return "";
            }
        }
        else {
            /**
             * @brief case 3:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|-------------|
             * 
             */
            if(node.label[matches] < word[offset + matches]) {
                return "";
            }
            else {
                // return word.substr(0, offset) + node.label;
                const rdx_node *p_min_node = &node;
                std::string successor = word.substr(0, offset);
                while(p_min_node != nullptr) {
                    successor.append(p_min_node->label);
                    if(p_min_node->is_end_of_node) {
                        return successor;
                    }
                    const rdx_node *p_min_sub_node = nullptr;
                    for(auto &sub_node: p_min_node->sub_nodes) {
                        if(p_min_sub_node == nullptr || p_min_sub_node->label[0] > sub_node.label[0]) {
                            p_min_sub_node = &sub_node;
                        }
                    }
                    p_min_node = p_min_sub_node;
                }
                return "";
            }
        }
    }
}