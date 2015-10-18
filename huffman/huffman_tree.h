#pragma once
#include <forward_list>
#include <vector>

class encoded_reader;
class raw_writer;

namespace internal_tree
{
	class node
	{
	public:
		explicit node(size_t frequency) { weight = frequency; }
		node(node&) = default;
		node& operator=(node&) = delete;
		virtual ~node() {}

		virtual bool is_leaf() = 0;
		size_t weight;
	};

	class internal_node : public node
	{
	public:
		internal_node(node* l, node* r)
			: node(l->weight + r->weight), left(l), right(r) {}
		internal_node(internal_node&) = default;
		internal_node& operator=(internal_node&) = delete;
		~internal_node() { delete left; delete right; }

		virtual bool is_leaf() override { return false; };

		node* left;
		node* right;
	};

	class leaf : public node
	{
	public:
		explicit leaf(uint8_t c, size_t frequency)
			: node(frequency), ch(c) {}
		leaf(leaf&) = default;
		leaf& operator=(leaf&) = delete;
		~leaf() {};

		virtual bool is_leaf() override { return true; };

		uint8_t ch;
	};


	struct nodes_compare
	{
		bool operator()(node* l, node* r) const
		{
			return l->weight > r->weight;
		}
	};
} // internal_tree

class huffman_tree
{
public:
	explicit huffman_tree(std::vector<std::pair<uint8_t, size_t>>&);
	huffman_tree(huffman_tree const& src) = delete;
	huffman_tree& operator=(huffman_tree& src) = delete;
	~huffman_tree();

	std::vector<std::vector<bool>> const& get_codes_mapping();
	void decode(encoded_reader &, raw_writer&) const;
private:
	static const size_t alphabet_size_;
	internal_tree::node* tree_root_;
	std::vector<std::vector<bool>> codes_;
	bool is_codes_resolved_;

	void build(std::vector<std::pair<uint8_t, size_t>>&);
	void resolve_codes(internal_tree::node* node, std::vector<bool> path);
};
