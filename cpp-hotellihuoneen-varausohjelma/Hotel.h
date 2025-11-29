#pragma once

#include "Room.h"
#include <iostream>
#include <string>
#include <vector>
#include "IPrintable.h"
#include "Reservation.h"
#include <unordered_map>
#include <memory>


class Hotel : public IPrintable
{
private:
	static std::unordered_map<RoomType, std::vector<std::shared_ptr<Room>>> generate_rooms(int num_rooms);

	int min_reservation_id{ 10000 };
	int max_reservation_id{ 99999 };

	std::string name;
	std::unordered_map<RoomType, std::vector<std::shared_ptr<Room>>> rooms_map;
	std::unordered_map<int, std::shared_ptr<Reservation>> reservations_map;
	std::vector<double> sale_percentages;
	std::string csv_file_name;

	size_t get_num_rooms() const;
	size_t get_num_rooms(RoomType room_type) const;
	bool reservation_id_exists(int id) const;
	void add_reservation_from_csv_line(std::string csv_line);
	void save_reservation_to_csv(std::shared_ptr<const Reservation> reservation) const;
public:
	virtual void print(std::ostream& os) const override;

	Hotel(const std::string& name, int rooms_to_generate, const std::string& csv_file_name = "");
	virtual ~Hotel() = default;

	// Huoneet
	size_t get_num_rooms_available() const;
	size_t get_num_rooms_available(RoomType room_type) const;
	std::shared_ptr<Room> get_available_room(RoomType room_type) const;
	std::shared_ptr<Room> get_room_by_number(int room_number);

	// Varaukset
	void list_reservations() const;
	std::shared_ptr<const Reservation> get_reservation_by_id(int reservation_id) const;
	std::vector<std::shared_ptr<const Reservation>> get_reservations_by_guest_name(const std::string& guest_name) const;
	std::shared_ptr<const Reservation> create_reservation(const int room_number, const std::string& guest_name, const int num_nights);
	void remove_reservation(int id);
	const std::string& get_csv_file_name();

	void load_reservations_from_csv();
	void save_reservations_to_csv() const;
};

