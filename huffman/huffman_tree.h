#pragma once
#include <vector>
#include "internal_huffman_tree.h"

class encoded_reader;
class raw_writer;

class huffman_tree
{
public:
    explicit huffman_tree  (std::vector<std::pair<uint8_t, size_t>>&);
    huffman_tree           (huffman_tree const& src) = delete;
    huffman_tree& operator=(huffman_tree& src)       = delete;
    ~huffman_tree          ();

    std::vector<std::vector<bool>> const& get_codes_mapping();
    
    void decode(encoded_reader &, raw_writer&) const;
private:
    static const size_t            alphabet_size_;
    internal_tree::node*           tree_root_;
    std::vector<std::vector<bool>> codes_;
    bool                           is_codes_resolved_;

    void build         (std::vector<std::pair<uint8_t, size_t>>&);
    void resolve_codes (internal_tree::node* node, std::vector<bool> path);
};
