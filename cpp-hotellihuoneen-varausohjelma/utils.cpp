#include "utils.h"
#include <random>
#include <iomanip>
#include <string>

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
