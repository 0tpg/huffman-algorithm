#ifndef CLI_H
#define CLI_H
#include "../huff_tree/huff_tree.h"

/// CLI е класът представящ интерфейса използван от потребителя.
///
/// Член-променливата е текущото съобщение написано от потребителя в терминала.

class CLI {
    std::string m_input;

	/// Първо ниво на дълбочина на команди. Тук потребителя избира режим на работа.
    void command_sequence();
	/// Помощна функция за обработване на входа.
    void command_delegation();

	/// Второ ниво на дълбочина на командите. Тук потребителя избира файл, от който да се чете.
	void mode_sequence();
	/// Помощна функция за обработване на входа ако е избран режим компресия.
	void c_seq_delegation(const std::string&);
	/// Помощна функция за обработване на входа ако е избран режим декомпресия.
	void d_seq_delegation(const std::string&);
	/// Помощна функция обобщаваща общите изключения за двата режинма.
	void mode_seq_delegation_exceptions(const std::string&);

	/// Второ ниво на дълбочина на командите. Тук потребителя избира файл, в който да пише.
	void tree_cmd_sequence(const std::string&, const HuffmanTree&);
	/// Помощна функция за обработване на входа ако е избран режим компресия.
	void c_tree_cmd_seq_delegation(const std::string&, const HuffmanTree&);
	/// Помощна функция за обработване на входа ако е избран режим декомпресия.
	void d_tree_cmd_seq_delegation(const std::string&, const HuffmanTree&);
	/// Помощна функция за процедиране при съществуващ файл за писане.
	const std::string tree_overwrite_sequence_answer(const std::string&, const std::string&);
	/// Помощна функция обобщаваща общите изключения за двата режинма.
	void tree_cmd_seq_delegation_exceptions(const std::string&);

	/// Помощна функция за процедиране при желание за започване на нова сесия или приключване на програмата.
	const std::string critical_sequence_answer();
	/// Помощна функция за извличане на съдържанието от файл.
	const std::string contents_from(const std::string&) const;
	/// Помощна функция за получаване на речник от низ. В конкретния контекст по-рано речникът се сериализира до низ и тази функция се използва за да декодира низа. 
	const std::map<char, std::string> dictionary_from(const std::string&) const;

    void print_instructions() const;

public:
    CLI() = default;
    ~CLI() = default;

	/// Функция за активиране на интерфейса.
    void operator()();
};

#endif
