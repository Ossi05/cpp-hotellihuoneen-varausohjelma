#pragma once

#include "Room.h"
#include <iostream>
#include <string>
#include <vector>
#include "IPrintable.h"


class Hotel : public IPrintable
{
private:
	static std::vector<Room> generate_rooms(int num_rooms);
	static const int print_width;
	std::string name;
	std::vector<Room> rooms;

public:
	virtual void print(std::ostream& os) const override;

	Hotel(const std::string& name, int rooms_to_generate);

	int get_rooms_available_amt() const;

	void print_menu_options() const;


};

