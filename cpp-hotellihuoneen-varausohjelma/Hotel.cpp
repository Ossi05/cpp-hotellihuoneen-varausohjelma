#include "Hotel.h"
#include "Room.h"
#include <iomanip>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include <ios>
#include "HotelApp.h"
#include "utils.h"
#include <memory>
#include <unordered_map>
#include <sstream>
#include "exceptions.hpp"

/*
	Generates rooms for the hotel
*/
std::unordered_map<RoomType, std::vector<std::shared_ptr<Room>>> Hotel::generate_rooms(int num_rooms)
{
	std::unordered_map<RoomType, std::vector<std::shared_ptr<Room>>> rooms;

	int num_room_types{ static_cast<int>(Room::ROOM_TYPE_DATA.size()) }; // Get number of room types
	int rooms_per_type{ num_rooms / num_room_types };
	int extra_rooms{ num_rooms % num_room_types }; // Extra rooms that were left after division

	int room_number{ 1 };

	for (int i = 0; i < num_room_types; i++) // Create rooms for each room type
	{
		RoomType room_type{ static_cast<RoomType>(i) };
		int rooms_to_create{ rooms_per_type + (i < extra_rooms ? 1 : 0) }; // Add an extra room if needed
		for (int j = 0; j < rooms_to_create; j++)
		{
			rooms[room_type].push_back(std::make_shared<Room>(room_number++, room_type));
		}
	}
	return rooms;
}

Hotel::Hotel(const std::string& name, int rooms_to_generate, const std::vector<double>& sale_percentages,
	int min_reservation_id, int max_reservation_id) :
	name{ name },
	rooms_map{ generate_rooms(rooms_to_generate) },
	sale_percentages{ sale_percentages }
{
}

const std::vector<double>& Hotel::get_sale_percentages() const { return sale_percentages; }
int Hotel::get_min_reservation_id() const { return reservation_manager.get_min_reservation_id(); }
int Hotel::get_max_reservation_id() const { return reservation_manager.get_max_reservation_id(); }
bool Hotel::has_available_reservation_ids() const { return reservation_manager.has_available_reservation_ids(); }

/*
	Rooms
*/
const std::string& Hotel::get_name() const { return name; }

size_t Hotel::get_num_rooms_available() const
{
	size_t num_available{ 0 };
	for (const auto& pair : rooms_map) {
		num_available += get_num_rooms_available(pair.first);
	}

	return num_available;
}
size_t Hotel::get_num_rooms_available(RoomType room_type) const
{
	return std::count_if(rooms_map.at(room_type).begin(), rooms_map.at(room_type).end(), [room_type](const std::shared_ptr<Room>& room) {return !room->is_occupied(); });
}

std::shared_ptr<Room> Hotel::get_available_room(RoomType room_type) const
{
	const auto& rooms = rooms_map.at(room_type);
	auto result = std::find_if(
		rooms.begin(), rooms.end(),
		[](const std::shared_ptr<Room>& r) { return !r->is_occupied(); }
	);

	if (result == rooms.end()) {
		throw RoomNotAvailableException{ "Ei vapaita huoneita tässä huonetyypissä" };
	}

	return *result;
}

size_t Hotel::get_num_rooms() const
{
	size_t num_rooms{ 0 };
	for (const auto& pair : rooms_map) {
		num_rooms += pair.second.size();
	}
	return num_rooms;
}

size_t Hotel::get_num_rooms(RoomType room_type) const
{
	return rooms_map.at(room_type).size();
}

std::shared_ptr<Room> Hotel::get_room_by_number(int room_number)
{
	for (auto& pair : rooms_map) {
		auto& rooms = pair.second;
		auto result{
			std::find_if(rooms.begin(), rooms.end(),
				[room_number](const std::shared_ptr<Room>& r) {return r->get_room_number() == room_number; })
		};
		if (result != rooms.end()) {
			return *result;
		}
	}

	throw RoomNotFoundException{ "Huonetta " + std::to_string(room_number) + " ei löytynyt." };
}

