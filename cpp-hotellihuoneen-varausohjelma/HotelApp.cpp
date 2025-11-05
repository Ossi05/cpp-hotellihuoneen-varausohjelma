#include "HotelApp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "utils.h"
#include <string>


const int HotelApp::print_width{ 35 };

const std::vector <std::string> HotelApp::menu_options{
	"Tee uusi varaus",
	"Näytä kaikki varaukset",
	"Hae varausta varausnumerolla",
	"Hae varausta varaajan nimellä",
	"Poista varaus",
	"Hae huoneen tiedot",
	"Näytä vapaat huoneet",
	"Lopeta ohjelma"
};


HotelApp::HotelApp(const std::string& hotel_name, int num_rooms) : hotel{ hotel_name, num_rooms } {
}

void HotelApp::print_menu_options() const
{
	for (size_t i = 0; i < menu_options.size(); i++)
	{
		std::cout << i + 1 << " " << menu_options.at(i) << std::endl;
	}
	std::cout << std::setfill('-') << std::setw(HotelApp::print_width) << "" << std::setfill(' ') << std::endl;
}

void HotelApp::get_handle_input() const
{
	const std::string msg{ "Valitse toiminto (1-" + std::to_string(menu_options.size()) + "): " };
	const int input = get_input<int>(
		msg,
		"Tuntematon komento",
		false,
		[](int& value) { return value >= 1 && value <= static_cast<int>(HotelApp::menu_options.size()); }
	);

}

void HotelApp::print(std::ostream& os) const
{
	os << hotel;
}
