#include "HotelApp.h"
#include <clocale>
#include "utils.h"
#include <string>

int main() {

	std::string config_file_name{ "config.cfg" };

	std::setlocale(LC_ALL, "fi_FI");
	HotelApp app{ config_file_name };
	app.run();
	return 0;
}