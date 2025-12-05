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
	int min_reservation_id;
	int max_reservation_id;
public:
	ReservationManager(int min_reservation_id = 10000, int max_reservation_id = 99999);
	void list_reservations() const;
	void add_reservation(std::shared_ptr<Reservation> reservation);
	void remove_reservation(int id);
	bool reservation_id_exists(int id) const;
	bool has_available_reservation_ids() const;
	int get_min_reservation_id() const;
	int get_max_reservation_id() const;
	int get_available_reservation_id() const;
	std::shared_ptr<Reservation> get_reservation_by_id(int reservation_id) const;
	std::vector<std::shared_ptr<const Reservation>> get_reservations_by_guest_name(const std::string& guest_name) const;
	std::vector<std::shared_ptr<const Reservation>> get_all_reservations() const;
};

