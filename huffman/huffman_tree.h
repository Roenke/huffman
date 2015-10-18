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
		explicit node(size_t frequency)
		{
			weight = frequency;
		}

		virtual bool is_leaf() = 0;
		size_t weight;

		virtual ~node(){}
	};

	class internal_node : public node
	{
	public:
		internal_node(node* l, node* r)
			: node(l->weight + r->weight), left(l), right(r) {}

		virtual bool is_leaf() override;

		node* left;
		node* right;
	};

	class leaf : public node
	{
	public:
		explicit leaf(uint8_t c, size_t frequency)
			: node(frequency), ch(c) {}

		virtual bool is_leaf() override;

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
	std::vector<std::vector<bool>> const& get_codes_mapping();
	void decode(encoded_reader &, raw_writer&) const;
private:
	static const size_t alphabet_size;
	internal_tree::node* tree_root;
	size_t symbols_count;
	std::vector<std::vector<bool>> codes;
	bool is_codes_resolved;

	void build(std::vector<std::pair<uint8_t, size_t>>&);
	void resolve_codes(internal_tree::node* node, std::vector<bool> path);
};
