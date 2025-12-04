#include "utils.h"
#include <random>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include "exceptions.hpp"
#include <algorithm>
#include <cctype>

void clear_screen()
{
	system("CLS");
}

void wait_for_input()
{
	system("PAUSE");
}

int get_random_number(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}

void print_line(char c, int width)
{
	std::cout << std::setfill(c) << std::setw(width) << "" << std::endl << std::setfill(' ');
}

std::string to_lower(const std::string& str) { // Makes a string lowercase
	std::string result{ str };
	std::transform(result.begin(), result.end(), result.begin(),
		[](char c) { return std::tolower(c); });
	return result;
}

// Appends text to a CSV file
void save_to_csv(const std::string& text, const std::string& filename)
{
	std::ofstream out_file{ filename, std::ios::app };

	if (!out_file) {
		throw FileAccessException{ "Tiedoston " + filename + " avaaminen epäonnistui!" };
	}

	out_file << text << std::endl;
	out_file.close();
}

void clear_file(const std::string& filename)
{
	std::ofstream out_file{ filename, std::ios::trunc };
	if (!out_file) {
		throw FileAccessException{ "Tiedoston " + filename + " avaaminen epäonnistui!" };
	}
	out_file.close();
}

std::vector<std::string> get_data_from_csv(const std::string& filename)
{
	std::ifstream in_file{ filename };

	if (!in_file) {
		throw FileAccessException{ "Tiedostoa " + filename + " ei löytynyt!" };
	}

	std::vector<std::string> data{};
	std::string item{};
	while (std::getline(in_file, item)) {
		data.push_back(item);
	}

	in_file.close();
	return data;
}

void trim(std::string& str)
{
	// Poistetaan alusta välilyönnit
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
	// Poistetaan lopusta välilyönnit
	str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), str.end());
}

/*
	Prints two columns of text, for example:
	word1           word2
*/
std::string two_column_text(
	int print_width,
	const std::string& left_text,
	int padding_left,
	std::string separator,
	const std::string& right_text,
	int padding_right)
{
	std::ostringstream os;

	os << separator
		<< std::setw(padding_left) << " "
		<< std::left << std::setw(print_width / 2 - padding_right) << left_text
		<< std::right << std::setw(print_width / 2 - padding_right) << right_text
		<< std::setw(padding_right) << " "
		<< separator << std::endl;

	return os.str();

}
