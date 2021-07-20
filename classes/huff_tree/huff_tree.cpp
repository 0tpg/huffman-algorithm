#include "huff_tree.h"
#include <cmath>
#include <fstream>

HuffmanTree::Node::Node(const unsigned short int& _value, Node* _left = nullptr, Node* _right = nullptr) : value{_value}, left{_left}, right{_right} {}

void HuffmanTree::destr_helper(Node*& curr_root) {
	if(!curr_root) {
		return;
	}

	destr_helper(curr_root -> left);
	destr_helper(curr_root -> right);

	delete curr_root;
}

void HuffmanTree::constr_tree(std::vector<Node*>& nodes) {
	while(nodes.size() != 1) {
		Node* first_min{nodes.front()};
		size_t first_min_index{0};

		for(size_t i = 0; i < nodes.size(); i++) {
			if(nodes[i] -> value < first_min -> value) {
				first_min = nodes[i];
				first_min_index = i;
			}
		}

		nodes.erase(nodes.begin() + first_min_index);
		
		Node* second_min{nodes.front()};
		size_t second_min_index{0};

		for(size_t i = 0; i < nodes.size(); i++) {
			if(nodes[i] -> value < second_min -> value) {
				second_min = nodes[i];
				second_min_index = i;
			}
		}

		nodes.erase(nodes.begin() + second_min_index);

		if(first_min_index <= second_min_index) {
			nodes.insert(nodes.begin() + first_min_index, new (std::nothrow) Node{(unsigned short int)(first_min -> value + second_min -> value), first_min, second_min});
		} else {
			nodes.insert(nodes.begin() + second_min_index, new (std::nothrow) Node{(unsigned short int)(first_min -> value + second_min -> value), second_min, first_min});
		}
	}
}

void HuffmanTree::constr_dictionary(const Node* curr_root, std::string& trace, const std::string& char_vec, const std::vector<Node*>& address_vec) {
	if(!curr_root) {
		return;
	}

	if(curr_root -> left) {
		trace.push_back('0');
	}

	constr_dictionary(curr_root -> left, trace, char_vec, address_vec);

	if(curr_root -> right) {
		trace.push_back('1');
	}

	constr_dictionary(curr_root -> right, trace, char_vec, address_vec);

	bool is_leaf{ !curr_root -> left && !curr_root -> right };

	if(is_leaf) {
		for(size_t i = 0; i < address_vec.size(); i++){
			if(curr_root == address_vec[i]) {
				m_dictionary.insert(std::make_pair(char_vec[i], trace));
			}
		}
	}

	if(!trace.empty()) {
		trace.pop_back();
	}
}

const std::map<char, unsigned short int> HuffmanTree::freq_table() const {
	std::map<char, unsigned short int> table;

	for(const char& curr_char : m_input) {
		table[curr_char]++;
	}

	return table;
}

const std::vector<HuffmanTree::Node*> HuffmanTree::leaf_nodes() const {
	std::vector<Node*> leaves;

	for(const auto& curr_pair : freq_table()) {
		leaves.push_back(new (std::nothrow) Node{curr_pair.second});
	}

	return leaves;
}

const std::string HuffmanTree::leaf_chars() const {
	std::string leaf_chars;

	for (const auto& curr_pair : freq_table()) {
        leaf_chars.push_back(curr_pair.first);
    }

	return leaf_chars;
}

const std::string HuffmanTree::binary_input() const {
	std::string bin;

	for(const char& curr_char : m_input) {
		bin.append(m_dictionary.find(curr_char) -> second);
	}

	return bin;
}

const std::vector<std::string> HuffmanTree::bytes_from_input() const {
	std::vector<std::string> bytes{""};

	for(const char& curr_char : m_compressed_sequence) {
		std::string string_from_char{curr_char};

		if(bytes.back().length() == 8) {
			bytes.push_back(string_from_char);
		} else {
			bytes.back().append(string_from_char);
		}
	}

	return bytes;
}

const std::string HuffmanTree::reverse_string(const std::string& bin) const {
	std::string reversed;

	for(size_t i = bin.length() - 1; i != -1; i--) {
		reversed.push_back(bin[i]);
	}

	return reversed;
}

const std::vector<std::string> HuffmanTree::reverse_string_vec(const std::vector<std::string>& vec) const {
	std::vector<std::string> reversed;

	for(const std::string& curr_string : vec) {
		reversed.push_back(reverse_string(curr_string));
	}

	return reversed;
}

const unsigned short int HuffmanTree::decimal_from(const std::string& bin) const {
	size_t power{0};
	unsigned short int dec{0};

	for(const char& curr_char : bin) {
		dec += (curr_char - '0') * pow(2, power);
		power++;
	}

	return dec;
}

const std::vector<unsigned short int> HuffmanTree::decimal_bytes() const {
	std::vector<unsigned short int> dec_vec;

	for(const std::string& curr_string : reverse_string_vec(bytes_from_input())) {
		dec_vec.push_back(decimal_from(curr_string));
	}

	return dec_vec;
}

