#include "cli.h"
#include <fstream>

void CLI::command_sequence() {
    do {
		m_input.clear();
        std::cout << "Please enter 'c' or 'd' to choose operational mode, 'new' to start new session or 'q' to quit\n"
            	  << "$ ";

        std::getline(std::cin, m_input);

        try {
            command_delegation();
        } catch(const std::runtime_error& err) {
            std::cout << err.what() << '\n';
        }

    } while (m_input != "q");
}

void CLI::command_delegation() {
	bool is_import_command{m_input.substr(0, 2) == "i " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};
	bool is_export_command{m_input.substr(0, 2) == "o " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};
	bool has_wrong_extension{(m_input.substr(0, 2) == "i " && m_input.substr(m_input.length() - 4, 4) != ".txt" && m_input.length() > 6) || 
							 (m_input.substr(0, 2) == "o " && m_input.substr(m_input.length() - 4, 4) != ".txt" && m_input.length() > 6)};
	
	if(m_input == "c") {
		std::cout << "Entered compression mode\n\n";
		mode_sequence();

	} else if(m_input == "d") {
		std::cout << "Entered decompression mode\n\n";
		mode_sequence();

	} else if(is_import_command) {
		throw std::runtime_error("you have to enter a mode before importing from file\n");

	} else if(is_export_command) {
		throw std::runtime_error("you have to enter a mode and choose file before exporting\n");

	} else if(has_wrong_extension) {
		throw std::runtime_error("invalid file extension\n");

	} else if(m_input == "dbg") {
		throw std::runtime_error("you have to enter a mode and choose file before debugging\n");

	} else if(m_input == "new") {
		const std::string answer{critical_sequence_answer()};

		if(answer == "y") {
			std::cout << "Canceled current session\n"
					  << "Started new session\n\n";

		} else if(answer == "n") {
			std::cout << "Cancelling new session...\n\n";
			m_input.clear();
		}

		return;

	} else if(m_input == "q") {
		const std::string answer{critical_sequence_answer()};

		if(answer == "y") {
			std::cout << "Exiting...";

		} else if(answer == "n") {
			std::cout << "Cancelling quit sequence...\n\n";
			m_input.clear();
		}

		return;

	} else {
		throw std::runtime_error("invalid input\n");
	}
}

void CLI::mode_sequence() {
	bool is_valid{false};
	const std::string mode{m_input};

	do {
        m_input.clear();

		if(mode == "c") {
			std::cout << "C: ";
		} else if(mode == "d"){
			std::cout << "D: ";
		}

        std::cout << "Please enter 'i <filename>', 'new' to start new session or 'q' to quit\n"
        		  << "$ ";

        std::getline(std::cin, m_input);

        try {
			if(mode == "c") {
				c_seq_delegation(mode);
			} else if(mode == "d"){
				d_seq_delegation(mode);
			}
        } catch(const std::runtime_error& err) {
        	std::cout << err.what() << '\n';
        }

		bool is_import_command{m_input.substr(0, 2) == "i " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};

		is_valid = is_import_command ||
				   m_input == "new" ||
				   m_input == "q";

    } while (!is_valid);
}

void CLI::c_seq_delegation(const std::string& mode) {
	bool is_import_command{m_input.substr(0, 2) == "i " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};

	if(is_import_command) {
		try {
			std::string filepath{".\\.\\files\\decompression_files\\decompressed_sequences\\"};
			std::string vispath{".\\.\\graphviz\\"};

			const std::string filename{m_input.substr(2, m_input.length() - 1)};
			const std::string dotname{filename.substr(0, filename.length() - 4).append(".dot")};
			const std::string input_str{contents_from(filepath.append(filename))};

			HuffmanTree curr{input_str};
			curr.graphviz(dotname);

			vispath.append(dotname);

			std::cout << "Imported from " << filepath << '\n'
					  << "Visualized in " << vispath << "\n\n";

        	tree_cmd_sequence(mode, curr);

		} catch(const std::runtime_error& err) {
			m_input.clear();
			throw;
		}

	} else {
		mode_seq_delegation_exceptions(mode);
	}
}

