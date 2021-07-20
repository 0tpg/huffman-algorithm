#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H
#include <iostream>
#include <vector>
#include <string>
#include <map>

/// HuffmanTree е класът представляващ дървото, което строим по информацията от даден файл. С негова помощ кодираме съотвтното съобщение в двоичен код.
///
/// Член-променливите са коренът на дървото, текстовия низ по който изграждаме дървото, компресирания бинарен низ и речник, с чиято помощ помним на коя буква кой бинарен низ съответства.

class HuffmanTree {

	struct Node {
		unsigned short int value;
		Node* left;
		Node* right;

		Node(const unsigned short int&, Node*, Node*);
	};

	Node* m_root;
	std::string m_input;
	std::string m_compressed_sequence;
	std::map<char, std::string> m_dictionary;

	/// Помощна функция за деструктора на дървото
	void destr_helper(Node*&);

	/// Помощна функция имплементираща алгоритъма за изграждане на дървото по подаден вектор с листа.
	void constr_tree(std::vector<Node*>&);
	/// Помощна функция имплементираща алгоритъма за изграждане на речник по подадени текущ корен, текуща следа, набора от различни букви във входния низ и вектор с адресите на възлите.
	void constr_dictionary(const Node*, std::string&, const std::string&, const std::vector<Node*>&);
	
	/// Помощна функция за получаване на честотна таблица.
	const std::map<char, unsigned short int> freq_table() const;
	/// Помощна функция за изграждане на вектор с листата на дървото.
	const std::vector<Node*> leaf_nodes() const;
	/// Помощна функция за получаване на набора от различни букви.
	const std::string leaf_chars() const;

	/// Помощна функция за получаване на бинарен низ от входния низ.
	const std::string binary_input() const;
	/// Помощна функция за получаване на вектор с байтове от бинарния низ.
	const std::vector<std::string> bytes_from_input() const;

	/// Помощна функция за обръщане на низ.
	const std::string reverse_string(const std::string&) const;
	/// Помощна функция за обръщане на вектор с низове.
	const std::vector<std::string> reverse_string_vec(const std::vector<std::string>&) const;

	/// Помощна функция за получаване на десетично число от бинарен низ.
	const unsigned short int decimal_from(const std::string&) const;
	/// Помощна функция за получаване на вектор с десетични числа от вектор с бинарни низове.
	const std::vector<unsigned short int> decimal_bytes() const;

	/// Помощна функция имплементираща алгоритъма за разделяне на бинарен низ на вектор с бинарни следи от някакъв речник.
	void divide_compressed_values(std::string, std::vector<std::string>&) const;
	/// Помощна функция за получаване на вектор с бинарни следи от бинарен низ.
	const std::vector<std::string> compressed_values_vec() const;
	/// Помощна функция за получаване на низ от бинарни следи.
	const std::string stringified_input() const;

	/// Помощна функция за запазване на бинарен низ във файл.
	void save_compression(const std::string&) const;
	/// Помощна функция за запазване на речник във файл.
	void save_dictionary(const std::string&) const;
	/// Помощна функция за запазване на низ във файл.
	void save_decompression(const std::string&) const;

	/// Помощна функция за визуализиращата дървото функция.
	void graphviz_helper(const Node*, std::ostream&) const;

public:
	/// Конструктор използван за строене на дърво от низ.
	HuffmanTree(const std::string&);
	/// Конструктор използван за строене на дърво от бинарен низ и речник.
	HuffmanTree(const std::string&, const std::map<char, std::string>&);
	~HuffmanTree();

	/// Функция за смятане на степента на компресия.
	double degree_of_compression() const;
	/// Функция за дебъг режим.
	void debug() const;

	/// Функция за запазване на информацията получена след компресия.
	void save_compression_data(const std::string&) const;
	/// Функция за запазване на информацията получена след декомресия.
	void save_decompression_data(const std::string&) const;

	/// Функция за визуализиране на дървото.
	void graphviz(const std::string&) const;
};

#endif
