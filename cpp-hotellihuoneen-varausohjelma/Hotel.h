#pragma once

#include "Room.h"
#include <iostream>
#include <string>
#include <vector>
#include "IPrintable.h"


class Hotel : public IPrintable
{
private:
	std::string name;
	std::vector<Room> rooms;
	const int max_rooms;

public:
	Hotel(const std::string& name, int max_rooms);
	virtual void print(std::ostream& os) const override {
		os << name << std::endl;
	}

};

