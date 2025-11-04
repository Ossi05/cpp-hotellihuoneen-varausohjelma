#include "Reservation.h"

Reservation::Reservation(const std::string& guest_name, int room_number, int num_nights, double normal_price, double sale_percentage) :
	guest_name{ guest_name }, room_number{ room_number }, num_nights{ num_nights }, normal_price{ normal_price }, sale_percentage {
	sale_percentage
} {}

double Reservation::get_total_price() const
{
	return normal_price * (1 - sale_percentage / 100);
}

std::string Reservation::get_guest_name() const { return guest_name; }
int Reservation::get_room_number() const { return room_number; }
double Reservation::get_normal_price() const { return normal_price; }
double Reservation::get_sale_percentage() const { return sale_percentage; }

