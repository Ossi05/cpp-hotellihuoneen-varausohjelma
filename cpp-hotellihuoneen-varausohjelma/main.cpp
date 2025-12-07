#include "HotelApp.h"
#include <clocale>
#include "utils.h"
#include <string>
#include "exceptions.hpp"

int main() {

	// Use Finnish locale for formatting
	std::setlocale(LC_ALL, "fi_FI");

	std::string config_file_name{ "config.cfg" };
	try {
		HotelApp app{ config_file_name };
		app.run();
	}
	catch (const HotelAppInitializationError& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}