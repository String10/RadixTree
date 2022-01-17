#include "rdx_node.cpp"

namespace RadixTree {
    class rdx_tree {
    public:
        rdx_tree();
        ~rdx_tree();
        void insert(const std::string &word);
        bool lookup(const std::string &word);
        void remove(const std::string &word);
        std::string findSuccessor(const std::string &word);
        std::string findPredecessor(const std::string &word);
    private:
        // rdx_node root;
        rdx_node *root;

        size_t matchingConsecutiveCharacter(const std::string &word, size_t offset, const rdx_node *node);
        void insertRec(const std::string &word, size_t offset, rdx_node *node);
        bool lookupRec(const std::string &word, size_t offset, const rdx_node *node);
        void removeRec(const std::string &word, size_t offset, rdx_node *node);
        std::string findSuccessorRec(const std::string &word, size_t offset, const rdx_node *node);
        std::string findPredecessorRec(const std::string &word, size_t offset, const rdx_node *node);
        std::string getMinSuffix(const rdx_node *node);
        std::string getMaxSuffix(const rdx_node *node);
    };

    rdx_tree::rdx_tree() {
        root = new rdx_node;
    }
    rdx_tree::~rdx_tree() {
        delete root;
        root = nullptr;
    }
    void rdx_tree::insert(const std::string &word) {
        insertRec(word, 0, root);
    }
    void rdx_tree::insertRec(const std::string &word, size_t offset, rdx_node *node) {
        static size_t matches;
        matches = matchingConsecutiveCharacter(word, offset, node);
        if((matches == 0) || (matches > 0 && offset + matches < word.length() && matches >= node->label.length())) {
            /**
             * @brief case 0:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|
             * 
             */
            offset += matches;
            if(node->sub_nodes.count(word[offset]) && node->sub_nodes[word[offset]] != nullptr) {
                insertRec(word, offset, node->sub_nodes[word[offset]]);
            }
            else {
                node->sub_nodes[word[offset]] = new rdx_node(word.substr(offset, word.length()));
                node->sub_nodes[word[offset]]->is_end_of_node = true;
            }
        }
        else if(offset + matches >= word.length() && matches < node->label.length()) {
            /**
             * @brief case 1:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|-------------|
             * 
             */
            rdx_node *p_new_node = new rdx_node(node->label.substr(matches, node->label.length()));
            p_new_node->is_end_of_node = node->is_end_of_node;
            p_new_node->sub_nodes.swap(node->sub_nodes);

            node->label = node->label.substr(0, matches);
            node->is_end_of_node = true;
            node->sub_nodes[p_new_node->label[0]] = p_new_node;
        }
        else if(offset + matches >= word.length() && matches == node->label.length()) {
            /**
             * @brief case 2:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|
             * 
             */
            node->is_end_of_node = true;
        }
        else {
            /**
             * @brief case 3:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|-------------|
             * 
             */
            rdx_node *p_new_node = new rdx_node(node->label.substr(matches, node->label.length()));
            p_new_node->is_end_of_node = node->is_end_of_node;
            p_new_node->sub_nodes.swap(node->sub_nodes);

            node->label = node->label.substr(0, matches);
            node->is_end_of_node = false;
            node->sub_nodes[p_new_node->label[0]] = p_new_node;

            p_new_node = new rdx_node(word.substr(offset + matches, word.length()));
            p_new_node->is_end_of_node = true;
            node->sub_nodes[p_new_node->label[0]] = p_new_node;
        }
    }
    size_t rdx_tree::matchingConsecutiveCharacter(const std::string &word, size_t offset, const rdx_node *node) {
        static size_t matches;
        for(matches = 0; matches < node->label.length() && offset < word.length(); offset++, matches++) {
            if(word[offset] != node->label[matches]) {
                return matches;
            }
        }
        return matches;
    }
    bool rdx_tree::lookup(const std::string &word) {
        return lookupRec(word, 0, root);
    }
    bool rdx_tree::lookupRec(const std::string &word, size_t offset, const rdx_node *node) {
        static size_t matches;
        matches = matchingConsecutiveCharacter(word, offset, node);
        if((matches == 0) || (matches > 0 && offset + matches < word.length() && matches >= node->label.length())) {
            /**
             * @brief case 0:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|
             * 
             */
            offset += matches;
            if(node->sub_nodes.count(word[offset]) != 0 && node->sub_nodes.at(word[offset]) != nullptr) {
                return lookupRec(word, offset, node->sub_nodes.at(word[offset]));
            }
            return false;
        }
        else if(offset + matches >= word.length() && matches < node->label.length()) {
            /**
             * @brief case 1:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|-------------|
             * 
             */
            return false;
        }
        else if(offset + matches >= word.length() && matches == node->label.length()) {
            /**
             * @brief case 2:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|
             * 
             */
            return node->is_end_of_node;
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
    void rdx_tree::removeRec(const std::string &word, size_t offset, rdx_node *node) {
        static size_t matches;
        matches = matchingConsecutiveCharacter(word, offset, node);
        if((matches == 0) || (matches > 0 && offset + matches < word.length() && matches >= node->label.length())) {
            /**
             * @brief case 0:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|
             * 
             */
            offset += matches;
            if(node->sub_nodes.count(word[offset]) != 0 && node->sub_nodes.at(word[offset]) != nullptr) {
                removeRec(word, offset, node->sub_nodes.at(word[offset]));
            }
        }
        else if(offset + matches >= word.length() && matches < node->label.length()) {
            /**
             * @brief case 1:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|-------------|
             * 
             */
        }
        else if(offset + matches >= word.length() && matches == node->label.length()) {
            /**
             * @brief case 2:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|
             * 
             */
            node->is_end_of_node = false;
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
    std::string rdx_tree::findSuccessorRec(const std::string &word, size_t offset, const rdx_node *node) {
        static size_t matches;
        matches = matchingConsecutiveCharacter(word, offset, node);
        if((matches == 0) || (matches > 0 && offset + matches < word.length() && matches >= node->label.length())) {
            /**
             * @brief case 0:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|
             *                              ^offset
             */
            offset += matches;
            for(auto &sub_node: node->sub_nodes) {
                if(sub_node.first == word[offset]) {
                    std::string ret = findSuccessorRec(word, offset, sub_node.second);
                    if(ret != "") {
                        return ret;
                    }
                }
                if(sub_node.first > word[offset]) {
                    std::string ret = getMinSuffix(sub_node.second);
                    if(ret != "") {
                        return word.substr(0, offset) + ret;
                    }
                }
            }
            return "";
        }
        else if(offset + matches >= word.length() && matches < node->label.length()) {
            /**
             * @brief case 1:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|-------------|
             *              ^offset
             */
            // return word.substr(0, offset) + node.label;
            std::string ret = getMinSuffix(node);
            if(ret != "") {
                return word.substr(0, offset) + ret;
            }
            for(auto &sub_node: node->sub_nodes) {
                ret = getMinSuffix(sub_node.second);
                if(ret != "") {
                    return word.substr(0, offset) + node->label + ret;
                }
            }
            return "";
        }
        else if(offset + matches >= word.length() && matches == node->label.length()) {
            /**
             * @brief case 2:
             * 
             * word_part:   |----matches----|
             * node.label:  |----matches----|
             *              ^offset
             */
            std::string ret;
            for(auto &sub_node: node->sub_nodes) {
                ret = getMinSuffix(sub_node.second);
                if(ret != "") {
                    return word.substr(0, offset) + node->label + ret;
                }
            }
            return "";
        }
        else {
            /**
             * @brief case 3:
             * 
             * word_part:   |----matches----|-------------|
             * node.label:  |----matches----|-------------|
             *              ^offset
             */
            if(word[offset + matches] < node->label[matches]) {
                std::string ret = getMinSuffix(node);
                if(ret != "") {
                    return word.substr(0, offset) + ret;
                }
                for(auto &sub_node: node->sub_nodes) {
                    ret = getMinSuffix(sub_node.second);
                    if(ret != "") {
                        return word.substr(0, offset) + node->label + ret;
                    }
                }
                return "";
            }
            else { // word[offset + matches] > node->label[matches]
                return "";
            }
        }
    }
    std::string rdx_tree::getMinSuffix(const rdx_node *node) {
        std::string min_suffix = node->label;
        if(node->is_end_of_node) {
            return min_suffix;
        }
        for(auto &sub_node: node->sub_nodes) {
            std::string ret = getMinSuffix(sub_node.second);
            if(ret != "") {
                return min_suffix + ret;
            }
        }
        return "";
    }
    std::string rdx_tree::findPredecessor(const std::string &word) {
        return findPredecessorRec(word, 0, root);
    }
    std::string rdx_tree::findPredecessorRec(const std::string &word, size_t offset, const rdx_node *node) {
        /* TODO: Finish fdPredecessor. */
        return "";
    }
    std::string rdx_tree::getMaxSuffix(const rdx_node *node) {
        /* TODO: Finish getMaxSUff. */
        return "";
    }
}