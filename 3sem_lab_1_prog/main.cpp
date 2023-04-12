#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

// randomly selects a character from the input file name
char generate_byte_encoding(const string& text)
{
	srand(time(nullptr));

	return text[rand() % text.size()];
}

// writes the string line to the binary file
void write_bin_file(const string& filename, const string& line, const char byte_encoding)
{
	ofstream out(filename, ios::binary | ios::app);
	if (!out.is_open())
	{
		throw exception("Failed to open the output binary file");
	}

	char size = line.size();

	out.write((char*)&size, sizeof(size));
	for (int i = 0; i < line.size(); i++)
	{
		char symbol = line[i] ^ byte_encoding;
		out.write((char*)&symbol, sizeof(symbol));
	}

	out.close();
}

// returns the contents of a binary file as a string
string read_bin_file(const string& filename, const char byte_encoding)
{
	ifstream in(filename, ios::binary);
	if (!in.is_open())
	{
		throw exception("Failed to open the input binary file");
	}

	string received_text = "";
	char size = 0;
	while (in.read(&size, sizeof(size)))
	{
		for (int i = 0; i < size; i++)
		{
			char c;

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
	const string filename_in = string(argv[1]);
	const string filename_out = string(argv[1]) + ".bin";

	ifstream in(filename_in);

	if (!in.is_open())
	{
		cerr << "Failed to open the input file\n";
		return 1;
	}
	
	// reading from a .txt file and writing to .bin file
	char byte_encoding = generate_byte_encoding(filename_in);

	string line = "";
	while (getline(in, line))
	{
		try
		{
			write_bin_file(filename_out, line, byte_encoding);
		}
		catch (exception ex)
		{
			cerr << ex.what() << "\n";
			return 1;
		}
	}
	in.close();

	// reading from .bin file and content output to the console
	try
	{
		cout << read_bin_file(filename_out, byte_encoding);
	}
	catch (exception ex)
	{
		cerr << ex.what() << "\n";
		return 1;
	}

	return 0;
}