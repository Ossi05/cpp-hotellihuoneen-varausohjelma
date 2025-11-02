#pragma once
#include <iostream>

enum class RoomType {
	Single,
	Double
};

struct Room {

	const int room_number;
	const RoomType type;
	bool is_occupied;

	Room(int room_number, RoomType type, bool is_occupied = false);
};