void HuffmanTree::divide_compressed_values(std::string curr_str, std::vector<std::string>& compressed_values_vec) const {
	if(curr_str.empty()) {
		return;
	}

	size_t length{1};

	do {
		for(auto curr_pair : m_dictionary) {
			if(curr_str.substr(0, length) == curr_pair.second) {
				compressed_values_vec.push_back(curr_pair.second);
				divide_compressed_values(curr_str.erase(0, length), compressed_values_vec);
				return;
			}
		}

		length++;
	} while(length != curr_str.length());
}

const std::vector<std::string> HuffmanTree::compressed_values_vec() const {
	std::vector<std::string> result;
	divide_compressed_values(m_compressed_sequence, result);
	
	return result;
}

const std::string HuffmanTree::stringified_input() const {
	std::string stringified;

	for(const std::string& curr_str : compressed_values_vec()) {
		for(auto curr_pair : m_dictionary) {
			if(curr_str == curr_pair.second) {
				stringified.push_back(curr_pair.first);
			}
		}
	}

	return stringified;
}

void HuffmanTree::save_compression(const std::string& filepath) const {
	std::ofstream file;
	file.open(filepath.c_str(), std::ios::out);

	file << m_compressed_sequence;

	file.close();
}

void HuffmanTree::save_dictionary(const std::string& filepath) const {
	std::ofstream file;
	file.open(filepath.c_str(), std::ios::out);

	size_t curr_pos{0};
	for(auto curr_pair : m_dictionary) {
		file << curr_pair.first << "," << curr_pair.second;

		if(curr_pos != m_dictionary.size() - 1) {
			file << ",";
		}

		curr_pos++;
	}

	file.close();
}

void HuffmanTree::save_decompression(const std::string& filepath) const {
	std::ofstream file;
	file.open(filepath.c_str(), std::ios::out);

	file << m_input;

	file.close();
}

void HuffmanTree::graphviz_helper(const Node* curr_node, std::ostream& out) const {
	if(!curr_node) {
		return;
	}

	if (curr_node -> left) {
        out << (long long)curr_node << "->" << (long long)(curr_node -> left) << "[label=""0""];\n";
        graphviz_helper(curr_node -> left, out);
    } 

	out << (long long)curr_node << "[label=\"" << curr_node -> value << "\"];\n";

    if (curr_node -> right) {
        out << (long long)curr_node << "->" << (long long)(curr_node -> right) << "[label=""1""];\n";
        graphviz_helper(curr_node -> right, out);
    }
}

HuffmanTree::HuffmanTree(const std::string& input) : m_root{nullptr}, m_input{input}, m_compressed_sequence{}, m_dictionary{} {
	std::vector<Node*> nodes{leaf_nodes()};
	const std::vector<Node*> leaves_addresses{nodes};

	constr_tree(nodes);
	m_root = nodes.front();

	std::string trace;
	constr_dictionary(m_root, trace, leaf_chars(), leaves_addresses);

	m_compressed_sequence = binary_input();
}

HuffmanTree::HuffmanTree(const std::string& compressed_sequence, const std::map<char, std::string>& dictionary) : m_root{nullptr}, m_input{}, m_compressed_sequence{compressed_sequence}, m_dictionary{dictionary} {
	m_input = stringified_input();

	std::vector<Node*> nodes{leaf_nodes()};
	const std::vector<Node*> leaves_addresses{nodes};

	constr_tree(nodes);
	m_root = nodes.front();

	std::string trace;
	constr_dictionary(m_root, trace, leaf_chars(), leaves_addresses);
}

HuffmanTree::~HuffmanTree() {
	destr_helper(m_root);
}

double HuffmanTree::degree_of_compression() const {
	return (double)m_compressed_sequence.length() / ((double)m_input.length() * 8);
}

void HuffmanTree::debug() const {
	size_t curr_pos{0};
	for(const unsigned short int& curr_int : decimal_bytes()) {
		std::cout << curr_int;

		if(curr_pos != decimal_bytes().size() - 1) {
			std::cout << ' ';
		}

		curr_pos++;
	}
}

void HuffmanTree::save_compression_data(const std::string& filename) const {
	std::string compression_filepath{".\\.\\files\\compression_files\\compressed_sequences\\"};
	std::string dictionary_filepath{".\\.\\files\\compression_files\\dictionaries\\"};

	compression_filepath.append(filename);
	dictionary_filepath.append(filename);

	save_compression(compression_filepath);
	save_dictionary(dictionary_filepath);
}

void HuffmanTree::save_decompression_data(const std::string& filename) const {
	std::string decompression_filepath{".\\.\\files\\decompression_files\\decompressed_sequences\\"};

	decompression_filepath.append(filename);

	save_decompression(decompression_filepath);
}

void HuffmanTree::graphviz(const std::string& filename) const {
	std::string vispath{".\\.\\graphviz\\"};
	vispath.append(filename);

	std::ofstream file;
	file.open(vispath.c_str(), std::ios::out);

	file << "digraph test{ " << std::endl;
    graphviz_helper(m_root, file);
    file << "}" << std::endl;

	file.close();
}