/*
	Reservations
*/

/*
	Creates a new reservation

	1. Check for available rooms and reservation IDs
	2. Generate a unique reservation ID
	3. Create reservation
	4. Add reservation to list
*/
std::shared_ptr<const Reservation> Hotel::create_reservation(const int room_number, const std::string& guest_name, const int num_nights)
{
	// 1. Check for available rooms and reservation IDs
	if (get_num_rooms_available() == 0) {
		throw RoomNotAvailableException{ "Huoneita ei ole vapaana" };
	}
	if (!has_available_reservation_ids()) {
		throw NoAvailableReservationIdsException{ "Ei vapaita varausnumeroita" };
	}

	// 2. Generate a unique reservation ID
	int attempts{ 1000 };
	int id{};
	do { // Try to get a random ID
		id = get_random_number(get_min_reservation_id(), get_max_reservation_id());
		attempts--;
	} while (reservation_id_exists(id) && attempts > 0);
	if (attempts == 0) {
		// Couldn't get random ID, get the first available one
		id = reservation_manager.get_available_reservation_id();
	}

	auto room{ get_room_by_number(room_number) };
	double sale_percentage{
		sale_percentages.at(get_random_number(0, static_cast<int>(sale_percentages.size()) - 1))
	};

	// 3. Create reservation
	auto reservation_ptr = std::make_shared<Reservation>(
		id,
		guest_name,
		room_number,
		num_nights,
		room->get_price() * num_nights, // Hinta on huoneen hinta * yöt
		sale_percentage
	);
	reservation_ptr->assign_room(room);
	reservation_manager.add_reservation(reservation_ptr);

	return reservation_ptr;
}

void Hotel::remove_reservation(int id)
{
	reservation_manager.remove_reservation(id);
}

std::vector<std::shared_ptr<const Reservation>> Hotel::get_all_reservations() const
{
	return reservation_manager.get_all_reservations();
}

std::shared_ptr<const Reservation> Hotel::get_reservation_by_id(int reservation_id) const
{
	return reservation_manager.get_reservation_by_id(reservation_id);
}

std::vector<std::shared_ptr<const Reservation>> Hotel::get_reservations_by_guest_name(const std::string& guest_name) const
{
	return reservation_manager.get_reservations_by_guest_name(guest_name);
}

void Hotel::list_reservations() const
{
	reservation_manager.list_reservations();
}

bool Hotel::reservation_id_exists(int id) const
{
	return reservation_manager.reservation_id_exists(id);
}

// Tulostus
void Hotel::print(std::ostream& os) const
{
	const std::streamsize padding_left{ 2 };
	const std::streamsize padding_right{ 4 };

	// Title
	Menu::print_two_col_text(name);
	print_line('=', Menu::print_width);

	// Basic info
	const size_t num_rooms{ get_num_rooms() };
	const size_t rooms_available{ get_num_rooms_available() };
	Menu::print_two_col_text("Huoneita 1-" + std::to_string(num_rooms));
	Menu::print_two_col_text("Vapaana: " + std::to_string(rooms_available));
	Menu::print_two_col_text("Varattu: " + std::to_string(num_rooms - rooms_available));

	Menu::print_two_col_text("");

	// Print room types
	Menu::print_two_col_text("Tyyppi", "Vapaana");
	std::cout << std::setfill('-') << std::setw(Menu::print_width) << "" << std::setfill(' ') << std::endl;

	for (const auto& room : rooms_map)
	{
		std::string available_text{
			std::to_string(get_num_rooms_available(room.first))
			+ "/"
			+ std::to_string(get_num_rooms(room.first)) };
		Menu::print_two_col_text(Room::ROOM_TYPE_DATA.at(room.first).name, available_text);
	}

	print_line('=', Menu::print_width);
	std::cout << std::endl;
}
