#include "utils.h"
#include <random>
#include <iomanip>

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
