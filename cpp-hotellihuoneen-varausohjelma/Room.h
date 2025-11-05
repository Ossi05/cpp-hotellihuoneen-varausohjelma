#pragma once
#include <unordered_map>

enum class RoomType {
	Single,
	Double
};

class Room {

private:
	static std::unordered_map<RoomType, double> prices;

	const int room_number;
	const RoomType type;
	double price;
	bool is_occupied;
public:
	Room(int room_number, RoomType type, bool is_occupied = false);

	int get_room_number() const;
	RoomType get_room_type() const;
	double get_price() const;
	bool get_is_occupied() const;
	void set_is_occupied(bool is_occupied);


	static double get_price(RoomType type);

};

