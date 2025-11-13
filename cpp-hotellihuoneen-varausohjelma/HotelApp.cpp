#include "HotelApp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "utils.h"
#include <string>
#include "MenuOption.h"
#include "Room.h"


const int HotelApp::print_width{ 35 };

HotelApp::HotelApp(const std::string& hotel_name, int num_rooms) : hotel{ hotel_name, num_rooms }, is_running{ true },
menu_options{
	{ "Luo varaus", [this]() { create_reservation(); } },
	{ "Näytä kaikki varaukset", [this]() { show_reservations(); } },
	{ "Hae varaus varausnumerolla", [this]() { find_reservation_by_id(); } },
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
		clear_screen();
		std::cout << hotel;
		print_menu_options();
		int choice = get_menu_choice();
		clear_screen();
		handle_choice(choice - 1);
		std::cout << std::endl;
		wait_for_input();
	}
}

/*
Valikon toiminnot
*/


void HotelApp::create_reservation()
{
	print_title("Varauksen luonti");

	// 1. Tarkistetaan onko huoneita vapaana
	if (hotel.get_num_rooms_available() == 0) {
		std::cerr << "Huoneita ei ole vapaana";
		return;
	}

	// 2. Annetaan varauksen tiedot
	std::cout << "Valitse huoneentyypi:\n";
	for (const auto& room_data : Room::room_type_data) {
		RoomType room_type = room_data.first;
		int num_available = hotel.get_num_rooms_available(room_type);
		std::cout << (static_cast<int>(room_type) + 1) << ": " << room_data.second.name << " (" << num_available << " kpl)" << std::endl;
	}

	std::cout << std::endl;

	RoomType room_type{};
	while (true) {
		int input{ get_input<int>("Huoneentyyppi: ", "Virheellinen syöte", false,
			[](int num) {return num >= 1 && num <= Room::room_type_data.size(); }) - 1 };

		room_type = static_cast<RoomType>(input);
		int num_rooms_available{ hotel.get_num_rooms_available(room_type) };
		if (num_rooms_available != 0) {
			// Vapaa huone löytyi
			break;
		}
		std::cout << "Huoneita ei vapaana." << std::endl;
	};

	auto room{ hotel.get_available_room(room_type) };
	int room_number = room->get_room_number();

	std::string guest_name{ get_input<std::string>("Varaajan nimi: ") };
	int num_nights{ get_input<int>("Montako yötä?: ", "Öitä pitää olla vähintää yksi", false, [](int n) {return n >= 1; }) };

	// 3. Luodaan varaus
	const Reservation& reservation{
		hotel.create_reservation(room_number, guest_name, num_nights)
	};
	std::cout << std::endl;
	std::cout << "Huone: " << room_number << " varattu " << std::endl;
	std::cout << "Hinta " << reservation.get_total_price() << " euroa (" << reservation.get_sale_percentage() << "% alennus)" << std::endl;
}
void HotelApp::show_reservations() const
{
	print_title("Kaikki varaukset");
	hotel.list_reservations();
}
void HotelApp::find_reservation_by_id() const
{
	print_title("Varauksen haku varausnumerolla");
}
void HotelApp::find_reservation_by_name() const
{
	print_title("Varauksen haku nimellä");
}
void HotelApp::remove_reservation() const
{
	print_title("Varauksen poisto");
}
void HotelApp::get_room_details() const
{
	print_title("Huoneen tietojen haku");
}
void HotelApp::show_available_rooms() const
{
	print_title("Vapaat huoneet");
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

void HotelApp::print_title(const std::string title) const
{
	print_line('=', HotelApp::print_width);
	std::cout << std::setw((HotelApp::print_width + title.length()) / 2) << title << std::endl;
	print_line('=', HotelApp::print_width);
}