void CLI::d_seq_delegation(const std::string& mode) {
	bool is_import_command{m_input.substr(0, 2) == "i " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};

	if(is_import_command) {
		try {
			std::string compressed_filepath{".\\.\\files\\compression_files\\compressed_sequences\\"};
			std::string dict_filepath{".\\.\\files\\compression_files\\dictionaries\\"};
			std::string vispath{".\\.\\graphviz\\"};

			const std::string filename{m_input.substr(2, m_input.length() - 1)};
			const std::string dotname{filename.substr(0, filename.length() - 4).append(".dot")};
			const std::string compressed_str{contents_from(compressed_filepath.append(filename))};
			const std::map<char, std::string> dict{dictionary_from(contents_from(dict_filepath.append(filename)))};

			HuffmanTree curr{compressed_str, dict};
			curr.graphviz(dotname);

			vispath.append(dotname);

        	std::cout << "Imported from " << compressed_filepath << " and " << dict_filepath << "\n"
					  << "Visualized in " << vispath << "\n\n";

			tree_cmd_sequence(mode, curr);

		} catch(const std::runtime_error& err) {
			m_input.clear();
			throw;
		}

	} else {
		mode_seq_delegation_exceptions(mode);
	}
}

void CLI::mode_seq_delegation_exceptions(const std::string& mode) {
	bool is_export_command{m_input.substr(0, 2) == "o " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};
	bool has_wrong_extension{(m_input.substr(0, 2) == "i " && m_input.substr(m_input.length() - 4, 4) != ".txt" && m_input.length() > 6) || 
							 (m_input.substr(0, 2) == "o " && m_input.substr(m_input.length() - 4, 4) != ".txt" && m_input.length() > 6)};

	if(m_input == "c" || m_input == "d") {
		std::string message;

		if(mode == "c") {
			message = "already in compression mode\n";
		} else if(mode == "d") {
			message = "already in decompression mode \n";
		}

		throw std::runtime_error(message);

	} else if(is_export_command) {
		throw std::runtime_error("you have to choose a file before exporting\n");

	} else if(has_wrong_extension) {
		throw std::runtime_error("invalid file extension\n");
		
	} else if(m_input == "dbg") {
		throw std::runtime_error("you have to choose a file before debugging\n");
		
	} else if(m_input == "new") {
		const std::string answer{critical_sequence_answer()};

		if(answer == "y") {
			std::cout << "Canceled current session\n"
					  << "Started new session\n\n";

		} else if(answer == "n") {
			std::cout << "Cancelling new session...\n\n";
			m_input.clear();
		}

		return;

	} else if(m_input == "q") {
		const std::string answer{critical_sequence_answer()};

		if(answer == "y") {
			std::cout << "Exiting...";

		} else if(answer == "n") {
			std::cout << "Cancelling quit sequence...\n\n";
			m_input.clear();
		}

		return;

	} else {
		throw std::runtime_error("invalid input\n");
	}
}

void CLI::tree_cmd_sequence(const std::string& mode, const HuffmanTree& curr_tree) {
	bool is_valid{false};

	do {
        m_input.clear();
        
		if(mode == "c") {
			std::cout << "C: Please enter 'o <filename>' to export, 'new' to start new session or type 'q' to quit\n"
        		  << "$ ";
		} else if(mode == "d") {
			std::cout << "D: Please enter 'o <filename>' to export, 'dbg' to enter debug mode, 'new' to start new session or type 'q' to quit\n"
        		  << "$ ";
		}

        std::getline(std::cin, m_input);

        try {
			if(mode == "c") {
				c_tree_cmd_seq_delegation(mode, curr_tree);
			} else if(mode == "d") {
				d_tree_cmd_seq_delegation(mode, curr_tree);
			}
        } catch(const std::runtime_error& err) {
        	std::cout << err.what() << '\n';
        }

		bool is_export_command{m_input.substr(0, 2) == "o " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};

		is_valid = is_export_command ||
				   m_input == "dbg" ||
				   m_input == "new" ||
				   m_input == "q";

    } while (!is_valid);
}

