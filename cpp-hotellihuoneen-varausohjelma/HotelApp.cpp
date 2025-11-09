#include "HotelApp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "utils.h"
#include <string>
#include "MenuOption.h"

const int HotelApp::print_width{ 35 };

HotelApp::HotelApp(const std::string& hotel_name, int num_rooms) : hotel{ hotel_name, num_rooms }, is_running{ true },
menu_options{
	{ "Luo varaus", [this]() { create_reservation(); } },
	{ "Näytä kaikki varaukset", [this]() { show_reservations(); } },
	{ "Hae varaus varausnumerolla", [this]() { find_reservation_by_number(); } },
	{ "Hae varausta varaajan nimellä", [this]() { find_reservation_by_name(); } },
	{ "Poista varaus", [this]() { remove_reservation(); } },
	{ "Hae huoneen tiedot", [this]() { get_room_details(); } },
	{ "Näytä vapaat huoneet", [this]() { show_available_rooms(); } },
	{ "Poistu ohjelmasta", [this]() { handle_exit_program(); }}
}
{

}

/*
Hotellisovelluksen pääohjelma, joka suoritetaan alussa
*/
void HotelApp::run()
{
	is_running = true;

	while (is_running)
	{
		std::cout << hotel;
		print_menu_options();
		int choice = get_menu_choice();
		clear_screen();
		handle_choice(choice - 1);
		wait_for_input();
	}
}

/*
Valikon toiminnot
*/
void HotelApp::create_reservation() const
{
	std::cout << "Luo varaus" << std::endl;
}
void HotelApp::show_reservations() const
{
	std::cout << "Näytä kaikki varaukset" << std::endl;
}
void HotelApp::find_reservation_by_number() const
{
	std::cout << "Hae varaus varausnumerolla" << std::endl;
}
void HotelApp::find_reservation_by_name() const
{
	std::cout << "Hae varausta varaajan nimellä" << std::endl;
}
void HotelApp::remove_reservation() const
{
	std::cout << "Poista varaus" << std::endl;
}
void HotelApp::get_room_details() const
{
	std::cout << "Hae huoneen tiedot" << std::endl;
}
void HotelApp::show_available_rooms() const
{
	std::cout << "Näytä vapaat huoneet" << std::endl;
}

void HotelApp::handle_exit_program()
{
	std::cout << "Poistutaan ohjelmasta..." << std::endl;
	is_running = false;
}


/*
Apufunktioita
*/
void HotelApp::print_menu_options() const
{
	for (size_t i = 0; i < menu_options.size(); i++)
	{
		std::cout << i + 1 << " " << menu_options.at(i).name << std::endl;
	}
	std::cout << std::setfill('-') << std::setw(HotelApp::print_width) << "" << std::setfill(' ') << std::endl;
}

int HotelApp::get_menu_choice() const
{
	const std::string msg{ "Valitse toiminto (1-" + std::to_string(menu_options.size()) + "): " };
	const int input = get_input<int>(
		msg,
		"Tuntematon komento",
		false,
		[this](int& value) { return value >= 1 && value <= static_cast<int>(menu_options.size()); }
	);
	return input;
}

void HotelApp::handle_choice(int choice) const
{
	// TODO VIRHEENKÄSITTELY

	// Kutsutaan valittua toimintoa
	menu_options.at(choice).action();
}



