#pragma once
#include "Hotel.h"
#include <vector>
#include <string>
#include "MenuOption.h"
#include "Menu.h"

class HotelApp
{
public:
	HotelApp(const std::string& hotel_name, int num_rooms);
	void run();
private:
	Menu menu;
	Hotel hotel;
	bool is_running;

	// Valikon toiminnot
	void handle_exit_program();

	// Varaukset
	void create_reservation();
	void show_reservations() const;
	void find_reservation_by_id() const;
	void find_reservations_by_name() const;
	void remove_reservation();

};