void CLI::c_tree_cmd_seq_delegation(const std::string& mode, const HuffmanTree& curr_tree) {
	bool is_export_command{m_input.substr(0, 2) == "o " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};

	if(is_export_command) {
		std::string compressed_filepath{".\\.\\files\\compression_files\\compressed_sequences\\"};
		std::string dict_filepath{".\\.\\files\\compression_files\\dictionaries\\"};
		const std::string filename{m_input.substr(2, m_input.length() - 1)};

		compressed_filepath.append(filename);
		dict_filepath.append(filename);

		bool file_already_exists{std::ifstream{compressed_filepath.c_str()}.good() &&
		   						 std::ifstream{dict_filepath.c_str()}.good()};

		if(file_already_exists) {
			const std::string answer{tree_overwrite_sequence_answer(mode, filename)};

			if(answer == "y") {
				std::cout << '\n';

			} else if(answer == "n") {
				std::cout << "Cancelling export...\n\n";
				m_input.clear();

				return;
			}
		}

		std::cout << "Exported to " << compressed_filepath << " and " << dict_filepath << "\n";
		curr_tree.save_compression_data(filename);
		std::cout << "Degree of compression: " << curr_tree.degree_of_compression() << "\n\n";

	} else if(m_input == "dbg") {
		m_input.clear();
		throw std::runtime_error("can't debug in compression mode\n");
		
	} else {
		tree_cmd_seq_delegation_exceptions(mode);
	}
}

void CLI::d_tree_cmd_seq_delegation(const std::string& mode, const HuffmanTree& curr_tree) {
	bool is_export_command{m_input.substr(0, 2) == "o " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};

	if(is_export_command) {
		std::string decompressed_filepath{".\\.\\files\\decompression_files\\decompressed_sequences\\"};
		const std::string filename{m_input.substr(2, m_input.length() - 1)};

		decompressed_filepath.append(filename);

		bool file_already_exists{std::ifstream{decompressed_filepath.c_str()}.good()};

		if(file_already_exists) {
			const std::string answer{tree_overwrite_sequence_answer(mode, filename)};

			if(answer == "y") {
				std::cout << '\n';

			} else if(answer == "n") {
				std::cout << "Cancelling export...\n\n";
				m_input.clear();

				return;
			}
		}
		
		std::cout << "Exported to " << decompressed_filepath << "\n\n";
		curr_tree.save_decompression_data(filename);

	} else if(m_input == "dbg") {
		std::cout << "Debug sequence: ";
		curr_tree.debug();
		std::cout << "\n\n";
		
	} else {
		tree_cmd_seq_delegation_exceptions(mode);
	}
}

const std::string CLI::tree_overwrite_sequence_answer(const std::string& mode, const std::string& filename) {
	std::string answer;
	bool is_valid{false};

	do {
        answer.clear();

		if(mode == "c") {
			std::cout << "\nC";
		} else if(mode == "d") {
			std::cout << "\nD";
		}

		std::cout << ": File " << filename << " already exists. Do you want to overwrite it? ('y'/'n')\n"
					  << "$ ";

        std::getline(std::cin, answer);

		is_valid = answer == "y" ||
				   answer == "n";

		if(!is_valid) {
			std::cout << "invalid answer\n";
		}

    } while (!is_valid);

	return answer;
}

