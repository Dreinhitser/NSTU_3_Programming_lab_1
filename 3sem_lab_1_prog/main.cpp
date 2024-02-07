#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


// randomly selects a character from the input file name
char generate_byte_encoding(const std::string& text)
{
	srand(time(nullptr));
	return text[rand() % text.size()];
}

// writes the string line to the binary file
void write_bin_file(const std::string& filename, const std::string& line, const char byte_encoding)
{
	std::ofstream out(filename, std::ios::binary | std::ios::app);
	if (!out.is_open()) {
		throw std::exception("Failed to open the output binary file");
	}

	char size = line.size();

	out.write(&size, sizeof(size));
	for (int i = 0; i < line.size(); i++) {
		char symbol = line[i] ^ byte_encoding;
		out.write(&symbol, sizeof(symbol));
	}

	out.close();
}

// returns the contents of a binary file as a string
std::string read_bin_file(const std::string& filename, const char byte_encoding)
{
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open()) {
		throw std::exception("Failed to open the input binary file");
	}

	std::string received_text = "";
	char size = 0;
	while (in.read(&size, sizeof(size))) {
		for (int i = 0; i < size; i++) {
			char c = 0;

			in.read(&c, sizeof(c));
			c ^= byte_encoding;
			received_text += c;
		}
		received_text += "\n";
	}

	in.close();
	return received_text;
}

int main(int argc, char* argv[])
{
	const std::string filename_in = std::string(argv[1]);
	const std::string filename_out = std::string(argv[1]) + ".bin";

	std::ifstream in(filename_in);

	if (!in.is_open()) {
		std::cerr << "Failed to open the input file\n";
		return 1;
	}
	
	// reading from a .txt file and writing to .bin file
	char byte_encoding = generate_byte_encoding(filename_in);

	std::string line = "";
	while (std::getline(in, line)) {
		try {
			write_bin_file(filename_out, line, byte_encoding);
		}
		catch (std::exception ex) {
			std::cerr << ex.what() << "\n";
			return 1;
		}
	}
	in.close();

	// reading from .bin file and content output to the console
	try {
		std::cout << read_bin_file(filename_out, byte_encoding);
	}
	catch (std::exception ex) {
		std::cerr << ex.what() << "\n";
		return 1;
	}

	return 0;
}