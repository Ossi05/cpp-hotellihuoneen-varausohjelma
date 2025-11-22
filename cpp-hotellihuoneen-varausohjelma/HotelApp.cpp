#include "HotelApp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "utils.h"
#include <string>
#include "MenuOption.h"
#include "Room.h"


HotelApp::HotelApp(const std::string& hotel_name, int num_rooms) : hotel{ hotel_name, num_rooms }, is_running{ true },
menu{ {
	{ "Luo varaus", [this]() { create_reservation(); } },
	{ "Näytä kaikki varaukset", [this]() { show_reservations(); } },
	{ "Hae varaus varausnumerolla", [this]() { find_reservation_by_id(); } },
	{ "Hae varauksia varaajan nimellä", [this]() { find_reservations_by_name(); } },
	{ "Poista varaus", [this]() { remove_reservation(); } },
	{ "Poistu ohjelmasta", [this]() { handle_exit_program(); }}
} }
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
		menu.print_menu_options();
		int choice{ menu.get_menu_choice() };
		clear_screen();
		menu.handle_choice(choice - 1);
		std::cout << std::endl;
		wait_for_input();
	}
}

/*
Valikon toiminnot
*/


void HotelApp::create_reservation()
{
	// 1. Tarkistetaan onko huoneita vapaana
	if (hotel.get_num_rooms_available() == 0) {
		std::cerr << "Huoneita ei ole vapaana";
		return;
	}

	// 2. Annetaan varauksen tiedot
	std::cout << "Valitse huoneentyypi:\n";
	for (const auto& room_data : Room::room_type_data) {
		RoomType room_type = room_data.first;
		size_t num_available{ hotel.get_num_rooms_available(room_type) };
		std::cout << (static_cast<int>(room_type) + 1) << ": " << room_data.second.name << " (" << num_available << " kpl)" << std::endl;
	}

	std::cout << std::endl;

	RoomType room_type{};
	while (true) {
		int input{ get_input<int>("Huoneentyyppi: ", "Virheellinen syöte", false,
			[](int num) {return num >= 1 && num <= Room::room_type_data.size(); }) - 1 };

		room_type = static_cast<RoomType>(input);
		size_t num_rooms_available{ hotel.get_num_rooms_available(room_type) };
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
	auto reservation{
		hotel.create_reservation(room_number, guest_name, num_nights)
	};
	std::cout << std::endl;
	std::cout << "Huone: " << room_number << " varattu " << std::endl;
	std::cout << "Hinta " << reservation->get_total_price() << " euroa (" << reservation->get_sale_percentage() << "% alennus)" << std::endl;
}
void HotelApp::show_reservations() const
{
	hotel.list_reservations();
}
void HotelApp::find_reservation_by_id() const
{
	int reservation_id{ get_input<int>("Anna varausnumero: ") };
	try {
		auto reservation{ hotel.get_reservation_by_id(reservation_id) };
		std::cout << std::endl << *reservation << std::endl;
	}
	catch (const std::exception&) {
		std::cerr << "Varausta ei löytynyt" << std::endl;
	}
}
void HotelApp::find_reservations_by_name() const
{
	std::string guest_name{};
	std::cout << "Anna varaajan nimi: ";
	std::getline(std::cin, guest_name);

	auto reservations{ hotel.get_reservations_by_guest_name(guest_name) };
	if (reservations.size() == 0) {
		std::cout << "Varauksia ei löytynyt" << std::endl;
		return;
	}

	std::cout << std::endl << "Tulokset:\n" << std::endl;
	for (const auto& reservation : reservations) {
		std::cout << *reservation << std::endl;
	}
}
void HotelApp::remove_reservation()
{
	int reservation_id{ get_input<int>("Anna varausnumero: ") };
	try {
		hotel.remove_reservation(reservation_id);
		std::cout << "Varaus (" << reservation_id << ") poistettu" << std::endl;
	}
	catch (const std::exception&) {
		std::cerr << "Varausta ei löytynyt" << std::endl;
	}

}

void HotelApp::handle_exit_program()
{
	std::cout << "Poistutaan ohjelmasta..." << std::endl;
	is_running = false;
}

