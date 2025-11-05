#include <iostream>
#include "HotelApp.h"
#include <clocale>


int main() {

	std::setlocale(LC_ALL, "fi_FI");
	HotelApp hotelApp{ "C++ hotelli", 20 };
	std::cout << hotelApp;

	hotelApp.print_menu_options();
	hotelApp.get_handle_input();

	return 0;
}