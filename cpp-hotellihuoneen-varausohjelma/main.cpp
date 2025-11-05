#include <iostream>
#include "Hotel.h"


int main() {

	std::setlocale(LC_ALL, "fi_FI");
	Hotel hotel("C++ hotelli", 20);
	std::cout << hotel;
	hotel.print_menu_options();

	return 0;
}