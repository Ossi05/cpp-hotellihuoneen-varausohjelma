#pragma once
#include <string>
#include "IPrintable.h"
#include "Room.h"
#include <memory>

class Reservation : public IPrintable
{
	friend bool operator==(const Reservation& lhs, const int id);

private:
	std::weak_ptr<Room> room;
	const int id;
	const std::string guest_name;
	const int num_nights;

	const double normal_price;
	const double sale_percentage;

public:
	Reservation(int id,
		const std::string& guest_name,
		std::weak_ptr<Room> room,
		int num_nights,
		double normal_price,
		double sale_percentage = 0
	);

	std::string get_guest_name() const;
	int get_room_number() const;
	double get_normal_price() const;
	double get_sale_percentage() const;
	double get_total_price() const;
	int get_id() const;
	int get_num_nights() const;
	RoomType get_room_type() const;

	virtual void print(std::ostream& os) const override;
	virtual ~Reservation() = default;
	std::string to_csv() const;

};

