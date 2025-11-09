#pragma once
#include "Hotel.h"
#include <vector>
#include <string>
#include "MenuOption.h"

class HotelApp
{
public:
	static const int print_width;
	HotelApp(const std::string& hotel_name, int num_rooms);
	void run();
private:
	std::vector <MenuOption> menu_options;
	const Hotel hotel;
	bool is_running;

	void print_menu_options() const;
	void handle_choice(int choice) const;
	int get_menu_choice() const;

	// Valikon toiminnot
	void create_reservation() const;
	void show_reservations() const;
	void find_reservation_by_number() const;
	void find_reservation_by_name() const;
	void remove_reservation() const;
	void get_room_details() const;
	void show_available_rooms() const;
	void handle_exit_program();
};

