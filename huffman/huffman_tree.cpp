#include "huffman_tree.h"
#include <vector>
#include <queue>
#include "encoded_reader.h"
#include "raw_writer.h"

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

void huffman_tree::decode(encoded_reader &reader, raw_writer& writer) const
{
	using namespace internal_tree;

	auto input_buffer = new uint8_t[4096];
	auto output_buffer = new uint8_t[4096];
	size_t i_ptr;
	size_t o_ptr = 0;
	auto node_ptr = tree_root;
	internal_node* internal_node_ptr;
	size_t readed;
	uint8_t bit_number = 0;
	auto total_characters = tree_root->weight;
	size_t total_decoded = 0;
	bool binary_value;
	while(reader.read_content(reinterpret_cast<char*>(input_buffer), 4096, readed))
	{
		for (i_ptr = 0; i_ptr < readed; ++i_ptr)
		{
			for (bit_number = 0; bit_number < CHAR_BIT; ++bit_number)
			{
				binary_value = input_buffer[i_ptr] >> bit_number & 1 != 0;
				if(node_ptr->is_leaf())
				{
					output_buffer[o_ptr++] = dynamic_cast<leaf*>(node_ptr)->ch;
					
					if(o_ptr >= 4096)
					{
						writer.append_data(reinterpret_cast<char*>(output_buffer), o_ptr);
						total_decoded += o_ptr;
						o_ptr = 0;
					}

					if (tree_root->is_leaf()) continue;
					node_ptr = tree_root;
				}

				internal_node_ptr = dynamic_cast<internal_node*>(node_ptr);
				node_ptr = binary_value ? internal_node_ptr->left : internal_node_ptr->right;
			}
		}
	}

	if(node_ptr->is_leaf())
	{
		output_buffer[o_ptr] = dynamic_cast<leaf*>(node_ptr)->ch;
	}

	writer.append_data(reinterpret_cast<char*>(output_buffer), total_characters - total_decoded);
}
