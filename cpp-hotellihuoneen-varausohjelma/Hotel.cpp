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

std::unordered_map<RoomType, std::vector<std::shared_ptr<Room>>> Hotel::generate_rooms(int num_rooms)
{
	std::unordered_map<RoomType, std::vector<std::shared_ptr<Room>>> rooms;

	for (int i = 0; i < num_rooms; i++)
	{
		// Huoneen numero ja tyyppi (parilliset yhden hengen, parittomat kahden hengen)
		RoomType room_type{ i % 2 == 0 ? RoomType::Single : RoomType::Double };
		rooms[room_type].push_back(std::make_shared<Room>(i + 1, room_type));
	}

	return rooms;
}

Hotel::Hotel(const std::string& name, int rooms_to_generate) :
	name{ name },
	rooms_map{ generate_rooms(rooms_to_generate) },
	reservations{},
	sale_percentages{ 0, 10, 20 }
{
}

int Hotel::get_num_rooms_available() const
{
	int num_available{ 0 };
	for (const auto& pair : rooms_map) {
		num_available += get_num_rooms_available(pair.first);
	}

	return num_available;
}

int Hotel::get_num_rooms_available(RoomType room_type) const
{
	return std::count_if(rooms_map.at(room_type).begin(), rooms_map.at(room_type).end(), [room_type](const std::shared_ptr<Room>& room) {return !room->is_occupied(); });
}

Reservation& Hotel::create_reservation(const int room_number, const std::string& guest_name, const int num_nights)
{
	// Tarkistetaan onko huoneita vapaana
	if (get_num_rooms_available() == 0) {
		std::cerr << "Huoneita ei ole vapaana" << std::endl;
		throw std::runtime_error("Huoneita ei ole vapaana");
	}

	int id{ get_random_number(min_reservation_id, max_reservation_id) };

	// TODO Lisää montako kertaa yritetään luoda random id
	while (reservation_id_exists(id)) {
		std::cout << "Tetst";
		id = get_random_number(min_reservation_id, max_reservation_id);
	}

	auto room{ get_room_by_number(room_number) };

	double sale_percentage{
		sale_percentages.at(get_random_number(0, static_cast<int>(sale_percentages.size()) - 1))
	};

	// Luodaan varaus
	reservations.emplace_back(id,
		guest_name,
		room,
		num_nights,
		room->get_price() * num_nights, // Hinta on huoneen hinta * yöt
		sale_percentage);

	return reservations.back();
}

const Reservation& Hotel::get_reservation_by_id(int reservation_id) const
{
	auto result{ std::find(reservations.begin(), reservations.end(), reservation_id) };

	if (result != reservations.end()) return *result;

	throw std::runtime_error("Varausta ei löytynyt");
}

std::shared_ptr<Room> Hotel::get_available_room(RoomType room_type) const
{
	const auto& rooms = rooms_map.at(room_type);
	auto result = std::find_if(
		rooms.begin(), rooms.end(),
		[](const std::shared_ptr<Room>& r) { return !r->is_occupied(); }
	);

	if (result == rooms.end()) {
		throw std::runtime_error("Ei vapaita huoneita tässä huonetyypissä");
	}

	return *result;
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

	throw std::runtime_error("Huonetta ei löytynyt");
}

void Hotel::list_reservations() const
{
	if (!reservations.size())
	{
		std::cout << "Ei varauksia." << std::endl;
		return;
	}
	for (const auto& reservation : reservations)
	{
		std::cout << reservation << std::endl;
	}
}

int Hotel::get_num_rooms() const
{
	int num_rooms{ 0 };
	for (const auto& pair : rooms_map) {
		num_rooms += pair.second.size();
	}
	return num_rooms;
}

bool Hotel::reservation_id_exists(int id) const
{
	return std::any_of(reservations.begin(), reservations.end(),
		[id](const Reservation& r) { return r == id; });
}

// Tulostus
void Hotel::print(std::ostream& os) const
{
	const std::streamsize text_spacing_left{ 2 };
	const std::streamsize border_width{ 2 };

	const auto print_text = [&os, text_spacing_left](const std::string& text) {
		os << "|"
			<< std::setw(text_spacing_left) << " "
			<< std::left << std::setw(HotelApp::print_width - text_spacing_left - border_width) << text
			<< "|" << '\n';
		os << std::right;
		};

	print_text(name);
	print_line('=', HotelApp::print_width);

	const int num_rooms{ get_num_rooms() };
	const int rooms_available = get_num_rooms_available();
	print_text("Huoneita 1-" + std::to_string(num_rooms));
	print_text("Vapaana: " + std::to_string(rooms_available));
	print_text("Varattu: " + std::to_string(num_rooms - rooms_available));
	print_text("");

	for (const auto& room : rooms_map)
	{
		print_text(Room::room_type_data.at(room.first).name + ": " + std::to_string(get_num_rooms_available(room.first)) + " jäljellä");
	}

	print_line('=', HotelApp::print_width);
	std::cout << std::endl;

}
