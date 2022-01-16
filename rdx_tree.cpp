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

        size_t matchingConsecutiveCharacter(const std::string &word, size_t offset, const rdx_node &node);
        void insertRec(const std::string &word, size_t offset, rdx_node &node);
        bool lookupRec(const std::string &word, size_t offset, const rdx_node &node);
        void removeRec(const std::string &word, size_t offset, rdx_node &node);
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
}