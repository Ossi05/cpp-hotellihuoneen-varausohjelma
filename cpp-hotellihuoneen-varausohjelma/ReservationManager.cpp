#include "ReservationManager.h"
#include <iostream>
#include <iomanip>
#include "utils.h"
#include <vector>
#include "exceptions.hpp"
#include <algorithm>

ReservationManager::ReservationManager(int min_reservation_id, int max_reservation_id) :
	min_reservation_id{ min_reservation_id },
	max_reservation_id{ max_reservation_id }
{
}

void ReservationManager::add_reservation(std::shared_ptr<Reservation> reservation)
{
	reservations_map.emplace(reservation->get_id(), reservation);
}

void ReservationManager::remove_reservation(int id)
{
	auto reservation = get_reservation_by_id(id);
	reservation->unassign_room();
	reservations_map.erase(id);
}

bool ReservationManager::reservation_id_exists(int id) const
{
	return reservations_map.find(id) != reservations_map.end();
}
bool ReservationManager::has_available_reservation_ids() const
{	// Checks if there are available reservation IDs based on the vector's size
	return reservations_map.size() < static_cast<size_t>(max_reservation_id - min_reservation_id + 1);
}
int ReservationManager::get_min_reservation_id() const { return min_reservation_id; }
int ReservationManager::get_max_reservation_id() const { return max_reservation_id; }
int ReservationManager::get_available_reservation_id() const
{
	for (int id = min_reservation_id; id <= max_reservation_id; ++id) {
		if (reservation_id_exists(id)) { continue; }
		return id;
	}
	throw NoAvailableReservationIdsException{};
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
			<< std::setw(field3_width) << std::left << Room::ROOM_TYPE_DATA.at(reservation.get_room_type()).name
			<< std::setw(field4_width) << std::right << reservation.get_room_number()
			<< std::endl;

		std::cout << std::endl;
	}
}