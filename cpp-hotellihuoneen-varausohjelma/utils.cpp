#include "utils.h"
#include <random>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>

const char CSV_SEPERATOR{ ',' };

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

std::string to_lower(const std::string& str) {
	std::string result{ str };
	std::transform(result.begin(), result.end(), result.begin(),
		[](char c) { return std::tolower(c); });
	return result;
}

void save_to_csv(const std::string& text, const std::string& filename)
{
	std::ofstream out_file{ filename, std::ios::app };

	if (!out_file) {
		throw std::runtime_error("Tiedoston " + filename + " avaaminen epäonnistui!");
	}

	out_file << text << std::endl;
	out_file.close();
}

void clear_file(const std::string& filename)
{
	std::ofstream out_file{ filename, std::ios::trunc };
	if (!out_file) {
		throw std::runtime_error("Tiedoston " + filename + " avaaminen epäonnistui!");
	}
	out_file.close();
}

std::vector<std::string> get_data_from_csv(const std::string& filename)
{	
	std::ifstream in_file{ filename };
	
	if (!in_file) {
		throw std::runtime_error("Tiedostoa " + filename + " ei löytynyt!");
	}

	std::vector<std::string> data{};
	std::string item{};
	while (std::getline(in_file, item)) {
		data.push_back(item);
	}

	in_file.close();
	return data;
}
