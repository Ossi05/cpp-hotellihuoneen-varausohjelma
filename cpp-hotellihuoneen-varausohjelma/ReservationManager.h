#pragma once
#include <unordered_map>
#include <memory>
#include "Reservation.h"
#include <vector>
#include <functional>

/*
	Manages reservations.
*/
class ReservationManager
{
private:
	std::unordered_map<int, std::shared_ptr<Reservation>> reservations_map;
public:
	void list_reservations() const;
	std::shared_ptr<Reservation> get_reservation_by_id(int reservation_id) const;
	std::vector<std::shared_ptr<const Reservation>> get_reservations_by_guest_name(const std::string& guest_name) const;
	void remove_reservation(int id);
	bool reservation_id_exists(int id) const;
	void add_reservation(std::shared_ptr<Reservation> reservation);
	std::vector<std::shared_ptr<const Reservation>> get_all_reservations() const;
};