void CLI::tree_cmd_seq_delegation_exceptions(const std::string& mode) {
	bool is_import_command{m_input.substr(0, 2) == "i " && m_input.substr(m_input.length() - 4, 4) == ".txt" && m_input.length() > 6};
	bool has_wrong_extension{(m_input.substr(0, 2) == "i " && m_input.substr(m_input.length() - 4, 4) != ".txt" && m_input.length() > 6) || 
							 (m_input.substr(0, 2) == "o " && m_input.substr(m_input.length() - 4, 4) != ".txt" && m_input.length() > 6)};

	if(m_input == "c" || m_input == "d") {
		std::string message;

		if(mode == "c") {
			message = "already in compression mode\n";
		} else if(mode == "d") {
			message = "already in decompression mode \n";
		}

		throw std::runtime_error(message);
		
	} else if(is_import_command) {
		throw std::runtime_error("already finished importing\n");

	} else if(has_wrong_extension) {
		throw std::runtime_error("invalid file extension\n");
		
	} else if(m_input == "new") {
		const std::string answer{critical_sequence_answer()};

		if(answer == "y") {
			std::cout << "Canceled current session\n"
					  << "Started new session\n\n";

		} else if(answer == "n") {
			std::cout << "Cancelling new session...\n\n";
			m_input.clear();
		}

		return;

	} else if(m_input == "q") {
		const std::string answer{critical_sequence_answer()};

		if(answer == "y") {
			std::cout << "Exiting...";

		} else if(answer == "n") {
			std::cout << "Cancelling quit sequence...\n\n";
			m_input.clear();
		}

		return;

	} else {
		throw std::runtime_error("invalid input\n");
	}
}

const std::string CLI::critical_sequence_answer() {
	std::string answer;
	bool is_valid{false};

	do {
        answer.clear();

		if(m_input == "new") {
			std::cout << "\nAre you sure you want to cancel this session and begin a new one? ('y'/'n')\n"
					  << "$ ";
		} else if(m_input == "q") {
			std::cout << "\nAre you sure you want to quit? ('y'/'n')\n"
					  << "$ ";
		}

        std::getline(std::cin, answer);

		is_valid = answer == "y" ||
				   answer == "n";

		if(!is_valid) {
			std::cout << "invalid answer\n";
		}

    } while (!is_valid);

	return answer;
}

const std::string CLI::contents_from(const std::string& filepath) const {
	std::ifstream file;
	std::string line;
	std::string contents;

	file.open(filepath.c_str(), std::ios::in);

	if(!file) {
		throw std::runtime_error("file doesn't exist\n");
	}

	while(std::getline(file, line)) {
		contents.append(line);
	}

	if(contents.empty()) {
		throw std::runtime_error("empty file\n");
	}

	file.close();

	return contents;
}

const std::map<char, std::string> CLI::dictionary_from(const std::string& encoded) const {
	size_t comma_count{0};
	char curr_key;
	std::string curr_val;
	std::map<char, std::string> dictionary;

	for(const char& curr_char : encoded) {
		if(curr_char == ',') {
			comma_count++;
			if(comma_count % 2 == 0) {
				dictionary.insert(std::make_pair(curr_key, curr_val));
				curr_val.clear();
			}
		} else {
			if(comma_count % 2 == 0) {
				curr_key = curr_char;
			} else if(comma_count % 2 == 1) {
				curr_val.push_back(curr_char);
			}
		}
	}

	dictionary.insert(std::make_pair(curr_key, curr_val));
	return dictionary;
}

void CLI::print_instructions() const {
    std::cout << "              Instructions\n"
              << " ==========================================\n"
              << "||                                        ||\n"
              << "|| 'c' - precedes compression             ||\n"
			  << "||                                        ||\n"
              << "|| 'd' - precedes decompression           ||\n"
			  << "||                                        ||\n"
              << "|| 'i <filename>' - indicates which file  ||\n"
              << "|| will be used to build tree             ||\n"
			  << "||                                        ||\n"
              << "|| 'o <filename>' - indicates filename in ||\n"
              << "|| which the compressed sequence will be  ||\n"
              << "|| saved in                               ||\n"
			  << "||                                        ||\n"
			  << "|| 'dbg' - prints debug information       ||\n"
			  << "||                                        ||\n"
			  << "|| 'new' - cancels current session and    ||\n"
			  << "|| starts over                            ||\n"
			  << "||                                        ||\n"
              << "|| 'q' - quit                             ||\n"
              << "||                                        ||\n"
			  << "|| * please write file extensions i.e.    ||\n"
			  << "||  '.txt' at the ends of file names      ||\n"
			  << "|| whenever you have to                   ||\n"
			  << "||                                        ||\n"
              << " ==========================================\n\n";
}

void CLI::operator()() {
	print_instructions();
	command_sequence();
}
