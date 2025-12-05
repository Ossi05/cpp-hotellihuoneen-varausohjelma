#pragma once
#include <unordered_map>
#include <string>

enum class RoomType {
	Single,
	Double,
};

struct RoomTypeData {
	std::string name;
	double price;
};

class Room {
	friend bool operator==(const Room& lhs, const int room_number);

private:
	const int room_number;
	const RoomType type;
	double price;
	bool _is_occupied;

public:
	static std::unordered_map<RoomType, RoomTypeData> ROOM_TYPE_DATA;
	static double get_price(RoomType type);

	Room(int room_number, RoomType type, bool is_occupied = false);

	void set_is_occupied(bool is_occupied);
	bool is_occupied() const;
	int get_room_number() const;
	double get_price() const;
	RoomType get_room_type() const;

};

