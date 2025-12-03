#include "ReservationManager.h"
#include <iostream>
#include <iomanip>
#include "utils.h"
#include <vector>
#include "exceptions.hpp"

void ReservationManager::remove_reservation(int id)
{
	auto reservation = get_reservation_by_id(id);
	reservation->unassign_room();

	// Poistetaan huone
	reservations_map.erase(id);
}

std::shared_ptr<Reservation> ReservationManager::get_reservation_by_id(int reservation_id) const
{
	auto result = reservations_map.find(reservation_id);
	if (result != reservations_map.end()) {
		return result->second;
	}

	throw ReservationNotFoundException{};
}

std::vector<std::shared_ptr<const Reservation>> ReservationManager::get_reservations_by_guest_name(const std::string& guest_name) const
{
	std::vector<std::shared_ptr<const Reservation>> results;
	for (const auto& pair : reservations_map)
	{
		if (to_lower(pair.second->get_guest_name()).find(to_lower(guest_name)) == std::string::npos) continue;

		results.push_back(pair.second);
	}
	return results;
}

bool ReservationManager::reservation_id_exists(int id) const
{
	return reservations_map.find(id) != reservations_map.end();
}

void ReservationManager::add_reservation(std::shared_ptr<Reservation> reservation)
{
	reservations_map.emplace(reservation->get_id(), reservation);
}

std::vector<std::shared_ptr<const Reservation>> ReservationManager::get_all_reservations() const {
	std::vector<std::shared_ptr<const Reservation>> results;
	for (const auto& pair : reservations_map)
	{
		results.push_back(pair.second);
	}
	return results;
}

void ReservationManager::list_reservations() const
{
	if (!reservations_map.size())
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

	for (const auto& pair : reservations_map)
	{
		const Reservation& reservation{ *pair.second };
		std::cout << std::setw(field1_width) << std::left << reservation.get_id()
			<< std::setw(field2_width) << std::left << reservation.get_guest_name()
			<< std::setw(field3_width) << std::left << Room::room_type_data.at(reservation.get_room_type()).name
			<< std::setw(field4_width) << std::right << reservation.get_room_number()
			<< std::endl;

		std::cout << std::endl;
	}
}