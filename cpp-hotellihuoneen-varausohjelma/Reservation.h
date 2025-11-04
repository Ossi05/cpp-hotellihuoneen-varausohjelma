#pragma once
#include <string>

class Reservation
{
private:
	const std::string guest_name;
	const int room_number;
	const int num_nights;
	const double normal_price;
	const double sale_percentage;

public:
	Reservation(const std::string& guest_name, int room_number, int num_nights, double normal_price, double sale_percentage = 0);

	std::string get_guest_name() const;
	int get_room_number() const;
	double get_normal_price() const;
	double get_sale_percentage() const;
	double get_total_price() const;

};

