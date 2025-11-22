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
	for (size_t i{}; i < rooms_map.at(RoomType::Single).size(); i++) {
		if (i % 3 == 0) {
			// Feikki varauksia
			create_reservation(i + 1, "Matti Meikäläinen", 2);
		}
	}
}

/*
	Huoneet
*/
// Palauttaa, montako huonetta on vapaana yhteensä
size_t Hotel::get_num_rooms_available() const
{
	size_t num_available{ 0 };
	for (const auto& pair : rooms_map) {
		num_available += get_num_rooms_available(pair.first);
	}

	return num_available;
}
// Palauttaa, montako huonetta on vapaana tietyssä huonetyypissä
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
		throw std::runtime_error("Ei vapaita huoneita tässä huonetyypissä");
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

	throw std::runtime_error("Huonetta ei löytynyt");
}

/*
	Varaukset
*/
const Reservation& Hotel::create_reservation(const int room_number, const std::string& guest_name, const int num_nights)
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

std::vector<const Reservation*> Hotel::get_reservations_by_guest_name(const std::string& guest_name) const
{	
	std::vector<const Reservation*> results;
	for (const auto& reservation : reservations)
	{	
		if (to_lower(reservation.get_guest_name()).find(to_lower(guest_name)) == std::string::npos) continue;
		results.push_back(&reservation);
	}
	return results;
}

void Hotel::list_reservations() const
{
	if (!reservations.size())
	{
		std::cout << "Ei varauksia." << std::endl;
		return;
	}

	int field1_width{ 8 }, field2_width{ 25 }, field3_width{ 15 },
		field4_width{ 10 };

	int total_width{ field1_width + field2_width + field3_width + field4_width };

	std::cout << std::endl << std::setw(field1_width) << std::left << "ID"
		<< std::setw(field2_width) << std::left << "Varaaja"
		<< std::setw(field3_width) << std::left << "Tyyppi"
		<< std::setw(field4_width) << std::right << "Huone" << std::endl;
	std::cout << std::setfill('-') << std::setw(total_width) << "" << std::setfill(' ') << std::endl;

	for (const auto& reservation : reservations)
	{
		std::cout << std::setw(field1_width) << std::left << reservation.get_id()
			<< std::setw(field2_width) << std::left << reservation.get_guest_name()
			<< std::setw(field3_width) << std::left << Room::room_type_data.at(reservation.get_room_type()).name
			<< std::setw(field4_width) << std::right << reservation.get_room_number()
			<< std::endl;

		std::cout << std::endl;
	}
}

bool Hotel::reservation_id_exists(int id) const
{
	return std::any_of(reservations.begin(), reservations.end(),
		[id](const Reservation& r) { return r == id; });
}

// Tulostus
void Hotel::print(std::ostream& os) const
{
	const std::streamsize padding_left{ 2 };
	const std::streamsize padding_right{ 4 };

	static const auto print_text = [&os](const std::string& left_text, const std::string& right_text = "") {
		os << "|"
			<< std::setw(padding_left) << " "
			<< std::left << std::setw(Menu::print_width / 2 - padding_right) << left_text
			<< std::right << std::setw(Menu::print_width / 2 - padding_right) << right_text
			<< std::setw(padding_right) << " "
			<< "|" << '\n';
		os << std::right;
		};

	// Otsikko
	print_text(name);
	print_line('=', Menu::print_width);

	// Perustietoja
	const size_t num_rooms{ get_num_rooms() };
	const size_t rooms_available{ get_num_rooms_available() };
	print_text("Huoneita 1-" + std::to_string(num_rooms));
	print_text("Vapaana: " + std::to_string(rooms_available));
	print_text("Varattu: " + std::to_string(num_rooms - rooms_available));

	print_text("");


	print_text("Tyyppi", "Vapaana");

	std::cout << std::setfill('-') << std::setw(Menu::print_width) << "" << std::setfill(' ') << std::endl;

	for (const auto& room : rooms_map)
	{
		std::string available_text{
			std::to_string(get_num_rooms_available(room.first))
			+ "/"
			+ std::to_string(get_num_rooms(room.first)) };
		print_text(Room::room_type_data.at(room.first).name, available_text);
	}

	print_line('=', Menu::print_width);
	std::cout << std::endl;
}
