#pragma once
#include <string>
#include "Reservation.h"
#include <vector>
#include <memory>

/*
	Handles saving and loading reservations to and from a CSV file.
*/
class CSVReservationHandler
{
private:
	std::string file_name;
public:
	static const char CSV_SEPARATOR;
	CSVReservationHandler(const std::string& file_name);
	void save_reservation(std::shared_ptr<const Reservation> reservation) const;
	void save_all(const std::vector<std::shared_ptr<const Reservation>>& reservations) const;
	void remove_reservation(int id) const;
	void clear() const;
	std::vector<std::shared_ptr<Reservation>> load_reservations() const;
	std::string get_file_name() const;
	bool has_file_name() const;
};

