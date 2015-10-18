#include "huffman_tree.h"
#include <vector>
#include <queue>


typedef std::vector<bool> char_code;

bool internal_tree::internal_node::is_leaf()
{
	return false;
}

bool internal_tree::leaf::is_leaf()
{
	return true;
}

const size_t huffman_tree::alphabet_size = 256;

huffman_tree::huffman_tree(std::vector<std::pair<uint8_t, size_t>>& frequencies)
{
	tree_root = nullptr;
	symbols_count = 0;
	codes.resize(alphabet_size);
	build(frequencies);
}

void huffman_tree::build(std::vector<std::pair<uint8_t, size_t>>& frequencies)
{
	using namespace internal_tree;
	std::priority_queue<node*, std::vector<node*>, nodes_compare> queue;
	auto size = frequencies.size();
	symbols_count = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if (frequencies[i].second != 0)
		{
			symbols_count++;
			queue.push(new leaf(frequencies[i].first, frequencies[i].second));
		}
	}

	while(queue.size() > 1)
	{
		auto first = queue.top();
		queue.pop();
		auto second = queue.top();
		queue.pop();
		queue.push(new internal_node(first, second));
	}

	tree_root = queue.top();
}

void huffman_tree::resolve_codes(internal_tree::node* node, std::vector<bool> path)
{
	if(node->is_leaf())
	{
		auto leaf = dynamic_cast<internal_tree::leaf*>(node);
		if(node == tree_root)
		{
			path.push_back(true);
		}

		codes[leaf->ch] = path;
		return;
	}
	
	auto internal_node = dynamic_cast<internal_tree::internal_node*>(node);

	auto to_right(path);
	path.push_back(true);
	to_right.push_back(false);
	resolve_codes(internal_node->left, path);
	resolve_codes(internal_node->right, to_right);
}

std::vector<std::vector<bool>> const& huffman_tree::get_codes_mapping()
{
	if (!is_codes_resolved)
	{
		char_code vector;
		resolve_codes(tree_root, vector);
		is_codes_resolved = true;
	}

	return codes;
}

void huffman_tree::decode(std::vector<bool>& content, std::vector<uint8_t>& output)
{
	using namespace internal_tree;
	auto node_ptr = tree_root;
	internal_node* internal_node_ptr;
	auto content_size = content.size();
	for (size_t i = 0; i < content_size; ++i)
	{
		if(node_ptr->is_leaf())
		{
			output.push_back(dynamic_cast<leaf*>(node_ptr)->ch);
			if (tree_root->is_leaf()) continue;
			node_ptr = tree_root;
		}
		
		internal_node_ptr = dynamic_cast<internal_node*>(node_ptr);
		node_ptr = content[i] ? internal_node_ptr->left : internal_node_ptr->right;
	}
}
